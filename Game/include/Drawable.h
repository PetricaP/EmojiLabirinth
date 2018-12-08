#pragma once

#include "Shader.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"

namespace d3d11 {

struct Drawable {
	const VertexBuffer *m_VertexBuffer;
	UINT m_VertexSize;

	const IndexBuffer *m_IndexBuffer;
	UINT m_NumIndices;

	Drawable();
	Drawable(UINT vertexSize, const VertexBuffer *vertexBuffer,
			const IndexBuffer *indexBuffer, UINT numIndices);
};

} // namespace d3d11
