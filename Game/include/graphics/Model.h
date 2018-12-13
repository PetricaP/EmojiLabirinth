#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace d3d11 {

struct Model {
	d3d11::IndexBuffer indexBuffer;
	d3d11::VertexBuffer vertexBuffer;
};

} // namespace d3d11
