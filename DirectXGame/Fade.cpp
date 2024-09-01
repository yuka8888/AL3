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
		// 何もしない
		break;
	case Status::FadeIn:
		// 1フレーム文の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;

		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ地を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp((duration_ - counter_) / duration_, 0.0f, 1.0f)));

		break;
	case Status::FadeOut:
		// 1フレーム文の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;

		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ地を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));

		break;
	}
}

void Fade::Draw(ID3D12GraphicsCommandList* commandList) {
	if (status_ == Status::None) {
		return;
	}
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	Sprite::PostDraw();
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop() { status_ = Status::None; }

bool Fade::IsFinished() const {
	switch (status_) {
	case Fade::Status::FadeIn:
	case Fade::Status::FadeOut:
		if (counter_ >= duration_) {
			return true;
		} else {
			return false;
		}
	}

	return true;
}

Fade::Status Fade::GetStatus() { return status_; }

bool Fade::IsStart() {
	if (counter_ < duration_) {
		return true;
	} else {
		return false;
	}
}
