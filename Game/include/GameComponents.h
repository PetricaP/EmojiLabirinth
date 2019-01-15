#pragma once

#include "UtilComponents.h"
#include "Tile.h"

class CEnemyControl : public ecs::Component {
	CMotionComponent2D *m_MotionComponent;
	CTransform2D *m_TransformComponent;
	CSprite *m_SpriteComponent;
	Node *m_LastNode;
	Node *m_NextNode;

  public:
	CEnemyControl(Node &firstNode)
		: m_LastNode(&firstNode), m_NextNode((firstNode.GetNeighbors()[0])->node) {}

	void Init() override {
		m_MotionComponent = &entity->GetComponent<CMotionComponent2D>();
		m_TransformComponent = &entity->GetComponent<CTransform2D>();
		m_SpriteComponent = &entity->GetComponent<CSprite>();
		m_TransformComponent->SetTranslation(m_LastNode->GetTile()->GetPosition());

		m_MotionComponent->SetAcceleration({0.0f, 0.0f});
		auto &&nextPosition = m_NextNode->GetTile()->GetPosition();
		auto &&lastPosition = m_LastNode->GetTile()->GetPosition();
		auto &&difVec = nextPosition - lastPosition;
		auto &&newVelocity = 0.5f * math::normalize(difVec);
		m_MotionComponent->SetVelocity(newVelocity);
	}

	void Update(float deltaTime) override {
		static constexpr float eps = 0.01f;
		auto &position = m_TransformComponent->GetTranslation();
		auto &&nodePosition = m_NextNode->GetTile()->GetPosition();
		auto delta = math::norm(position - nodePosition);
		if(delta < eps) {
			m_TransformComponent->SetTranslation(nodePosition);

			auto randIndex =  rand() % m_NextNode->GetNeighbors().size();
			auto nextNode = m_NextNode->GetNeighbors()[randIndex]->node;
			if(nextNode == m_LastNode && m_NextNode->GetNeighbors().size() > 1) {
				auto newRandIndex = randIndex;
				int i = 0;
				while(newRandIndex == randIndex && i < 2) {
					newRandIndex = rand() % m_NextNode->GetNeighbors().size();
				}
				nextNode = m_NextNode->GetNeighbors()[newRandIndex]->node;
			}
			m_LastNode = m_NextNode;
			m_NextNode = nextNode;

			auto &&nextPosition = m_NextNode->GetTile()->GetPosition();
			auto &&lastPosition = m_LastNode->GetTile()->GetPosition();
			auto &&difVec = nextPosition - lastPosition;
			auto &&newVelocity = 0.5f * math::normalize(difVec);

			m_MotionComponent->SetVelocity(newVelocity);
		}
	}
};
