#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	//定数バッファに転送する
	TransferMatrix();
}