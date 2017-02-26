/**
 * @file entities.hpp
 * @brief A simply-written entity component system.
 */
#ifndef ENTITIES_HPP_
#define ENTITIES_HPP_

#include <algorithm> // std::find_if
#include <type_traits> // std::is_convertible
#include <vector>
#include <forward_list>
#include <map>


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
	std::forward_list<size_t> componentHash;
	std::forward_list<Component*> components;

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
	std::vector<EntityData>::iterator pos;

	/** Constructs an entity object to handle the given entity data. */
	Entity(std::vector<EntityData>::iterator p)
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
		(*pos).components.push_front(comp);
		(*pos).componentHash.emplace_front(typeid(T).hash_code());
		return comp;
	}

	/**
	 * Removes a component of the given type from the entity.
	 */
	template<class T>
	void remove(void) {
		static_assert(std::is_convertible<T*, Component*>::value,
			"components must inherit Component base class");
		auto it = (*pos).componentHash.begin();
		auto ic = (*pos).components.before_begin();
		for (; it != (*pos).componentHash.end(); ic++, it++) {
			if (*it == typeid(T).hash_code()) {
				(*pos).components.erase_after(ic);
				break;
			}
		}
		(*pos).componentHash.remove(typeid(T).hash_code());
	}

	/**
	 * Tests if the entity has a component of the given type.
	 * @return true if the entity has the component
	 */
	template<class T>
	bool hasComponent(void) const {
		static_assert(std::is_convertible<T*, Component*>::value,
			"components must inherit Component base class");
		return std::binary_search((*pos).componentHash.begin(), (*pos).componentHash.end(), typeid(T).hash_code());
	}

	/**
	 * Fetches a component from the entity.
	 * @return the component, nullptr if the entity does not have it
	 */
	template<class T>
	T* component(void) {
		static_assert(std::is_convertible<T*, Component*>::value,
			"components must inherit Component base class");
		auto it = (*pos).componentHash.begin();
		auto ic = (*pos).components.begin();
		for (; it != (*pos).componentHash.end(); ic++, it++) {
			if (*it == typeid(T).hash_code())
				return dynamic_cast<T*>(*ic);
		}
		return nullptr;
	}
};

/**
 * @class EntityManager
 * Manages a group of entities.
 */
class EntityManager {
private:
	/** The array of all entities. */
	std::vector<EntityData> entities;

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

	/**
	 * Runs a function through all entities with the given components.
	 * @param f the function to run through
	 */
	template<class T1>
	void each(std::function<void(Entity e)> f) {
		for (auto i = entities.begin(); i < entities.end(); ++i) {
			Entity en (i);
			if (en.hasComponent<T1>())
				f(en);
		}
	}

	template<class T1, class T2>
	void each(std::function<void(Entity e)> f) {
		for (auto i = entities.begin(); i < entities.end(); ++i) {
			Entity en (i);
			if (en.hasComponent<T1>() && en.hasComponent<T2>())
				f(en);
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
	std::map<size_t, System*> systems;
	EntityManager& entities;

public:
	SystemManager(EntityManager& em)
		: entities(em) {}

	template<class T, typename... Args>
	void add(Args... args) {
		static_assert(std::is_convertible<T*, System*>::value,
			"systems must inherit System base class");
		systems.try_emplace(typeid(T).hash_code(), new T(args...));
	}

	template<class T>
	void update(DeltaTime dt) {
		static_assert(std::is_convertible<T*, System*>::value,
			"systems must inherit System base class");
		systems.at(typeid(T).hash_code())->update(entities, dt);
	}
};

#endif // ENTITIES_HPP_
