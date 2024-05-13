#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"
#include "AxisIndicator.h"
#include "DebugCamera.h"
#include "Player.h"
#include "SkyDome.h"
#include <vector>
#include <cassert>
#include "mt.h"
#include "MapChipField.h"

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

	//マップチップからブロックを生成
	void GenerateBlocks();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	//ビュープロジェクション
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//天球
	SkyDome* skyDome_ = nullptr;

	//モデル
	Model* modelSkyDome_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0;

	// プレイヤー生成
	Player* player_ = nullptr;

		// 可変個配列
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	MapChipField* mapChipField_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
