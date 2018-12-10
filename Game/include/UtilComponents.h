#pragma once

#include "ECS.h"
#include "Transform.h"
#include "Sprite.h"

struct CTransform : public ecs::Component, public xmm::Transform {

	/*
	void Update(float deltaTime) override {
		transform.SetPosition({transform.GetPosition().x + 1,
							   transform.GetPosition().y,
							   transform.GetPosition().z});
	}
	*/
};

struct CSprite : public ecs::Component, public Sprite {

	CSprite(const d3d11::Renderer &renderer, const d3d11::Texture &texture,
			const math::rect *rect = nullptr) : Sprite(renderer, texture, rect) {}
};
