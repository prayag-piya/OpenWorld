#pragma once

#include "Entity.hpp"
#include <iostream>
#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<const std::string, EntityVec> EntityMap;

class EntityManager {
	EntityVec m_Entities;
	EntityVec m_entitiesToAdd;
	EntityMap m_entityMap;
	int m_totalEntities{ 0 }; 


	void removeDeadEntity(EntityVec& eVec);

public:
	//EntityManager();
	
	void update();

	std::shared_ptr<Entity> addEntity(const std::string tag);
	const EntityMap& getEntityMap() const;
	const EntityVec& getEntities() const;
	const EntityVec& getEntities(const std::string& tag) const;



};