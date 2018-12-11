#pragma once

#include <DirectXMath.h>

class Camera {
  private:
	DirectX::XMFLOAT3 m_Position;
	float m_Rotation;
	float m_AspectRatio;
	DirectX::XMMATRIX m_View;
	DirectX::XMMATRIX m_Projection;

  public:
	Camera();
	Camera(float aspectRatio);

	const DirectX::XMMATRIX &GetViewMatrix() const { return m_View; }
	const DirectX::XMMATRIX &GetProjectionMatrix() const { return m_Projection; }

	const DirectX::XMFLOAT3 &GetPosition() const { return m_Position; }
	const float &GetRotation() const { return m_Rotation; }

	void SetPosition(const DirectX::XMFLOAT3 &position);
	void SetRotation(float rotation);

	void SetAspectRatio(float aspectRatio);

  private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
};
