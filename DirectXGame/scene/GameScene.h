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
#include <cassert>
#include <vector>
#include "mt.h"

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
	bool isDebugCameraActive_ = false;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// モデルデータ
	Model* modelBlock_ = nullptr;

	//ワールドトランスフォームの初期化
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//可変個配列
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
