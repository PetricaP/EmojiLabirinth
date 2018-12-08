#pragma once

#include "Utility.h"
#include <FW1FontWrapper.h>

namespace d3d11 {

class Font {
	enum Color { BLUE = 0xff0000ff, RED = 0xffff0000, GREEN = 0xff00ff00 };
	static float m_ScreenX;
	static float m_ScreenY;

	d3d11::unique_handle<IFW1FontWrapper> m_Handle;
	std::wstring m_Name;

  public:
	
	static void Init(uint32_t screenX, uint32_t screenY);
	friend class Renderer;
};

}
