#include "ClearScene.h"

void ClearScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	finished_ = false;

	// ビュープロジェクションの初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	clearText_ = new ClearText;
	modelText_ = Model::CreateFromOBJ("ClearText", true);
	clearText_->Initialize(modelText_, &viewProjection_);

	skyDome_ = new SkyDome;
	modelSkyDome_ = Model::CreateFromOBJ("skyDome", true);
	skyDome_->Initialize(modelSkyDome_, &viewProjection_);

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

ClearScene::~ClearScene() {
	delete fade_;
	delete skyDome_;
	delete modelSkyDome_;
	delete modelText_;
	delete clearText_;
}

void ClearScene::Update() {
	skyDome_->Update();
	clearText_->Update();
	fade_->Update();

	if (!fade_->IsStart() && Input::GetInstance()->PushKey(DIK_SPACE)) {
		fade_->Start(Fade::Status::FadeOut, 1.0f);
	}

	if ((fade_->GetStatus() == Fade::Status::FadeOut) && (fade_->IsFinished() == true)) {
		finished_ = true;
	}
	viewProjection_.UpdateMatrix();
}

void ClearScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skyDome_->Draw();
	clearText_->Draw();

	fade_->Draw(commandList);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion
}

bool ClearScene::IsFinished() const { return finished_; }
