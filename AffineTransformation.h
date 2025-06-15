#pragma once
#include "Math/MathUtility.h"
using namespace KamataEngine;

// Helper function to multiply two Matrix4x4
inline Matrix4x4 MultiplyMatrix(const Matrix4x4& a, const Matrix4x4& b) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}

inline Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
    Matrix4x4 sm = MathUtility::MakeScaleMatrix(scale);
    Matrix4x4 rx = MathUtility::MakeRotateXMatrix(rotate.x);
    Matrix4x4 ry = MathUtility::MakeRotateYMatrix(rotate.y);
    Matrix4x4 rz = MathUtility::MakeRotateZMatrix(rotate.z);

    Matrix4x4 rm = MultiplyMatrix(MultiplyMatrix(rz, ry), rx);
    Matrix4x4 tm = MathUtility::MakeTranslateMatrix(translate);
    return MultiplyMatrix(MultiplyMatrix(sm, rm), tm);
}
inline Vector3 operator+ (const Vector3& v1, Vector3& v2) { 
    return Vector3{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }