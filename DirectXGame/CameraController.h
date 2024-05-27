#pragma once
#include <ViewProjection.h>
class Player;

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

	void SetTarget(Player* target) { target_ = target; }

private:
	ViewProjection* viewProjection = nullptr;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -15.0f};
};
