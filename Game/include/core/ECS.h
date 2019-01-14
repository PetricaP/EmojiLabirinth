#pragma once

#include "Utility.h"
#include <array>
#include <bitset>
#include <memory>
#include <vector>

namespace ecs {

class Entity;

constexpr std::size_t MAX_COMPONENTS{32ul};
constexpr std::size_t MAX_GROUPS{32ul};

using ComponentID = std::size_t;
using Group = std::size_t;

namespace Internal {

inline ComponentID GetUniqueComponentID() {
	static ComponentID lastID{0ul};
	return lastID++;
}

} // namespace Internal

// TODO: Maybe inject the ID into the component somehow
struct Component {
	Entity *entity;

	virtual void Init() {
	}
	virtual void Update(float deltaTime) {
	}
	virtual void Render() {
	}

	virtual ~Component() {
	}
};

template <typename T>
inline ComponentID GetComponentTypeID() {
	static_assert(std::is_base_of<Component, T>::value,
				  "T must inherit from component");

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
	bool m_IsAlive{true};

  public:
	Entity(Manager &manager) : m_Manager(manager) {
	}

	void Update(float deltaTime) {
		for (auto &c : m_Components) {
			c->Update(deltaTime);
		}
	}

	void Render() {
		for (auto &c : m_Components) {
			c->Render();
		}
	}

	bool IsAlive() {
		return m_IsAlive;
	}

	void Destroy() {
		m_IsAlive = false;
	}

	template <typename T, typename... TArgs>
	T &AddComponent(TArgs &&... args);

	template <typename T>
	bool HasComponent() const {
		return m_ComponentBitset[GetComponentTypeID<T>()];
	}

	bool HasGroup(Group group) const {
		return m_GroupBitset[group];
	}

	void AddGroup(Group group);
	void RemoveGroup(Group group) {
		m_GroupBitset[group] = false;
	}

	template <typename T>
	T &GetComponent() const {
		ASSERT(HasComponent<T>());
		auto pComponent(m_ComponentArray[GetComponentTypeID<T>()]);
		return *reinterpret_cast<T *>(pComponent);
	}
};

struct Listener {
	virtual void OnAddComponent(Component *component) {}
	virtual void OnAddEntity(Entity *entity) {}
	virtual void OnRemoveEntity(Entity *entity) {}
	virtual ~Listener() {}
};

class Manager {
  private:
	std::vector<std::unique_ptr<Entity>> m_Entities;
	std::array<std::vector<Entity *>, MAX_GROUPS> m_Groups;
	std::vector<Listener *> m_Listeners;

  public:
	void Update(float deltaTime) {
		for (auto &e : m_Entities) {
			e->Update(deltaTime);
		}
	}

	void Render() {
		for (auto &e : m_Entities) {
			e->Render();
		}
	}

	void AddToGroup(Entity *entity, Group group) {
		m_Groups[group].emplace_back(entity);
	}

	void AddListener(Listener *listener) {
		m_Listeners.push_back(listener);
	}

	std::vector<Entity *> &GetEntitiesByGroup(Group group) {
		return m_Groups[group];
	}

	void Refresh() {
		for(size_t i = 0; i < m_Entities.size(); ++i) {
			if(!m_Entities[i]->IsAlive()) {
				for(auto listener : m_Listeners) {
					listener->OnRemoveEntity(m_Entities[i].get());
				}
				m_Entities[i] = std::move(m_Entities[m_Entities.size() - 1]);
				m_Entities.pop_back();
			}
		}
	}

	Entity &AddEntity() {
		Entity *e(new Entity(*this));
		std::unique_ptr<Entity> pEntity{e};
		m_Entities.emplace_back(std::move(pEntity));
		for(auto &listener : m_Listeners) {
			listener->OnAddEntity(e);
		}
		DEBUG_LOG("Added entity.\n");
		return *e;
	}

	void OnAddComponent(Component *comp) {
		for(auto &listener : m_Listeners) {
			listener->OnAddComponent(comp);
		}
	}
};

void Entity::AddGroup(Group group) {
	m_GroupBitset[group] = true;
	m_Manager.AddToGroup(this, group);
}

template <typename T, typename... TArgs>
T &Entity::AddComponent(TArgs &&... args) {
	ASSERT(!HasComponent<T>());

	T *component{new T(std::forward<TArgs>(args)...)};
	component->entity = this;
	std::unique_ptr<Component> pComponent{component};
	m_Components.emplace_back(std::move(pComponent));

	auto ID = GetComponentTypeID<T>();
	m_ComponentArray[ID] = component;
	m_ComponentBitset[ID] = true;
	std::stringstream ss;
	ss << "Added component with ID: " << ID << std::endl;
	DEBUG_LOG(ss.str().c_str());

	component->Init();
	DEBUG_LOG("Initialized component\n");
	m_Manager.OnAddComponent(component);
	return *component;
}


} // namespace ecs

