#pragma once

#include "SimpleShader.h"
#include "Renderer.h"
#include "MathUtils.h"
#include "Model.h"

class Sprite : public d3d11::Drawable {

	const d3d11::Texture &m_Texture;
	d3d11::Model m_Model;
	const d3d11::Renderer &m_Renderer;

  public:
	Sprite(const d3d11::Renderer &renderer, const d3d11::Texture &texture, 
		   const math::vec2f &bottomLeft = {0.0f, 0.0f},
		   const math::vec2f &topRight = {1.0f, 1.0f});

	Sprite(const Sprite &other) = delete;
	Sprite &operator=(const Sprite &other) = delete;

	void SetRect(const math::vec2f &bottomLeft,
				 const math::vec2f &topRight);

	friend class d3d11::Renderer;
};
