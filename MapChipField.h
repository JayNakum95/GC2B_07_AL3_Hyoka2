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
	static inline const uint32_t kNumBlockHorizontal = 100; 

	MapChipData mapChipData_;

public:
	void LoadMapChipCsv(const std::string& filePath);
	uint32_t GetNumberBlockHorizontal() const { return kNumBlockHorizontal; }
	uint32_t GetNumberBlockVertical() const { return kNumBlockVerticle; }
	float GetBlockWidth() const { return kBlockWidth; }
	float GetBlockHeight() const { return kBlockHeight; }


	void ResetMapChipData();
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yindex);

};
