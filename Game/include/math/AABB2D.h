#pragma once

#include "MathUtils.h"
#include "IntersectionData.h"

class AABB2D {
  private:
	DirectX::XMFLOAT2 m_MinExtents;
	DirectX::XMFLOAT2 m_MaxExtents;
  public:
	AABB2D(const DirectX::XMFLOAT2 &minExtents, const DirectX::XMFLOAT2 &maxExtents)
	: m_MinExtents(minExtents), m_MaxExtents(maxExtents) {}

	IntersectionData IntersectAABB2D(const AABB2D &other) const;

	const DirectX::XMFLOAT2 &GetMinExtents() const { return m_MinExtents; }
	const DirectX::XMFLOAT2 &GetMaxExtents() const { return m_MaxExtents; }
};
