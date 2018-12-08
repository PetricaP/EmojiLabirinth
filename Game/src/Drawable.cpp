#include "Drawable.h"

namespace d3d11 {

Drawable::Drawable() : m_VertexSize(0), m_NumIndices(0) {}

Drawable::Drawable(UINT vertexSize, const VertexBuffer *vertexBuffer,
				   const IndexBuffer *indexBuffer, UINT numIndices)
	: m_VertexSize(vertexSize), m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer),
	  m_NumIndices(numIndices) {}

}