#pragma once

#include "MathUtils.h"

namespace xm {

struct Transform {
	vec3f m_Translation;
	vec3f m_Scale;
	vec3f m_Rotation;

	Transform(const vec3f &position = vec3f(0.0f, 0.0f, 0.0f), 
			  const vec3f &scale = vec3f(1.0f, 1.0f, 1.0f),
			  const vec3f &rotation = vec3f(0.0f, 0.0f, 0.0f))
		: m_Translation(position), m_Scale(scale), m_Rotation(rotation) {}

	DirectX::XMMATRIX GetModel() const {
		return 
			DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)
			* DirectX::XMMatrixRotationAxis({1, 0, 0}, DirectX::XMConvertToRadians(m_Rotation.x))
			* DirectX::XMMatrixRotationAxis({0, 1, 0}, DirectX::XMConvertToRadians(m_Rotation.y))
			* DirectX::XMMatrixRotationAxis({0, 0, 1}, DirectX::XMConvertToRadians(m_Rotation.z))
			* DirectX::XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z));
	}

	void SetTranslation(const vec3f &translation) { m_Translation = translation; }
	const vec3f &GetTranslation() const { return m_Translation; }
	void SetScale(const vec3f &scale) { m_Scale = scale; }
	const vec3f &GetScale() const { return m_Scale; }
	void SetRotation(const vec3f &rotation) { m_Rotation = rotation; }
	const vec3f &GetRotation() const { return m_Rotation; }
};

}
