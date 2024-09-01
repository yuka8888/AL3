#include "GameScene.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete skyDome_;
	if (player_) {
		delete player_;
		delete model_;
	}

	// エネミー
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	delete modelBlock_;
	delete modelSkyDome_;
	delete modelEnemy_;
	delete modelDeathParticle_;

	delete fade_;

	delete debugCamera_;

	delete mapChipField_;

	delete deathParticles_;

	delete goal_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {
	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

	finished_ = false;
	isGoal_ = false;
	isGameOver_ = false;

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// 3Dモデルの作成
	modelSkyDome_ = Model::CreateFromOBJ("skyDome", true);
	model_ = Model::CreateFromOBJ("Player", true);
	modelEnemy_ = Model::CreateFromOBJ("Player", true);
	modelBlock_ = Model::Create();
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle", true);
	modelGoal_ = Model::CreateFromOBJ("Goal", true);

	// 天球
	skyDome_ = new SkyDome;
	skyDome_->Initialize(modelSkyDome_, &viewProjection_);

	// デバックカメラ
	debugCamera_ = new DebugCamera(1024, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	// マップチップ
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	// プレイヤーの座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	// プレイヤー
	player_ = new Player();
	player_->Initialize(model_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	// エネミー
	for (int32_t i = 0; i < enemyNum; ++i) {
		// エネミーの座標をマップチップ番号で指定
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(12 + i * 3, 18);
		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	// ゴール
	goal_ = new Goal();
	goal_->Initialize(modelGoal_, &viewProjection_);

	// カメラコントローラの初期化
	movableArea_ = {17, 17, 9, 50};

	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->SetMoveableArea(movableArea_);
	cameraController_->Reset();

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameScene::Update() {
	switch (phase_) {
	case Phase::kPlay:
		skyDome_->Update();
		player_->Update();
		goal_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

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
		cameraController_->Update();
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->UpdateMatrix();
			}
		}

		// クリックした場所にブロックを設置
		NewGenerateBlock();

		CheckAllCollisions();

		break;

	case Phase::kDeath:
		skyDome_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		if (deathParticles_) {
			deathParticles_->Update();
		}

		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->UpdateMatrix();
			}
		}

		break;
	}
	fade_->Update();
	ChangePhase();
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
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (player_) {
		player_->Draw();
	}

	if (deathParticles_) {
		deathParticles_->Draw();
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	goal_->Draw();

	fade_->Draw(commandList);

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

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizonal = mapChipField_->GetNumBlockHorizontal();

	// 要素数の変更
	worldTransformBlocks_.resize(numBlockVirtical);

	// 列数の設定
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizonal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		for (uint32_t j = 0; j < numBlockHorizonal; j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::NewGenerateBlock() {
	// クリックしたマップチップ番号を取得
	if (mapChipField_->ClickPositionIsBlock().x >= 0 && mapChipField_->ClickPositionIsBlock().y >= 0) {
		// ブロックの生成
		WorldTransform* worldTransform = new WorldTransform();
		worldTransform->Initialize();
		worldTransformBlocks_[(uint32_t)mapChipField_->ClickPositionIsBlock().y][(uint32_t)mapChipField_->ClickPositionIsBlock().x] = worldTransform;
		worldTransformBlocks_[(uint32_t)mapChipField_->ClickPositionIsBlock().y][(uint32_t)mapChipField_->ClickPositionIsBlock().x]->translation_ =
		    mapChipField_->GetMapChipPositionByIndex((uint32_t)mapChipField_->ClickPositionIsBlock().x, (uint32_t)mapChipField_->ClickPositionIsBlock().y);
	}
}

void GameScene::CheckAllCollisions() {
#pragma region 自キャラと敵キャラの当たり判定

	// 判定対象１と２の座標
	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾すべての当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (isCollision(aabb1, aabb2)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);
			// 敵キャラの衝突時コールバックを呼び出す
			enemy->OnCollision(player_);
		}
	}
#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case GameScene::Phase::kPlay:
		if (player_->isDead() || player_->GetWorldTransform().translation_.y <= 0.0f) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			isGameOver_ = true;

			// 自キャラの座標を取得
			const Vector3& deathParticlePosition = player_->GetWorldPosition();

			// 死んだときのパーティクル
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(modelDeathParticle_, &viewProjection_, deathParticlePosition);

			// 自キャラを削除
			if (player_) {
				delete player_;
				player_ = nullptr;
				delete model_;
				model_ = nullptr;
			}
		}
		if (player_) {
			if (player_->IsGoal()) {
				phase_ = Phase::kDeath;
				isGoal_ = true;
				fade_->Start(Fade::Status::FadeOut, 1.0f);
			}
		}
		break;

	case GameScene::Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			// パーティクルを削除
			delete deathParticles_;
			deathParticles_ = nullptr;
			delete modelDeathParticle_;
			modelDeathParticle_ = nullptr;

			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}

		if (fade_->GetStatus() == Fade::Status::FadeOut && fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

bool GameScene::IsFinished() const { return finished_; }

bool GameScene::IsGoal() { return isGoal_; }

bool GameScene::IsGameOver() { return isGameOver_; }
