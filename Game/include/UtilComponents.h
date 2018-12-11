#pragma once

#include "ECS.h"
#include "Transform.h"
#include "InputControl.h"
#include "Sprite.h"

struct CTransform : public ecs::Component, public xmm::Transform {};

struct CSprite : public ecs::Component, public Sprite {

	CSprite(const d3d11::Renderer &renderer, const d3d11::Texture &texture,
			const math::rect *rect = nullptr) : Sprite(renderer, texture, rect) {}
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
			DirectX::XMFLOAT3 newPos = {oldPos.x + direction.x * input->GetAmount() * deltaTime,
										oldPos.y + direction.y * input->GetAmount() * deltaTime,
										oldPos.z + direction.z * input->GetAmount() * deltaTime};
			transform->SetTranslation(newPos);
		}
	}
};
