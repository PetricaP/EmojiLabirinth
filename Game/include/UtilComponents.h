#pragma once

#include "ECS.h"
#include "Transform.h"
#include "Transform2D.h"
#include "InputControl.h"
#include "Camera2D.h"
#include "Sprite.h"

struct CTransform : public ecs::Component, public xmm::Transform {};

struct CTransform2D : public ecs::Component, public xmm::Transform2D {};

struct CSprite : public ecs::Component, public Sprite {
	RenderParams &m_RenderParams;
	CTransform2D *transform;

	CSprite(RenderParams &renderParams, const d3d11::Texture &texture,
			const math::rect *rect = nullptr) : Sprite(renderParams.m_Renderer, texture, rect),
			m_RenderParams(renderParams) {}

	void Init() override {
		transform = &entity->GetComponent<CTransform2D>();
	}

	void Render() override {
		m_RenderParams.SetModel(transform->GetModel());
		m_RenderParams.UpdateShaderBuffer();
		m_RenderParams.Render(*this);
	}
};

struct CMovementControl : public ecs::Component {
	CTransform2D *transform;
	std::vector<std::pair<DirectX::XMFLOAT2, InputControl*>> controls;

	void Init() override {
		transform = &entity->GetComponent<CTransform2D>();
	}

	void Update(float deltaTime) override {
		for(const auto &control : controls) {
			auto &direction = control.first;
			auto input = control.second;

			auto &oldPos = transform->GetTranslation();
			DirectX::XMFLOAT2 newPos = oldPos + direction * input->GetAmount() * deltaTime;
			transform->SetTranslation(newPos);
		}
	}
};

/* TODO: Think of the way to make the camera component make sense */
/* while also managing projection and view correctly */
struct CCamera2D : public ecs::Component {
	RenderParams &m_RenderParams;
	CTransform2D *transform;
	DirectX::XMFLOAT2 m_Offset;
	Camera2D &camera;

	float aspectRatio;

	CCamera2D(RenderParams &renderParams, Camera2D &camera, 
		DirectX::XMFLOAT2 offset = DirectX::XMFLOAT2(0.0f, 0.0f))
		: camera(camera), m_RenderParams(renderParams), m_Offset(offset) {}

	void Init() override {
		transform = &entity->GetComponent<CTransform2D>();
	}

	void Update(float deltaTime) override {
		camera.SetPosition(transform->GetTranslation() + m_Offset);
		camera.SetRotation(transform->GetRotation());
		m_RenderParams.SetView(camera.GetViewMatrix());
		m_RenderParams.UpdateShaderBuffer();
	}
};

