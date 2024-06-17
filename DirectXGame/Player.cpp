#include "Player.h"
#include "MapChipField.h"

enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // 要素数
};

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

	// 移動
	Move();

	CollisionMapInfo collisionMapChipInfo;

	collisionMapChipInfo.velocity = velocity_;

	// マップ衝突チェック
	MapCollision(collisionMapChipInfo);

	// 地面との当たり判定
	SwitchingOnGround(collisionMapChipInfo);

	// 判定結果を反映して移動
	MoveByMapCollisionResult(collisionMapChipInfo);

	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 3.0f / 60.0f;
		// 左右の自キャラテーブル
		float destinationRotationYTable[]{std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, destinationRotationY, easeInOutCubic(turnTimer_));
	}

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }

void Player::Move() {

	// 接地状態
	if (onGround_) {
		// 移動入力
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 急ブレーキ
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				// 方向変換
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 1.0f;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
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
			velocity_.x += acceleration.x;
			velocity_.y += acceleration.y;
			velocity_.z += acceleration.z;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_.x += 0;
			velocity_.y += kJumpAcceleration;
			velocity_.z += 0;
		}
	} else {
		// 落下速度
		velocity_.x += 0;
		velocity_.y += -kGravityAcceleration;
		velocity_.z += 0;
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 着地フラグ
	bool landing = false;

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
}

Vector3 CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+Player::kWidth / 2.0f, -Player::kHeight / 2.0f, 0},
	    {-Player::kWidth / 2.0f, -Player::kHeight / 2.0f, 0},
	    {+Player::kWidth / 2.0f, +Player::kHeight / 2.0f, 0},
	    {-Player::kWidth / 2.0f, +Player::kHeight / 2.0f, 0}
    };

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::MapCollision(CollisionMapInfo& info) {
	MapCollisionTop(info);
	MapCollisionBottom(info);
	MapCollisionLeft(info);
	MapCollisionRight(info);

	CeilingCollision(info);
	WallCollision(info);
}

void Player::MapCollisionTop(CollisionMapInfo& info) {
	// 上昇あり?
	if (info.velocity.y <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, Corner::kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真上の当たり判定を行う
	bool hit = false;

	// 左上点の判定
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
		// めり込み先ブロックの矩形範囲
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - kHeight / 2 - kBlank);
		// info.velocity.y = std::max(0.0f, velocity_.y + kBlank);
		//  天井に当たったことを記録する
		info.isCollideCeiling = true;
	}
}
void Player::MapCollisionBottom(CollisionMapInfo& info) {
	// 下降あり？
	if (info.velocity.y >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, Corner::kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真下の当たり判定を行う
	bool hit = false;

	// 左下点の判定
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
		// めり込み先ブロックの矩形範囲
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::min(0.0f, (rect.top - worldTransform_.translation_.y) + kHeight / 2 + kBlank);
		// 地面に当たったことを記録する
		info.isLanded = true;
	}
}
void Player::MapCollisionLeft(CollisionMapInfo& info) {
	// 左移動あり？
	if (info.velocity.x >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, Corner::kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 左の当たり判定を行う
	bool hit = false;

	// 左上点の判定
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
		// めり込み先ブロックの矩形範囲
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.x = std::max(0.0f, (rect.right - worldTransform_.translation_.x) - kWidth / 2 + kBlank);
		// 地面に当たったことを記録する
		info.isContactWall = true;
	}
}
void Player::MapCollisionRight(CollisionMapInfo& info) {
	// 右移動あり？
	if (info.velocity.x <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, Corner::kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 右の当たり判定を行う
	bool hit = false;

	// 右上点の判定
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
		// めり込み先ブロックの矩形範囲
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.x = std::min(0.0f, (rect.left - worldTransform_.translation_.x) + kWidth / 2 - kBlank);
		// 地面に当たったことを記録する
		info.isContactWall = true;
	}
}

void Player::MoveByMapCollisionResult(CollisionMapInfo& info) {
	worldTransform_.translation_.x += info.velocity.x;
	worldTransform_.translation_.y += info.velocity.y;
	worldTransform_.translation_.z += info.velocity.z;
}

void Player::SwitchingOnGround(const CollisionMapInfo& info) {
	// 自キャラが接地状態？
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {

			// 移動後の4つの角の座標
			std::array<Vector3, Corner::kNumCorner> positionNew;

			for (uint32_t i = 0; i < positionNew.size(); ++i) {
				positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
			}

			MapChipType mapChipType;

			// 真下の当たり判定を行う
			bool hit = false;

			// 左下点の判定
			IndexSet indexSet;

			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector3(0, -kAdjustLanding, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector3(0, -kAdjustLanding, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// ブロックにヒット？
			if (!hit) {
				// 空中状態に切り替える
				onGround_ = false;
			}
		}
	} else {
		if (info.isLanded) {
			// 着地状態に切り替える
			onGround_ = true;
			// 着地時にｘ速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// 速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}

void Player::CeilingCollision(const CollisionMapInfo& info) {
	// 天井に当たった？
	if (info.isCollideCeiling) {
		velocity_.y = 0;
	}
}

void Player::WallCollision(const CollisionMapInfo& info) {
	// 壁接触による減速
	if (info.isContactWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

void Player::SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

const Vector3& Player::GetVelocity() const { return velocity_; }
