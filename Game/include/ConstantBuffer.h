#pragma once

#include "Utility.h"
#include <d3d11.h>

namespace d3d11 {

class ConstantBuffer {
	unique_handle<ID3D11Buffer> m_Handle;
	UINT m_Size;

	friend class Renderer;
};

}
