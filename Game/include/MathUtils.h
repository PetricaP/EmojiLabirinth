#pragma once

#include <cmath>

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

template <typename T>
struct rect_t {
	T x, y;
	T width, height;
};

using rect = rect_t<float>;

} // namespace math

