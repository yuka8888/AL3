#include "Goal.h"

void Goal::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {1, 1, 1};
	worldTransform_.translation_ = {30, 0, 0};
}

void Goal::Update() {
	worldTransform_.UpdateMatrix();
}

void Goal::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
