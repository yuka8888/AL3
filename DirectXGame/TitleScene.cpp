#include "TitleScene.h"

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	finished_ = false;

	// ビュープロジェクションの初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	//テキスト
	titleText = new TitleText;
	modelText_ = Model::CreateFromOBJ("TitleText", true);
	titleText->Initialize(modelText_, &viewProjection_);
	
	titleText2 = new TitleText2;
	modelText2_ = Model::CreateFromOBJ("TitleText2", true);
	titleText2->Initialize(modelText2_, &viewProjection_);

	skyDome_ = new SkyDome;
	modelSkyDome_ = Model::CreateFromOBJ("skyDome", true);
	skyDome_->Initialize(modelSkyDome_, &viewProjection_);

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

TitleScene::~TitleScene() {
	delete fade_;
	delete titleText;
	delete titleText2;
	delete skyDome_;
}

void TitleScene::Update() { 
	titleText->Update();
	titleText2->Update();
	skyDome_->Update();
	fade_->Update();

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		fade_->Start(Fade::Status::FadeOut, 1.0f);
	}

	if ((fade_->GetStatus() == Fade::Status::FadeOut) && (fade_->IsFinished() == true)) {	
		finished_ = true;
	}
	viewProjection_.UpdateMatrix(); }

void TitleScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	titleText->Draw();
	titleText2->Draw();
	skyDome_->Draw();
	
	fade_->Draw(commandList);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

}

bool TitleScene::IsFinished() const { return finished_; }
