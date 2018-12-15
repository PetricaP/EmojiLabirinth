#pragma once

#include <DirectXMath.h>
#include <cmath>
#include <cstdlib>

namespace math {

constexpr double PI = 3.14159265359;

inline float lerp(float current, float goal, float dt) {
	float dif = goal - current;
	if (dif > dt) {
		return current + dt;
	} else if (dif < -dt) {
		return current - dt;
	}
	return goal;
}

inline float clamp(float x, float min, float max) {
	if (x > max) {
		return max;
	}
	if (x < min) {
		return min;
	}
	return x;
}

inline float map(float x, float x0, float x1, float y0, float y1) {
	x = (x - x0) * (y1 - y0) / (x1 - x0) + y0;
	return x;
}

inline float toRadians(float angle) {
	return static_cast<float>(PI) * angle / 180.0f;
}

#undef max

template <typename T>
inline T max(T first, T second) {
	return ((first > second) ? first : second);
}

#undef min

template <typename T>
inline T min(T first, T second) {
	return ((first < second) ? first : second);
}

template <typename T>
struct rect_t {
	T x, y;
	T width, height;
};

using rect = rect_t<float>;

inline float randf(float min, float max) {
	return min + (rand() % RAND_MAX) * (max - min) / RAND_MAX;
}

} // namespace math

namespace DirectX {

inline XMFLOAT3 operator+(const XMFLOAT3 &v1, const XMFLOAT3 &v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline XMFLOAT3 operator-(const XMFLOAT3 &v1, const XMFLOAT3 &v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline XMFLOAT3 operator+(const XMFLOAT3 &v1, float s) {
	return {v1.x + s, v1.y + s, v1.z + s};
}

inline XMFLOAT3 operator+(float s, const XMFLOAT3 &v1) {
	return {v1.x + s, v1.y + s, v1.z + s};
}

inline XMFLOAT3 operator*(const XMFLOAT3 &v1, float s) {
	return {v1.x * s, v1.y * s, v1.z * s};
}

inline XMFLOAT3 operator*(float s, const XMFLOAT3 &v1) {
	return {v1.x * s, v1.y * s, v1.z * s};
}

inline XMFLOAT2 operator+(const XMFLOAT2 &v1, const XMFLOAT2 &v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

inline XMFLOAT2 operator-(const XMFLOAT2 &v1, const XMFLOAT2 &v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

inline XMFLOAT2 operator+(const XMFLOAT2 &v1, float s) {
	return {v1.x + s, v1.y + s};
}

inline XMFLOAT2 operator+(float s, const XMFLOAT2 &v1) {
	return {v1.x + s, v1.y + s};
}

inline XMFLOAT2 operator-(const XMFLOAT2 &v1, float s) {
	return {v1.x - s, v1.y - s};
}

inline XMFLOAT2 operator-(float s, const XMFLOAT2 &v1) {
	return {v1.x - s, v1.y - s};
}

inline XMFLOAT2 operator*(const XMFLOAT2 &v1, float s) {
	return {v1.x * s, v1.y * s};
}

inline XMFLOAT2 operator*(float s, const XMFLOAT2 &v1) {
	return {v1.x * s, v1.y * s};
}

} // namespace DirectX

namespace xm {
using vec2f = DirectX::XMFLOAT2;
using vec3f = DirectX::XMFLOAT3;
} // namespace xm
