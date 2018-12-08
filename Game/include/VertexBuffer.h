#pragma once

#include "Utility.h"
#include <d3d11.h>

namespace d3d11 {

class VertexBuffer {
	unique_handle<ID3D11Buffer> m_Handle;

	friend class Renderer;
};

}