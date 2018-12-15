#pragma once

#include "MathUtils.h"
#include "IntersectionData.h"
#include "Transform2D.h"

class AABB2D {
  private:
	xm::vec2f m_MinExtents;
	xm::vec2f m_MaxExtents;
	xm::vec2f m_Translation;
  public:
	AABB2D(const xm::vec2f &minExtents, const xm::vec2f &maxExtents)
	: m_MinExtents(minExtents), m_MaxExtents(maxExtents), m_Translation({0.0f, 0.0f}) {}

	IntersectionData IntersectAABB2D(const AABB2D &other) const;

	void SetTranslation(const xm::vec2f &translation) { m_Translation = translation; }

	const DirectX::XMFLOAT2 &GetMinExtents() const { return m_MinExtents; }
	const DirectX::XMFLOAT2 &GetMaxExtents() const { return m_MaxExtents; }
};
