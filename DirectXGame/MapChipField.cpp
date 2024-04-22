#include "MapChipField.h"
#include <map>

namespace {
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlock},
    {"1", MapChipType::kBlock},
};
}

void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
	
	mapChipDataLine.resize(kNumBlockHorizontal);
	}
}