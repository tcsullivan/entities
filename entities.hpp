/**
 * @file entities.hpp
 * @brief A simply-written entity component system.
 */
#ifndef ENTITIES_HPP_
#define ENTITIES_HPP_

#include <algorithm> // std::find_if
#include <type_traits> // std::is_convertible
//#include "pvector.hpp"
#include <vector>

#define Container std::vector

/**
 * @class Component
 * A base class for all components to inherit.
 */
class Component {
	/**
	 * Constructs the class using the given XML element(s).
	 */
	virtual void fromXML(/*TODO*/) {}
};

/** An ID number for entities. */
using Id = unsigned int;

/**
 * @struct EntityData
 * Contains all components and an ID, to form an entity.
 */
struct EntityData {
	/** The entity's ID. */
	Id id;

	/** A vector of all components. */
	Container<Component*> components;

	/** Constructs an entity with the given ID. */
	EntityData(Id _id = -1)
		: id (_id) {}

	/** Compares two entities through their IDs. */
	bool operator==(const EntityData& e) {
		return id == e.id;
	}
};

/**
 * @struct Entity
 * Allows access to an entity and it's components.
 * Note that this is not the actual entity's data, that is stored in the
 * EntityManager's EntityData array.
 */
struct Entity {
	Container<EntityData>::iterator pos;

	/** Constructs an entity object to handle the given entity data. */
	Entity(Container<EntityData>::iterator p)
		:  pos(p) {}
	Entity(EntityData& d)
		: pos(&d) {}

	/**
	 * Assigns a component to the entity.
	 * @param args arguments to pass to the component's constructor.
	 * @return a pointer to the new component
	 */
	template<class T, typename... Args>
	T* assign(Args... args) {
		static_assert(std::is_convertible<T*, Component*>::value,
			"components must inherit Component base class");
		auto comp = new T(args...);
		(*pos).components.push_back(comp);
		return comp;
	}

	/**
	 * Removes a component of the given type from the entity.
	 */
	template<class T>
	void remove(void) {
		static_assert(std::is_convertible<T*, Component*>::value,
			"components must inherit Component base class");
		auto c = std::find_if((*pos).components.begin(), (*pos).components.end(),
			[](auto c){ return dynamic_cast<T*>(c); });
		if (c != (*pos).components.end())
			(*pos).components.erase(c);
	}

	/**
	 * Tests if the entity has a component of the given type.
	 * @return true if the entity has the component
	 */
	template<class T>
	bool hasComponent(void) const {
		static_assert(std::is_convertible<T*, Component*>::value,
			"components must inherit Component base class");
		auto c = std::find_if((*pos).components.begin(), (*pos).components.end(),
			[](auto c){ return dynamic_cast<T*>(c); });
		return c != (*pos).components.end();
	}

	/**
	 * Fetches a component from the entity.
	 * @return the component, nullptr if the entity does not have it
	 */
	template<class T>
	T* component(void) {
		static_assert(std::is_convertible<T*, Component*>::value,
			"components must inherit Component base class");
		auto c = std::find_if((*pos).components.begin(), (*pos).components.end(),
			[](auto c){ return dynamic_cast<T*>(c); });
		return (c != (*pos).components.end()) ? dynamic_cast<T*>(*c) : nullptr;
	}
};

/**
 * @class EntityManager
 * Manages a group of entities.
 */
class EntityManager {
private:
	/** The array of all entities. */
	Container<EntityData> entities;

public:
	// max is not enforced
	EntityManager()
		/*: entities()*/ {}

	~EntityManager(void) {
		entities.clear();
	}

	/**
	 * Creates a new entity.
	 * @return an Entity object for the new entity
	 */
	Entity create(void) {
		static Id newId = 0;

		entities.emplace_back(newId);
		return Entity(entities.begin() + entities.size() - 1);
	}

	/**
	 * Kills (removes) an entity.
	 * @param e the entity to remove
	 */
	void kill(const Entity& e) {
		if (e.pos > entities.begin() && e.pos < entities.end())
			entities.erase(e.pos);
	}

	/**
	 * Destroys all entities.
	 */
	void reset(void) {
		entities.clear();
	}

	/**
	 * Runs a function through all entities.
	 * @param f the function to run through
	 */
	void each(std::function<void(Entity e)> f) {
		for (auto i = entities.begin(); i < entities.end(); ++i)
			f(Entity(i));
	}

	template<class T1>
	void trySort(void) {
		static unsigned int oldSize = 0;
		if (entities.size() < 100000 && entities.size() != oldSize) {
			oldSize = entities.size();

			std::sort(entities.begin(), entities.end(),
			[](auto&& e1, auto&& e2){
					return Entity(e1).hasComponent<T1>() && !Entity(e2).hasComponent<T1>();
			});
		}
	}

	/**
	 * Runs a function through all entities with the given components.
	 * @param f the function to run through
	 */
	template<class T1>
	void each(std::function<void(Entity e)> f) {
		trySort<T1>();
		bool good = false;
		for (auto i = entities.begin(); i < entities.end(); ++i) {
			Entity en (i);
			if (en.hasComponent<T1>()) {
				f(en);
				good = true;
			} else if (good) break;
		}
	}

	template<class T1, class T2>
	void each(std::function<void(Entity e)> f) {
		trySort<T1>();
		bool good = false;
		for (auto i = entities.begin(); i < entities.end(); ++i) {
			Entity en (i);
			if (en.hasComponent<T1>() && en.hasComponent<T2>()) {
				f(en);
				good = true;
			} else if (good) break;
		}
	}
};



using DeltaTime = int;

class System {
public:
	virtual void update(EntityManager& em, DeltaTime dt) = 0;
};

class SystemManager {
private:
	Container<System*> systems;
	EntityManager& entities;

public:
	SystemManager(EntityManager& em)
		: entities(em) {}

	template<class T, typename... Args>
	void add(Args... args) {
		static_assert(std::is_convertible<T*, System*>::value,
			"systems must inherit System base class");
		systems.push_back(new T(args...));
	}

	template<class T>
	void update(DeltaTime dt) {
		static_assert(std::is_convertible<T*, System*>::value,
			"systems must inherit System base class");
		for (auto s : systems) {
			if (dynamic_cast<T*>(s)) {
				s->update(entities, dt);
				return;
			}
		}
	}
};

#endif // ENTITIES_HPP_
