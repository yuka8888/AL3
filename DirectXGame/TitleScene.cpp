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
	modelText = Model::CreateFromOBJ("TitleText", true);
	titleText->Initialize(modelText, &viewProjection_);

	fade_ = new Fade();
	fade_->Initialize();
}

TitleScene::~TitleScene() {
	delete fade_;
	delete titleText;
}

void TitleScene::Update() { 
	titleText->Update();
	fade_->Update();

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
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
	//fade_->Draw(commandList);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

}

bool TitleScene::IsFinished() const { return finished_; }
