#pragma once

#include "AABB2D.h"
#include "Camera2D.h"
#include "ECS.h"
#include "InputControl.h"
#include "Sprite.h"
#include "Transform.h"
#include "Transform2D.h"
#include "RenderParams.h"
#include <functional>

struct CTransform : public ecs::Component, public math::Transform {};

struct CTransform2D : public ecs::Component, public math::Transform2D {};

struct CSprite : public ecs::Component, public Sprite {
	RenderParams &m_RenderParams;
	CTransform2D *transform;

	CSprite(RenderParams &renderParams, const d3d11::Texture &texture,
		   const math::vec2f &bottomLeft = {0.0f, 0.0f},
		   const math::vec2f &topRight = {1.0f, 1.0f})
		: Sprite(renderParams.m_Renderer, texture, bottomLeft, topRight),
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
	math::vec2f m_Offset;
	Camera2D &camera;

	float aspectRatio;

	CCamera2D(RenderParams &renderParams, Camera2D &camera,
			  math::vec2f offset = math::vec2f(0.0f, 0.0f))
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

	CBoxCollider2D(const math::vec2f &minExtents, const math::vec2f &maxExtents,
				   std::function<void(const CBoxCollider2D &other,
									  const IntersectionData &data)>
					   callback = nullptr)
		: m_AABB(minExtents, maxExtents), OnCollissionEnter(callback) {
	}

	void Update(float deltaTime) override {
		m_AABB.SetTranslation(transform->GetTranslation());
	}

	const AABB2D &GetAABB() const { return m_AABB; }

	friend class InteractionSystem;
};

namespace MotionIntegrators {
void ModifiedEuler(math::vec2f &position, math::vec2f &velocity,
				   const math::vec2f &acceleration, float deltaTime) {
	velocity = velocity + acceleration * deltaTime;
	position = position + velocity * deltaTime;
}
} // namespace MotionIntegrators

class CMotionComponent2D : public ecs::Component {
	math::vec2f m_Velocity;
	math::vec2f m_Acceleration;
	math::Transform2D *m_Transform;

  public:
	CMotionComponent2D(const math::vec2f &velocity = {0.0f, 0.0f},
					   const math::vec2f &acceleration = {0.0f, 0.0f})
		: m_Velocity(velocity), m_Acceleration(acceleration) {
	}

	void Init() override {
		m_Transform = &entity->GetComponent<CTransform2D>();
	}

	void Update(float deltaTime) override {
		math::vec2f newPos = m_Transform->GetTranslation();
		MotionIntegrators::ModifiedEuler(newPos, m_Velocity, m_Acceleration,
										 deltaTime);
		auto norm = math::norm(m_Velocity);
		static constexpr float maxVel = 0.8f;
		if(norm > maxVel) {
			m_Velocity = m_Velocity / norm * maxVel;
		}
		m_Transform->SetTranslation(newPos);
	}

	const math::vec2f &GetVelocity() { return m_Velocity; }
	void SetVelocity(const math::vec2f &velocity) { m_Velocity = velocity; }
	const math::vec2f &GetAcceleration() { return m_Acceleration; }
	void SetAcceleration(const math::vec2f &acceleration) { m_Acceleration = acceleration; }
};

struct MovementControl {
	math::vec2f movement;
	InputControl *inputControl;
	float lastInputAmount;

	MovementControl(const math::vec2f &movement, InputControl *inputControl)
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

