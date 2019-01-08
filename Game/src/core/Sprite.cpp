#include "Sprite.h"

Sprite::Sprite(const d3d11::Renderer &renderer, const d3d11::Texture &texture,
			   const math::vec2f &bottomLeft, const math::vec2f &topRight)
	: m_Renderer(renderer), m_Texture(texture) {
	SimpleShader::Vertex m_Vertices[] = {
		{-1.0f, -1.0f,  topRight.x,  topRight.y},
		{-1.0f,  1.0f,  topRight.x,  bottomLeft.y},
		{ 1.0f,  1.0f,  bottomLeft.x,  bottomLeft.y},
		{ 1.0f, -1.0f,  bottomLeft.x,  topRight.y}
	};

	static constexpr unsigned int m_Indices[] = {0, 1, 2, 0, 2, 3};
	m_Model.vertexBuffer = renderer.CreateVertexBuffer(m_Vertices, sizeof(m_Vertices));
	m_Model.indexBuffer = renderer.CreateIndexBuffer(m_Indices, sizeof(m_Indices));

	m_NumIndices = 6;
	m_VertexSize = sizeof(SimpleShader::Vertex);
	m_VertexBuffer = &m_Model.vertexBuffer;
	m_IndexBuffer = &m_Model.indexBuffer;
}

void Sprite::SetRect(const math::vec2f &bottomLeft,
					 const math::vec2f &topRight) {
	SimpleShader::Vertex m_Vertices[] = {
		{-1.0f, -1.0f,  topRight.x,  topRight.y},
		{-1.0f,  1.0f,  topRight.x,  bottomLeft.y},
		{ 1.0f,  1.0f,  bottomLeft.x,  bottomLeft.y},
		{ 1.0f, -1.0f,  bottomLeft.x,  topRight.y}
	};
	m_Model.vertexBuffer = m_Renderer.CreateVertexBuffer(m_Vertices, sizeof(m_Vertices));
}
