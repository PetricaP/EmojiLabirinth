#include "Font.h"

namespace d3d11 {

float Font::m_ScreenX = 0;
float Font::m_ScreenY = 0;

void Font::Init(uint32_t screenX, uint32_t screenY) {
	m_ScreenX = static_cast<float>(screenX);
	m_ScreenY = static_cast<float>(screenY);
}

} // namespace d3d11
