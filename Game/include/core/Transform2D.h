#pragma once

#include <DirectXMath.h>

namespace xm {

struct Transform2D {
	vec2f m_Translation;
	vec2f m_Scale;
	float m_Rotation;

	Transform2D(const vec2f &position = vec2f(0.0f, 0.0f), 
			  const vec2f &scale = vec2f(1.0f, 1.0f),
			  float rotation = 0)
		: m_Translation(position), m_Scale(scale), m_Rotation(rotation) {}

	DirectX::XMMATRIX GetModel() const {
		return 
			DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, 1)
			* DirectX::XMMatrixRotationAxis({0, 0, 1}, DirectX::XMConvertToRadians(m_Rotation))
			* DirectX::XMMatrixTranslation(m_Translation.x, m_Translation.y, 0));
	}

	void SetTranslation(const vec2f &translation) { m_Translation = translation; }
	const vec2f &GetTranslation() const { return m_Translation; }
	void SetScale(const vec2f &scale) { m_Scale = scale; }
	const vec2f &GetScale() const { return m_Scale; }
	void SetRotation(float rotation) { m_Rotation = rotation; }
	float GetRotation() const { return m_Rotation; }
};

}
