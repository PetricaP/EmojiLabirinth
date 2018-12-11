#pragma once

#include "Renderer.h"
#include "Shader.h"
#include <DirectXMath.h>

class SimpleShader : public d3d11::Shader {
  private:
	d3d11::ConstantBuffer m_MatrixBuffer;
	static constexpr const char m_VSFilePath[30]{"simpleVertexShader.cso"};
	static constexpr const char m_PSFilePath[30]{"simplePixelShader.cso"};

  public:
	SimpleShader(const d3d11::Renderer &renderer);

	/* Doesn't really feel good */
	struct MatrixBuffer {
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX projection;
		SimpleShader *shader = nullptr;

		void UpdateModel(const DirectX::XMMATRIX &model) {
			this->model = model;
			shader->UpdateBuffer(*this);
		}

		void UpdateProjection(const DirectX::XMMATRIX &projection) {
			this->projection = projection;
			shader->UpdateBuffer(*this);
		}
	};

	void UpdateBuffer(const MatrixBuffer &buffer);

	struct Vertex {
		float x, y;
		float u, v;
	};

	static constexpr size_t VERTEX_SIZE = sizeof(Vertex);
};
