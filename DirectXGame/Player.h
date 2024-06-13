#pragma once
#define NOMINMAX
#include "Model.h"
#include "WorldTransform.h"
#include "mt.h"
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>

class MapChipField;

// マップとの当たり判定情報
struct CollisionMapInfo {
	bool isCollideCeiling = false;
	bool isLanded = false;
	bool isContactWall = false;

	Vector3 velocity;
};
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

	/// <summary>
	/// 動き
	/// </summary>
	void Move();

	/// <summary>
	/// 判定結果を反映して移動させる
	/// </summary>
	/// <param name="info"></param>
	void MoveByMapCollisionResult(CollisionMapInfo& info);

	/// <summary>
	/// 接地状態の切り替え
	/// </summary>
	/// <param name="info"></param>
	void SwitchingOnGround(const CollisionMapInfo& info);

	void CeilingCollision(const CollisionMapInfo& info);

	WorldTransform& GetWorldTransform();

	void SetMapChipField(MapChipField* mapChipField);

	const Vector3& GetVelocity() const;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

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
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 1.0f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 2.0f;

	//着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.1f;

	// 接地状態フラグ
	bool onGround_ = true;

	// マップチップフィールドによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//余白
	static inline const float kBlank = 0.1f;

	//
	static inline const float kAdjustLanding = 0.2f;

	/// <summary>
	/// マップ衝突判定
	/// </summary>
	/// <param name="info"></param>
	void MapCollision(CollisionMapInfo& info);

	/// <summary>
	/// マップ衝突判定上方向
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionTop(CollisionMapInfo& info);
	/// <summary>
	/// マップ衝突判定下方向
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionBottom(CollisionMapInfo& info);
	///// <summary>
	///// マップ衝突判定左方向
	///// </summary>
	///// <param name="info"></param>
	//void MapCollisionLeft(CollisionMapInfo& info);
	///// <summary>
	///// マップ衝突判定右方向
	///// </summary>
	///// <param name="info"></param>
	//void MapCollisionRight(CollisionMapInfo& info);
};