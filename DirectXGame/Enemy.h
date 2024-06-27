#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "mt.h"
#include <Input.h>
#include <cassert>
#include <numbers>

class Player;

class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突応答
	/// </summary>
	/// <param name="player"></param>
	void OnCollision(const Player* player);

	Vector3 GetWorldPosition();

	AABB GetAABB();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.9f;
	static inline const float kHeight = 1.9f;

	//歩行の速さ
	static inline const float kWalkSpeed_ = 0.05f;

	//速度
	Vector3 velocity_ = {};

	//最初の角度　度
	static inline const float kWalkMotionAngleStart_ = -30.0f;

	//最後の角度　度
	static inline const float kWalkMotionAngleEnd_ = 30.0f;

	//アニメーションの周期となる時間　秒
	static inline const float kWalkMotionTime_ = 1.0f;

	//経過時間
	float walkTimer_ = 0.0f;
};
