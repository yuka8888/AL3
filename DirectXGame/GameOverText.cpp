#include "GameOverText.h"
void GameOverText::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {8, 8, 8};
	worldTransform_.translation_ = {-20, 0, 0};
}

void GameOverText::Update() {

	moveTimer_ += 3.0f / 60.0f;

	worldTransform_.translation_.y = 2.0f / std::sinf(moveTimer_);

	worldTransform_.UpdateMatrix();
}

void GameOverText::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
