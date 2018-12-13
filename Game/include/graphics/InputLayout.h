#pragma once

#include "Utility.h"
#include <d3d11.h>
#include <vector>
#include <string>

namespace d3d11 {

struct InputElement {
	std::string name;
    DXGI_FORMAT format;
    UINT inputSlot;

	InputElement(const std::string &name, DXGI_FORMAT format,
				 UINT inputSlot = 0)
		: name(name), format(format), inputSlot(inputSlot) {}
};

class InputLayout {
	unique_handle<ID3D11InputLayout> m_Handle;
	friend class Renderer;
};

}
