#pragma once
#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "Fade.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "SkyDome.h"
#include "Sprite.h"
#include <ViewProjection.h>
#include "ClearText.h"

class ClearScene {
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	~ClearScene();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// デスフラグのセッター
	bool IsFinished() const;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Model* modelSkyDome_ = nullptr; 
	SkyDome* skyDome_ = nullptr;

	Model* modelText_ = nullptr;

	ClearText* clearText_ = nullptr;

	// ビュープロジェクション
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 終了フラグ
	bool finished_ = false;

	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;
};
