#pragma once

#include "MathUtils.h"

struct IntersectionData {
	const math::vec2f distance;
	const bool intersects;
	IntersectionData(bool intersects, math::vec2f distance)
		: intersects(intersects), distance(distance) {}
};
