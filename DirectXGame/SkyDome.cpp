#include "SkyDome.h"

void SkyDome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();

	worldTransform_.Initialize(); }

void SkyDome::Update() {

}

void SkyDome::Draw() { model_->Draw(worldTransform_, *viewProjection_); }