#pragma once

#include <DirectXMath.h>

namespace xmm {

struct Transform2D {
	DirectX::XMFLOAT2 m_Translation;
	DirectX::XMFLOAT2 m_Scale;
	float m_Rotation;

	Transform2D(const DirectX::XMFLOAT2 &position = DirectX::XMFLOAT2(0.0f, 0.0f), 
			  const DirectX::XMFLOAT2 &scale = DirectX::XMFLOAT2(1.0f, 1.0f),
			  float rotation = 0)
		: m_Translation(position), m_Scale(scale), m_Rotation(rotation) {}

	DirectX::XMMATRIX GetModel() const {
		return 
			DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, 1)
			* DirectX::XMMatrixRotationAxis({0, 0, 1}, DirectX::XMConvertToRadians(m_Rotation))
			* DirectX::XMMatrixTranslation(m_Translation.x, m_Translation.y, 0));
	}

	void SetTranslation(const DirectX::XMFLOAT2 &translation) { m_Translation = translation; }
	const DirectX::XMFLOAT2 &GetTranslation() const { return m_Translation; }
	void SetScale(const DirectX::XMFLOAT2 &scale) { m_Scale = scale; }
	const DirectX::XMFLOAT2 &GetScale() const { return m_Scale; }
	void SetRotation(float rotation) { m_Rotation = rotation; }
	float GetRotation() const { return m_Rotation; }
};

}
