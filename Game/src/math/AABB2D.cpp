#include "AABB2D.h"

IntersectionData AABB2D::IntersectAABB2D(const AABB2D &other) const {
	DirectX::XMFLOAT2 distances1 = other.GetMinExtents() - m_MaxExtents;
	DirectX::XMFLOAT2 distances2 = m_MinExtents - other.GetMaxExtents();
	DirectX::XMFLOAT2 distance(math::max(distances1.x, distances2.x),
							   math::max(distances1.y, distances2.y));
	float maxDistance = math::max(distance.x, distance.y);

	return IntersectionData(maxDistance < 0, maxDistance);
}
