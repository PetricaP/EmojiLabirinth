#include "Utility.h"
#include "AABB2D.h"

IntersectionData AABB2D::IntersectAABB2D(const AABB2D &other) const {
	math::vec2f distances1 = (other.m_Translation + other.GetMinExtents()) - (m_Translation + m_MaxExtents);
	math::vec2f distances2 = (m_Translation + m_MinExtents) - (other.m_Translation + other.GetMaxExtents());
	math::vec2f distance(math::max(distances1.x, distances2.x),
					   math::max(distances1.y, distances2.y));

	float maxDistance = math::max(distance.x, distance.y);

	return IntersectionData(maxDistance < 0, distance);
}

void AABB2D::Test() {
	{
		AABB2D aabb1({-1.0f, -1.0f}, {1.0f, 1.0f});
		AABB2D aabb2({-0.5f, -0.5f}, {1.5f, 1.5f});
		auto intersectData = aabb1.IntersectAABB2D(aabb2);
		ASSERT(intersectData.intersects == true);
		ASSERT(intersectData.distance.x == -1.5f);
		ASSERT(intersectData.distance.y == -1.5f);
	}
	{
		AABB2D aabb1({-1.0f, -1.0f}, {0.0f, 0.0f});
		AABB2D aabb2({-2.0f, -1.1f}, {2.0f, -0.9f});
		auto intersectData = aabb1.IntersectAABB2D(aabb2);
		ASSERT(intersectData.intersects == true);
	}
}
