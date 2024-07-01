#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <array>
#include <algorithm>


class DeathParticles {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	/// <param name="position"></param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	//存続時間
	static inline const float kDuration = 2.0f;

	//移動の速さ
	static inline const float kSpeed = 0.1f;

	//分割した一個分の角度
	static inline const float kAngleUnit = 2.0f * 3.14f / kNumParticles;

	//終了フラグ
	bool isFinished_ = false;

	//経過時間カウント
	float counter_ = 0.0f;
	
	//色変更オブジェクト
	ObjectColor objectColor_;

	//色の数値
	Vector4 color_;

	std::array<WorldTransform, kNumParticles> worldTransforms_;
};
