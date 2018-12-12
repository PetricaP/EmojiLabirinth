#pragma once

#include "Renderer.h"
#include "SimpleShader.h"

class RenderParams {
	SimpleShader m_SimpleShader;
	SimpleShader::MatrixBuffer m_MBuffer;

  public:
	const d3d11::Renderer &m_Renderer;

	RenderParams(const d3d11::Renderer &renderer) : m_SimpleShader(renderer),
		m_Renderer(renderer) {
		m_Renderer.SetShader(m_SimpleShader);
		m_MBuffer.projection = DirectX::XMMatrixIdentity();
		m_MBuffer.view = DirectX::XMMatrixIdentity();
		m_MBuffer.model = DirectX::XMMatrixIdentity();
	}

	void SetProjection(const DirectX::XMMATRIX &projection) {
		m_MBuffer.projection = projection;
	}

	void SetModel(const DirectX::XMMATRIX &model) {
		m_MBuffer.model = model;
	}

	void SetView(const DirectX::XMMATRIX &view) {
		m_MBuffer.view = view;
	}

	void UpdateShaderBuffer() {
		m_SimpleShader.UpdateBuffer(m_MBuffer);
	}

	void Render(const Sprite &sprite) {
		m_Renderer.Submit(sprite);
	}
};
