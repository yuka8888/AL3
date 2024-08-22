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
#include "Enemy.h"
#include "SkyDome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "mt.h"
#include <cassert>
#include <vector>
#include "DeathParticles.h"
#include "Fade.h"

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

	// マップチップから新しいブロックを生成
	void NewGenerateBlock();

	//すべての当たり判定を行う
	void CheckAllCollisions();

	/// <summary>
	/// フェースの切り替え処理
	/// </summary>
	void ChangePhase();

	// デスフラグのセッター
	bool IsFinished() const;


private: // メンバ変数
	//ゲームのフェーズ
	enum class Phase {
		kPlay, //ゲームプレイ
		kDeath //デス演出
	};

	//ゲームの現在フェーズ
	Phase phase_;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Fade* fade_ = nullptr;
	
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
	Model* modelEnemy_ = nullptr;
	Model* modelDeathParticle_ = nullptr;
	
	// プレイヤー生成
	Player* player_ = nullptr;

	//デスフラグ
	bool isDead_ = false;

	//エネミー
	std::list<Enemy*> enemies_;
	int32_t enemyNum = 3;

	//死んだときのパーティクル
	DeathParticles* deathParticles_ = nullptr;
	
	// 可変個配列
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	MapChipField* mapChipField_;

	// カメラコントローラ
	CameraController* cameraController_ = nullptr;

	CameraController::Rect movableArea_ = {};

	// 終了フラグ
	bool finished_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};