#include "InteractionSystem.h"

/*
void InteractionSystem::OnAddComponent(ecs::Component *component) {
	auto collider = dynamic_cast<CBoxCollider2D *>(component);
	if(collider) {
		m_Colliders.push_back(collider);
	}
}

void InteractionSystem::Update() {
	for(int i = 0; i < m_Colliders.size() - 1; ++i) {
		for(int j = i + 1; i < m_Colliders.size(); ++i) {
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
*/
