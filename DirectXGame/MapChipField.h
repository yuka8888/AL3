#pragma once
#include "DirectXCommon.h"

enum class MapChipType {
	kBlock,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:
	/// <summary>
	/// 読み込み済みのデータ削除
	/// </summary>
	void ResetMapChipData();

private:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
};
