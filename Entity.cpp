#include "Entity.hpp"

Entity::Entity(const size_t id, const std::string tag) : m_id(id), m_tag(tag) {}

bool Entity::isActive() const {
	return m_active;
}

const std::string& Entity::tag() const {
	return m_tag;
}

void Entity::destory() {
	m_active = false;
}
 
const size_t Entity::id() const{
	return m_id;
}