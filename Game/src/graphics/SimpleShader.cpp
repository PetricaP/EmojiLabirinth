#include "SimpleShader.h"

SimpleShader::SimpleShader(const d3d11::Renderer &renderer) : Shader(renderer) {
	m_MatrixBuffer = m_Renderer.CreateConstantBuffer(sizeof(MatrixBuffer), D3D11_USAGE_DYNAMIC);
	m_ShaderProgram = m_Renderer.CreateShaderProgram(m_VSFilePath, m_PSFilePath);
	m_InputLayout = m_Renderer.CreateInputLayout({{"POSITION", DXGI_FORMAT_R32G32_FLOAT},
									  {"TEXCOORD", DXGI_FORMAT_R32G32_FLOAT}}, m_ShaderProgram);
}

void SimpleShader::UpdateBuffer(const MatrixBuffer &buffer) {
	m_Renderer.MapResource(m_MatrixBuffer, &buffer);
	m_Renderer.VSSetConstantBuffer(m_MatrixBuffer, 0);
}
