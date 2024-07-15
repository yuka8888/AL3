#include "Fade.h"

void Fade::Initialize() {
	directXCommon = DirectXCommon::GetInstance();

	sprite_ = new Sprite;
	textureHandle_ = TextureManager::Load("fade.png");
	sprite_ = Sprite::Create(textureHandle_, {0, 0});
	sprite_->SetSize({kWindowWidth, kWindowHeight});
	sprite_->SetColor({0, 0, 0, 1});
}

void Fade::Update() {
	switch (status_) {
	case Status::None:
		//何もしない
		break;
	case Status::FadeIn:
		break;
	case Status::FadeOut:
		//1フレーム文の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;

		//フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		break;
	}
}

void Fade::Draw(ID3D12GraphicsCommandList* commandList) {
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	Sprite::PostDraw();
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}
