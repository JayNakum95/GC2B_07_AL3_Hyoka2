#include "MapChipField.h"
#include <fstream>
#include <map>
#include <sstream>

namespace {
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};
}

void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVerticle);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	ResetMapChipData();
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());
	std::stringstream mapChipCvs;
	mapChipCvs << file.rdbuf();
	file.close();
	for (uint32_t i = 0; i < kNumBlockVerticle; ++i) {
		std::string line;
		getline(mapChipCvs, line);
		std::stringstream line_stream(line);
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');
			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex >= kNumBlockHorizontal) {
		return MapChipType::kBlank;
	}
	if (yIndex >= kNumBlockVerticle) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

KamataEngine::Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {
	return KamataEngine::Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVerticle - 1 - yIndex), 0);
}

MapChipField::IndexSet MapChipField::GetMapChipIndexByPosition(const KamataEngine::Vector3& position) {
	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2) / kBlockWidth);
indexSet.yIndex = static_cast<uint32_t>((kNumBlockVerticle - 1) - ((position.y + kBlockHeight / 2) / kBlockHeight));

	return indexSet; //
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	KamataEngine::Vector3 centre = GetMapChipPositionByIndex(xIndex, yIndex);
	Rect rect;
	rect.left = centre.x - kBlockWidth / 2.0f;
	rect.right = centre.x + kBlockWidth / 2.0f;
	rect.top = centre.y + kBlockHeight / 2.0f;
	rect.bottom = centre.y - kBlockHeight / 2.0f;
	return rect;
}
