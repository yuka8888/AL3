#include "TitleText.h"

void TitleText::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {8, 8, 8};
	worldTransform_.translation_ = {-10, 0, 0};

}


void TitleText::Update() { 

	moveTimer_ += 2.0f / 60.0f;

	worldTransform_.translation_.y = std::sinf(moveTimer_);
	
	worldTransform_.UpdateMatrix(); }

void TitleText::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
