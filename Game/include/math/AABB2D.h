#pragma once

#include "MathUtils.h"
#include "IntersectionData.h"
#include "Transform2D.h"

class AABB2D {
  private:
	math::vec2f m_MinExtents;
	math::vec2f m_MaxExtents;
	math::vec2f m_Translation;
  public:
	AABB2D(const math::vec2f &minExtents, const math::vec2f &maxExtents)
	: m_MinExtents(minExtents), m_MaxExtents(maxExtents), m_Translation({0.0f, 0.0f}) {}

	IntersectionData IntersectAABB2D(const AABB2D &other) const;

	void SetTranslation(const math::vec2f &translation) { m_Translation = translation; }

	const math::vec2f &GetMinExtents() const { return m_MinExtents; }
	const math::vec2f &GetMaxExtents() const { return m_MaxExtents; }
	const math::vec2f &GetTranslation() const { return m_Translation; }

	static void Test();
};
