#include "EntityManger.hpp"
//#include "Entity.hpp"
#include <unordered_set> 
#include <iostream>
#include <memory>

void EntityManager::update() {
    std::unordered_set<size_t> existingIds;
    for (const auto& entity : m_Entities) {
        existingIds.insert(entity->id());
    }

    for (const auto& entityToAdd : m_entitiesToAdd) {
        size_t id = entityToAdd->id();
        if (existingIds.find(id) == existingIds.end()) {
            m_Entities.push_back(entityToAdd);
            m_entityMap[entityToAdd->tag()].push_back(entityToAdd);
            existingIds.insert(id);
        }
    }
	removeDeadEntity(m_Entities);

	//for (auto iterator : m_entityMap) {
	//	std::cout << iterator.first << "   " << iterator.second[0] << "\n";
 //	}
}

std::shared_ptr<Entity> EntityManager::addEntity(std::string tag) {
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}

void EntityManager::removeDeadEntity(EntityVec& eVec) {
	eVec.erase(std::remove_if(eVec.begin(), eVec.end(),
		[](const std::shared_ptr<Entity>& entity) {
			return !entity->isActive(); 
		}),
		eVec.end());
}

const EntityVec& EntityManager::getEntities() const {
    return m_Entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag) const {
    auto it = m_entityMap.find(tag);
    if (it != m_entityMap.end()) {
        return it->second;
    }
    static EntityVec empty;
    return empty;
}

const EntityMap& EntityManager::getEntityMap() const {
    return m_entityMap;
}