#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void Player::Update() { worldTransform_.TransferMatrix(); }

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }