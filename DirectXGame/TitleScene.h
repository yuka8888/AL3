#pragma once
#include "Audio.h"
#include "AxisIndicator.h"
#include <ViewProjection.h>
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Sprite.h"
#include "TitleText.h"
#include "Fade.h"


class TitleScene {
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

	~TitleScene();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//デスフラグのセッター
	bool IsFinished() const;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Model* modelText = nullptr;
	TitleText* titleText = nullptr;

	// ビュープロジェクション
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//終了フラグ
	bool finished_ = false;

	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;
};
