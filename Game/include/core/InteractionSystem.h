#pragma once

#include "UtilComponents.h"
#include "ECS.h"

class InteractionSystem : public ecs::Listener {
	std::vector<CBoxCollider2D *> m_Colliders;

  public:
	InteractionSystem(ecs::Manager &manager) {
		manager.AddListener(this);
	}

	void OnAddEntity(ecs::Entity *entity) override {}

	void OnRemoveEntity(ecs::Entity *entity) override {
		for(size_t i = 0; i < m_Colliders.size(); ++i) {
			if(m_Colliders[i]->entity == entity) {
				m_Colliders[i] = m_Colliders[m_Colliders.size() - 1];
				m_Colliders.pop_back();
			}
		}
	}

	void OnAddComponent(ecs::Component *component) override {
		auto collider = dynamic_cast<CBoxCollider2D *>(component);
		if(collider) {
			m_Colliders.push_back(collider);
		}
	}

	void Update() {
		for(int i = 0; i < m_Colliders.size() - 1; ++i) {
			for(int j = i + 1; j < m_Colliders.size(); ++j) {
				auto data = m_Colliders[i]->m_AABB.IntersectAABB2D(m_Colliders[j]->m_AABB);
				if(data.intersects) {
					if(m_Colliders[i]->OnCollissionEnter) {
						m_Colliders[i]->OnCollissionEnter(*m_Colliders[j], data);
					}
					if(m_Colliders[j]->OnCollissionEnter) {
						m_Colliders[j]->OnCollissionEnter(*m_Colliders[i], data);
					}
				}
			}
		}
	}
};
