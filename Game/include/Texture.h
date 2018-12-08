#pragma once

#include "Utility.h"
#include <d3d11.h>

namespace d3d11 {

class Texture {
	unique_handle<ID3D11ShaderResourceView> m_Handle;
	unique_handle<ID3D11SamplerState> m_SampleState;

public:
	friend class Renderer;
};

}
