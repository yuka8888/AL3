#pragma once
#include "Audio.h"
#include "AxisIndicator.h"
#include "CameraController.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "Player.h"
#include "SkyDome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "mt.h"
#include <cassert>
#include <vector>

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
	
	// マップチップからブロックを生成
	void GenerateBlocks();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	
	// ビュープロジェクション
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	
	// 天球
	SkyDome* skyDome_ = nullptr;
	
	// モデル
	Model* modelSkyDome_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	
	// プレイヤー生成
	Player* player_ = nullptr;
	
	// 可変個配列
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	MapChipField* mapChipField_;

	// カメラコントローラ
	CameraController* cameraController_ = nullptr;

	CameraController::Rect movableArea_ = {};

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};