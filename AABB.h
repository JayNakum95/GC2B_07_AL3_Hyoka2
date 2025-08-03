#pragma once  
#include <KamataEngine.h>  
using namespace KamataEngine;  

struct AABB {
	Vector3 min; // 最小座標
	Vector3 max; // 最大座標
	
};
inline bool isCollisionAABB(const AABB& a, const AABB& b) {
	return (a.min.x <= b.max.x && a.max.x >= b.min.x) && (a.min.y <= b.max.y && a.max.y >= b.min.y) && (a.min.z <= b.max.z && a.max.z >= b.min.z);
}
