#pragma once

#include "InputLayout.h"
#include "ShaderProgram.h"

namespace d3d11 {

class Shader {
  protected:
	InputLayout m_InputLayout;
	ShaderProgram m_ShaderProgram;
	const d3d11::Renderer &m_Renderer;

  protected:
	Shader(const d3d11::Renderer &renderer) : m_Renderer(renderer) {}

  public:
	~Shader() = default;
	const InputLayout &GetInputLayout() const { return m_InputLayout; }
	const ShaderProgram &GetProgram() const { return m_ShaderProgram; }
};

} // namespace d3d11
