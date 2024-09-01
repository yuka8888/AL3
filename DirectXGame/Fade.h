#pragma once
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "algorithm"

class Fade {
public:
	// フェードの状態
	enum class Status {
		None,    // フェードなし
		FadeIn,  // フェードイン中
		FadeOut, // フェードアウト中
	};

	void Initialize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* commandList);

	void Start(Status status, float duration);

	void Stop();

	bool IsFinished() const;

	Status GetStatus();

	bool IsStart();

private:
	Sprite* sprite_ = nullptr;

	DirectXCommon* directXCommon = nullptr;

	Status status_ = Status::None;

	// フェードの持続時間
	float duration_ = 0.0f;

	// 経過時間カウンター
	float counter_ = 0.0f;

	uint32_t textureHandle_ = 0;

	float kWindowHeight = 720.0f;
	float kWindowWidth = 1280.0f;
};
