#pragma once

#include "ECS.h"
#include "Transform.h"
#include "InputControl.h"
#include "Sprite.h"

struct CTransform : public ecs::Component, public xmm::Transform {};

struct CSprite : public ecs::Component, public Sprite {
	GameRenderContext &m_RenderContext;
	CTransform *transform;

	CSprite(GameRenderContext &renderContext, const d3d11::Texture &texture,
			const math::rect *rect = nullptr) : Sprite(renderContext.m_Renderer, texture, rect),
			m_RenderContext(renderContext) {}

	void Init() override {
		transform = &entity->GetComponent<CTransform>();
	}

	void Render() override {
		m_RenderContext.SetModel(transform->GetModel());
		m_RenderContext.UpdateShaderBuffer();
		m_RenderContext.Render(*this);
	}
};

struct CMovementControl : public ecs::Component {
	CTransform *transform;
	std::vector<std::pair<DirectX::XMFLOAT3, InputControl*>> controls;

	void Init() override {
		transform = &entity->GetComponent<CTransform>();
	}

	void Update(float deltaTime) override {
		for(const auto &control : controls) {
			auto &direction = control.first;
			auto input = control.second;

			auto &oldPos = transform->GetTranslation();
			DirectX::XMFLOAT3 newPos = oldPos + direction * input->GetAmount() * deltaTime;
			transform->SetTranslation(newPos);
		}
	}
};
