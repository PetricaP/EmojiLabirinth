#pragma once

#include <DirectXMath.h>

class Camera2D {
  private:
	DirectX::XMFLOAT2 m_Position;
	float m_Rotation;
	float m_AspectRatio;
	DirectX::XMMATRIX m_View;
	DirectX::XMMATRIX m_Projection;

  public:
	Camera2D();
	Camera2D(float aspectRatio);

	const DirectX::XMMATRIX &GetViewMatrix() const { return m_View; }
	const DirectX::XMMATRIX &GetProjectionMatrix() const { return m_Projection; }

	const DirectX::XMFLOAT2 &GetPosition() const { return m_Position; }
	const float &GetRotation() const { return m_Rotation; }

	void SetPosition(const DirectX::XMFLOAT2 &position);
	void SetRotation(float rotation);

	void SetAspectRatio(float aspectRatio);

  private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
};
