#pragma once

#include <vector>
#include <memory>
#include <array>
#include <bitset>
#include "Utility.h"

namespace ecs {

class Entity;

constexpr std::size_t MAX_COMPONENTS {32ul};
constexpr std::size_t MAX_GROUPS {32ul};

using ComponentID = std::size_t;
using Group = std::size_t;

namespace Internal {

inline ComponentID GetUniqueComponentID() {
	static ComponentID lastID{0ul};
	return lastID++;
}

} // namespace Internal

struct Component {
	Entity *entity;

	virtual void Init() {}
	virtual void Update(float deltaTime) {}
	virtual void Render() {}

	virtual ~Component() {}
};

template <typename T>
inline ComponentID GetComponentTypeID() {
	static_assert(std::is_base_of<Component, T>::value, "T must inherit from component");

	static ComponentID typeID{Internal::GetUniqueComponentID()};
	return typeID;
}

class Manager;

class Entity {
	Manager &m_Manager;
	std::vector<std::unique_ptr<Component>> m_Components;
	std::array<Component *, MAX_COMPONENTS> m_ComponentArray;
	std::bitset<MAX_COMPONENTS> m_ComponentBitset;
	std::bitset<MAX_GROUPS> m_GroupBitset;
	bool m_IsAlive {true};

  public:
	Entity(Manager &manager) : m_Manager(manager) {}

	void Update(float deltaTime) {
		for(auto &c : m_Components) {
			c->Update(deltaTime);
		}
	}

	void Render() {
		for(auto &c : m_Components) {
			c->Render();
		}
	}

	bool IsAlive() { return m_IsAlive; }

	void Destroy() { m_IsAlive = false; }

	template <typename T>
	bool HasComponent() const {
		return m_ComponentBitset[GetComponentTypeID<T>()];
	}

	bool HasGroup(Group group) const {
		return m_GroupBitset[group];
	}

	void AddGroup(Group group);
	void RemoveGroup(Group group) { m_GroupBitset[group] = false; }

	template <typename T, typename... TArgs>
	T &AddComponent(TArgs&&... args) {
		ASSERT(!HasComponent<T>());

		T *component{new T(std::forward<TArgs>(args)...)};
		component->entity = this;
		std::unique_ptr<Component> pComponent{component};
		m_Components.emplace_back(std::move(pComponent));

		m_ComponentArray[GetComponentTypeID<T>()] = component;
		m_ComponentBitset[GetComponentTypeID<T>()] = true;

		component->Init();
		return *component;
	}

	template <typename T>
	T &GetComponent() const {
		ASSERT(HasComponent<T>());
		auto pComponent(m_ComponentArray[GetComponentTypeID<T>()]);
		return *reinterpret_cast<T *>(pComponent);
	}
};

class Manager {
  private:
	std::vector<std::unique_ptr<Entity>> m_Entities;
	std::array<std::vector<Entity *>, MAX_GROUPS> m_Groups;

  public:
	void Update(float deltaTime) {
		for(auto &e : m_Entities) {
			e->Update(deltaTime);
		}
	}

	void Render() {
		for(auto &e : m_Entities) {
			e->Render();
		}
	}

	void AddToGroup(Entity *entity, Group group) {
		m_Groups[group].emplace_back(entity);
	}

	std::vector<Entity *> &GetEntitiesByGroup(Group group) {
		return m_Groups[group];
	}

	void Refresh() {
		for(auto i(0u); i < MAX_GROUPS; ++i) {
			auto &entities(m_Groups[i]);

			entities.erase(std::remove_if(std::begin(entities), std::end(entities),
				[i](Entity *entity) {
					return !entity->IsAlive() || !entity->HasGroup(i);
				}
			), std::end(entities));
		}

		m_Entities.erase(std::remove_if(std::begin(m_Entities), std::end(m_Entities),
			[](const std::unique_ptr<Entity> &entity) {
				return !entity->IsAlive();
			}
		), std::end(m_Entities));
	}

	Entity *AddEntity() {
		Entity *e(new Entity(*this));
		std::unique_ptr<Entity> pEntity {e};
		m_Entities.emplace_back(std::move(pEntity));
		return e;
	}
};

void Entity::AddGroup(Group group) {
	m_GroupBitset[group] = true;
	m_Manager.AddToGroup(this, group);
}

} // namespace ecs
