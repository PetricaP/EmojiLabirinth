#pragma once

#include <DirectXMath.h>

namespace xmm {

struct Transform {
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT3 m_Rotation;

	Transform(const DirectX::XMFLOAT3 &position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 
			  const DirectX::XMFLOAT3 &scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
			  const DirectX::XMFLOAT3 &rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f))
		: m_Position(position), m_Scale(scale), m_Rotation(rotation) {}

	DirectX::XMMATRIX GetModel() const {
		return 
			DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)
			* DirectX::XMMatrixRotationAxis({1, 0, 0}, DirectX::XMConvertToRadians(m_Rotation.x))
			* DirectX::XMMatrixRotationAxis({0, 1, 0}, DirectX::XMConvertToRadians(m_Rotation.y))
			* DirectX::XMMatrixRotationAxis({0, 0, 1}, DirectX::XMConvertToRadians(m_Rotation.z))
			* DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z));
	}

	void SetPosition(const DirectX::XMFLOAT3 &position) { m_Position = position; }
	const DirectX::XMFLOAT3 GetPosition() const { return m_Position; }
	void SetScale(const DirectX::XMFLOAT3 &scale) { m_Scale = scale; }
	const DirectX::XMFLOAT3 GetScale() const { return m_Scale; }
	void SetRotation(const DirectX::XMFLOAT3 &rotation) { m_Rotation = rotation; }
	const DirectX::XMFLOAT3 GetRotation() const { return m_Rotation; }
};

}
