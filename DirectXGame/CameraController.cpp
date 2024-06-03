#include "CameraController.h"
#include "Player.h"

void CameraController::Initialize() { viewProjection_.Initialize(); }

void CameraController::Update() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// 追従対象とオフセットからカメラの座標を計算
	targetCoordinates_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	targetCoordinates_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	targetCoordinates_.z = targetWorldTransform.translation_.z + targetOffset_.z;

	//座標補間によりゆったり実装
	viewProjection_.translation_.x = Lerp(viewProjection_.translation_.x, targetCoordinates_.x, kInterpolationRate_);
	viewProjection_.translation_.y = Lerp(viewProjection_.translation_.y, targetCoordinates_.y, kInterpolationRate_);
	viewProjection_.translation_.z = Lerp(viewProjection_.translation_.z, targetCoordinates_.z, kInterpolationRate_);

	//移動範囲宣言
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, movableArea_.top);

	// 行列を更新する
	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// 追従対象とオフセットからカメラの座標を計算
	viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}

void CameraController::SetTarget(Player* target) { target_ = target; }
