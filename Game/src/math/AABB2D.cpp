#include "AABB2D.h"

IntersectionData AABB2D::IntersectAABB2D(const AABB2D &other) const {
	xm::vec2f distances1 = (other.m_Translation + other.GetMinExtents()) - (m_Translation + m_MaxExtents);
	xm::vec2f distances2 = (m_Translation + m_MinExtents) - (other.m_Translation + other.GetMaxExtents());
	xm::vec2f distance(math::max(distances1.x, distances2.x),
					   math::max(distances1.y, distances2.y));

	float maxDistance = math::max(distance.x, distance.y);

	return IntersectionData(maxDistance < 0, maxDistance);
}
