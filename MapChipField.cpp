#include "MapChipField.h"
#include <map>
#include<fstream>
#include<sstream>

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

void MapChipField::loadMapChipCsv(const std::string& filePath) { 
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
			if (mapChipTable.contains(word)){
				mapChipData_.data[i][j] = mapChipTable[word];
			}		
		}	
	}
}

