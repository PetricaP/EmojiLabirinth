#pragma once

struct IntersectionData {
	const float distance;
	const bool intersects;
	IntersectionData(bool intersects, float distance)
		: intersects(intersects), distance(distance) {}
};
