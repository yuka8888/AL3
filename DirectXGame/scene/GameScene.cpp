#include "GameScene.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete skyDome_;
	delete player_;

	delete modelBlock_;
	delete debugCamera_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// 3Dモデルの作成
	modelSkyDome_ = Model::CreateFromOBJ("skyDome", true);
	model_ = Model::Create();
	modelBlock_ = Model::Create();

	// 天球
	skyDome_ = new SkyDome;
	skyDome_->Initialize(modelSkyDome_, &viewProjection_);

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("sample.png");

	// プレイヤー
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &viewProjection_);

	// デバックカメラ
	debugCamera_ = new DebugCamera(1024, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	// ブロック一個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 要素数の変更
	worldTransformBlocks_.resize(kNumBlockVirtical);

	// 列数の設定
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j += 2) {
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}
}

void GameScene::Update() {
	skyDome_->Update();
	player_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif // _DEBUG

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// 行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// 行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->UpdateMatrix();
		}
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skyDome_->Draw();
	player_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
