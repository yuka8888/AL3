#include "Player.h"
#include "MapChipField.h"

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	worldTransform_.UpdateMatrix();

	// 接地状態
	if (onGround_) {
		// 移動
		Move(velocity_);
	} else {

		// 落下速度
		velocity_.x += 0;
		velocity_.y += -kGravityAcceleration;
		velocity_.z += 0;
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;

	// マップ衝突チェック
	MapCollisionTop(collisionMapInfo);

	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 3.0f / 60.0f;

		// 左右の自キャラテーブル
		float destinationRotationYTable[]{std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 2.0f / 2.0f};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, destinationRotationY, easeInOutCubic(turnTimer_));
	}

	// 移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 下降中？
	if (velocity_.y < 0) {
		// y座標が地面いかになったら着地
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	}

	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めりこみ排斥
			worldTransform_.translation_.y = 2.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }

void Player::Move(Vector3& velocity) {
	// 移動入力
	// 左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			// 急ブレーキ
			if (velocity.x < 0.0f) {
				velocity.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;

			// 方向変換
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = 1.0f;
			}
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

			if (velocity.x > 0.0f) {
				velocity.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;

			// 方向変換
			if (lrDirection_ != LRDirection::KLeft) {
				lrDirection_ = LRDirection::KLeft;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = 1.0f;
			}
		}
		// 加速、減速
		velocity.x += acceleration.x;
		velocity.y += acceleration.y;
		velocity.z += acceleration.z;

		velocity.x = std::clamp(velocity.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		velocity_.x *= (1.0f - kAttenuation);
	}

	if (Input::GetInstance()->PushKey(DIK_UP)) {
		// ジャンプ初速
		velocity_.x += 0;
		velocity_.y += kJumpAcceleration;
		velocity_.z += 0;
	}
}

void Player::MapCollisionTop(CollisionMapInfo& info) {
	//上昇あり？
	if (info.move.y <= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	//真上の当たり判定を行う
	bool hit = false;

	//左上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
	//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
		//めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, velocity_.y);
		//天井に当たったことを記録する
		info.isCollideCeiling = true;
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f + center.x, -kHeight / 2.0f + center.y, 0 + center.z},
	    {-kWidth / 2.0f + center.x, -kHeight / 2.0f + center.y, 0 + center.z},
	    {+kWidth / 2.0f + center.x, +kHeight / 2.0f + center.y, 0 + center.z},
	    {-kWidth / 2.0f + center.x, +kHeight / 2.0f + center.y, 0 + center.z}
    };
	return offsetTable[static_cast<uint32_t>(corner)];
}

WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

void Player::SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
