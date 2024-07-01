#include "DeathParticles.h"

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	model_ = model;
	viewProjection_ = viewProjection;

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

	//ワールド変換の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update() {
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	for (uint32_t i = 0; i < kNumParticles; ++i) {
	//基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};

		//回転角を計算する
		float angle = kAngleUnit * i;

		//Z軸周り回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);

		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);

		//移動処理
		worldTransforms_[i].translation_ = worldTransforms_[i].translation_ + velocity;
	}

	//カウンターを１フレーム文の秒数進める
	counter_ += 1.0f / 60.0f;

	//存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;

		//終了扱いにする
		isFinished_ = true;
	}

	color_.w -= 0.01f;

	color_.w = std::clamp(color_.w, 0.0f, 1.0f);
	//色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
	//色変更オブジェクトをVRAMに転換
	objectColor_.TransferMatrix();
	
	//ワールド変換の更新
	for (auto& worldTransform : worldTransforms_) {
	worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	//終了なら何もしない
	if (isFinished_) {
		return;
	}

	//モデルの描画
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}
}
