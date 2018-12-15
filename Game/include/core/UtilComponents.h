#pragma once

#include "AABB2D.h"
#include "Camera2D.h"
#include "ECS.h"
#include "InputControl.h"
#include "Sprite.h"
#include "Transform.h"
#include "Transform2D.h"
#include <functional>

struct CTransform : public ecs::Component, public xm::Transform {};

struct CTransform2D : public ecs::Component, public xm::Transform2D {};

struct CSprite : public ecs::Component, public Sprite {
	RenderParams &m_RenderParams;
	CTransform2D *transform;

	CSprite(RenderParams &renderParams, const d3d11::Texture &texture,
			const math::rect *rect = nullptr)
		: Sprite(renderParams.m_Renderer, texture, rect),
		  m_RenderParams(renderParams) {
	}

	void Init() override {
		transform = &entity->GetComponent<CTransform2D>();
	}

	void Render() override {
		m_RenderParams.SetModel(transform->GetModel());
		m_RenderParams.UpdateShaderBuffer();
		m_RenderParams.Render(*this);
	}
};

struct CCamera2D : public ecs::Component {
	RenderParams &m_RenderParams;
	CTransform2D *transform;
	xm::vec2f m_Offset;
	Camera2D &camera;

	float aspectRatio;

	CCamera2D(RenderParams &renderParams, Camera2D &camera,
			  xm::vec2f offset = xm::vec2f(0.0f, 0.0f))
		: camera(camera), m_RenderParams(renderParams), m_Offset(offset) {
	}

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

class CBoxCollider2D : public ecs::Component {
	AABB2D m_AABB;
	CTransform2D *transform;

  public:
	void Init() override {
		transform = &entity->GetComponent<CTransform2D>();
	}

	std::function<void(const CBoxCollider2D &other,
					   const IntersectionData &data)>
		OnCollissionEnter;

	CBoxCollider2D(const xm::vec2f &minExtents, const xm::vec2f &maxExtents,
				   std::function<void(const CBoxCollider2D &other,
									  const IntersectionData &data)>
					   callback = nullptr)
		: m_AABB(minExtents, maxExtents), OnCollissionEnter(callback) {
	}

	void Update(float deltaTime) override {
		m_AABB.SetTranslation(transform->GetTranslation());
	}

	friend class InteractionSystem;
};

namespace MotionIntegrators {
void ModifiedEuler(xm::vec2f &position, xm::vec2f &velocity,
				   const xm::vec2f &acceleration, float deltaTime) {
	velocity = velocity + acceleration * deltaTime;
	position = position + velocity * deltaTime;
}
} // namespace MotionIntegrators

class CMotionComponent2D : public ecs::Component {
	xm::vec2f m_Velocity;
	xm::vec2f m_Acceleration;
	xm::Transform2D *m_Transform;

  public:
	CMotionComponent2D(const xm::vec2f &velocity = {0.0f, 0.0f},
					   const xm::vec2f &acceleration = {0.0f, 0.0f})
		: m_Velocity(velocity), m_Acceleration(acceleration) {
	}

	void Init() override {
		m_Transform = &entity->GetComponent<CTransform2D>();
	}

	void Update(float deltaTime) override {
		xm::vec2f newPos = m_Transform->GetTranslation();
		MotionIntegrators::ModifiedEuler(newPos, m_Velocity, m_Acceleration,
										 deltaTime);
		m_Transform->SetTranslation(newPos);
	}

	const xm::vec2f &GetVelocity() { return m_Velocity; }
	void SetVelocity(const xm::vec2f &velocity) { m_Velocity = velocity; }
	const xm::vec2f &GetAcceleration() { return m_Acceleration; }
	void SetAcceleration(const xm::vec2f &acceleration) { m_Acceleration = acceleration; }
};

struct MovementControl {
	xm::vec2f movement;
	InputControl *inputControl;
	float lastInputAmount;

	MovementControl(const xm::vec2f &movement, InputControl *inputControl)
		: movement(movement), inputControl(inputControl),
		  lastInputAmount(inputControl->GetAmount()) {}
};

class CMovementControl : public ecs::Component {
	CTransform2D *m_Transform;
	CMotionComponent2D *m_MotionComponent;
  public:
	std::vector<MovementControl> controls;

	void Init() override {
		m_Transform = &entity->GetComponent<CTransform2D>();
		m_MotionComponent = &entity->GetComponent<CMotionComponent2D>();
	}

	void Update(float deltaTime) override {
		for (auto &control : controls) {
			auto &movement = control.movement;
			auto input = control.inputControl;

			float inputDelta = input->GetAmount() - control.lastInputAmount;
			control.lastInputAmount = input->GetAmount();
			m_MotionComponent->SetAcceleration(m_MotionComponent->GetAcceleration() + movement * inputDelta);
		}
		m_MotionComponent->SetVelocity(m_MotionComponent->GetVelocity() * 0.95f);
	}
};

