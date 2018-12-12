#include "Camera2D.h"

Camera2D::Camera2D() : m_Position(0.0f, 0.0f), m_Rotation(0.0f),
	m_AspectRatio(1.0f) {
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

Camera2D::Camera2D(float aspectRatio) : m_Position(0.0f, 0.0f), m_Rotation(0.0f),
	m_AspectRatio(aspectRatio) {
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera2D::SetPosition(const DirectX::XMFLOAT2 &position) {
	m_Position = position;
	UpdateViewMatrix();
}

void Camera2D::SetRotation(float rotation) {
	m_Rotation = rotation;
	UpdateViewMatrix();
}

void Camera2D::SetAspectRatio(float aspectRatio) {
	m_AspectRatio = aspectRatio;
	UpdateProjectionMatrix();
}

void Camera2D::UpdateViewMatrix() {
	m_View = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixTranslation(-m_Position.x, -m_Position.y, 0.0f)
		* DirectX::XMMatrixRotationAxis({0.0f, 0.0f, 1.0f}, DirectX::XMConvertToRadians(m_Rotation)));
}

void Camera2D::UpdateProjectionMatrix() {
	m_Projection = DirectX::XMMatrixScaling(m_AspectRatio, 1.0f, 1.0f);
}
