#pragma once

#include <d3d11.h>
#include <vector>
#include "Utility.h"

namespace d3d11 {

class ShaderProgram {
	unique_handle<ID3D11VertexShader> m_VertexShader;
	unique_handle<ID3D11PixelShader> m_PixelShader;

	std::vector<char> m_PSData;
	std::vector<char> m_VSData;

  public:

	ID3D11VertexShader *GetVertexShader() const {
		return m_VertexShader.get();
	}

	ID3D11PixelShader *GetPixelShader() const {
		return m_PixelShader.get();
	}

	const std::vector<char> &GetPixelShaderData() const {
		return m_PSData;
	}

	const std::vector<char> &GetVertexShaderData() const {
		return m_VSData;
	}

	friend class Renderer;
};

} // namespace d3d11
