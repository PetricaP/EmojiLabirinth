#include "Camera.h"

Camera::Camera() : m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f),
	m_AspectRatio(1.0f) {
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

Camera::Camera(float aspectRatio) : m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f),
	m_AspectRatio(aspectRatio) {
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::SetPosition(const DirectX::XMFLOAT3 &position) {
	m_Position = position;
	UpdateViewMatrix();
}

void Camera::SetRotation(float rotation) {
	m_Rotation = rotation;
	UpdateViewMatrix();
}

void Camera::SetAspectRatio(float aspectRatio) {
	m_AspectRatio = aspectRatio;
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix() {
	m_View = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, m_Rotation)
		* DirectX::XMMatrixTranslation(-m_Position.x, -m_Position.y, -m_Position.z));
}

void Camera::UpdateProjectionMatrix() {
	m_Projection = DirectX::XMMatrixScaling(m_AspectRatio, 1.0f, 1.0f);
}
