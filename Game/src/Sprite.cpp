#include "Sprite.h"

Sprite::Sprite(const d3d11::Renderer &renderer, const d3d11::Texture &texture,
			   const math::rect *rect) : m_Texture(texture) {
	m_Model.vertexBuffer = renderer.CreateVertexBuffer(m_Vertices, sizeof(m_Vertices));
	m_Model.indexBuffer = renderer.CreateIndexBuffer(m_Indices, sizeof(m_Indices));

	m_NumIndices = 6;
	m_VertexSize = sizeof(SimpleShader::Vertex);
	m_VertexBuffer = &m_Model.vertexBuffer;
	m_IndexBuffer = &m_Model.indexBuffer;
}
