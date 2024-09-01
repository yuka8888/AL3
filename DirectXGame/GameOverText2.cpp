#include "GameOverText2.h"
void GameOverText2::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {8, 8, 8};
	worldTransform_.translation_ = {0, 0, 0};
}

void GameOverText2::Update() {

	moveTimer_ += 3.0f / 60.0f;

	worldTransform_.translation_.y = 2.0f / std::sinf(moveTimer_);

	worldTransform_.UpdateMatrix();
}

void GameOverText2::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
