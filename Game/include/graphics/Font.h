#pragma once

#include "Utility.h"
#include <FW1FontWrapper.h>

namespace d3d11 {

class Font {
	static float m_ScreenX;
	static float m_ScreenY;

	unique_handle<IFW1FontWrapper> m_Handle;
	std::wstring m_Name;

  public:
	
	static void Init(uint32_t screenX, uint32_t screenY);
	friend class Renderer;

	enum Color { 
		BLACK = 0xff000000,
		GRAY = 0xff555555,
		BLUE = 0xff0000ff,
		LIGHT_BLUE = 0xffdd5555,
		RED = 0xff3322cc,
		GREEN = 0xff00ff00,
		ORANGE = 0xff5599ff,
		YELLOW = 0xff55ffff
	};
};

}
