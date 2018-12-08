#pragma once

#include "SimpleShader.h"
#include "Renderer.h"
#include "MathUtils.h"
#include "Model.h"

class Sprite : public d3d11::Drawable {
	static constexpr SimpleShader::Vertex m_Vertices[] = {
		{-1.0f, -1.0f,  0.0f,  0.0f},
		{-1.0f,  1.0f,  0.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f,  0.0f}
	};
	static constexpr unsigned int m_Indices[] = {0, 1, 2, 0, 2, 3};

	const d3d11::Texture &m_Texture;
	d3d11::Model m_Model;

  public:
	Sprite(const d3d11::Renderer &renderer, const d3d11::Texture &texture, 
		   const math::rect *rect = nullptr);

	Sprite(const Sprite &other) = delete;
	Sprite &operator=(const Sprite &other) = delete;

	friend class d3d11::Renderer;
};
