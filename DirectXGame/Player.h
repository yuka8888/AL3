#pragma once
#define NOMINMAX
#include "Model.h"
#include "WorldTransform.h"
#include "mt.h"
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const Vector3& GetVelocity() const { return velocity_; }

	WorldTransform& GetWorldTransform();

	void SetMapChipField(MapChipField* mapChipField);

	const Vector3& GetVelocity() const { return velocity_; }


private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;
	
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
	Vector3 velocity_ = {};
	
	// 加速度
	static inline const float kAcceleration = 0.05f;
	
	// 速度減衰率
	static inline const float kAttenuation = 0.2f;
	
	// 最高速度
	static inline const float kLimitRunSpeed = 2.0f;
	
	// 左右
	enum class LRDirection {
		kRight,
		KLeft,
	};
	
	LRDirection lrDirection_ = LRDirection::kRight;
	
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	
	// 旋回時間(秒)
	static inline const float kTimerTurn = 0.3f;
	
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.2f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 2.0f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 2.0f;
	
	// 接地状態フラグ
	bool onGround_ = true;
};