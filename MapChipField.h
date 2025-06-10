#pragma once  
#include <KamataEngine.h>
#include <cstdint>
#include <vector>

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};


class MapChipField {  

	static inline const float kBlockWidth = 1.0f;  
	static inline const float kBlockHeight = 1.0f;  

	static inline const uint32_t kNumBlockVerticle = 20;  
	static inline const uint32_t kNumBlockHorizontal = 30; 

	MapChipData mapChipData_;

private:
	void ResetMapChipData();
	void loadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

};
