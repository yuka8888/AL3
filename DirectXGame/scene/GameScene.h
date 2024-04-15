#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"
#include "DebugCamera.h"
#include "AxisIndicator.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr; 

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//音声再生ハンドル
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;

	// スプライト
	Sprite* sprite_ = nullptr;
	Model* model_ = nullptr;

	//ワールドトランスフォームの初期化
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	float imputFloat3[3] = {0, 0, 0};


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
