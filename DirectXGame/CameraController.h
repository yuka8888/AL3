#pragma once
#define NOMINMAX
#include <ViewProjection.h>
class Player;

struct Rect {
	float left = 0.0f;
	float right = 1.0f;
	float bottom = 0.0f;
	float top = 1.0f;
};

class CameraController {
public:
	/// <summary>
	///	初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Reset();

	void SetTarget(Player* target);

	ViewProjection& GetViewProjection() { return viewProjection_; }

	void SetMoveableArea(Rect area) { movableArea_ = area; }

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -25.0f};

	Rect movableArea_ = {0, 100, 0, 100};

	// カメラの目標座標
	Vector3 targetCoordinates_ = {};
	
	// 座標補間割合
	static inline const float kInterpolationRate_ = 0.5f;

	//速度掛け算
	static inline const float kVelocityBias = 3.0f;

	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect margin = {-10, 10, -30, 5};
};
