#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_.x -= 2.0f;
	worldTransform_.translation_.y -= 2.0f;
}

void Player::Update() { worldTransform_.UpdateMatrix(); }

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }