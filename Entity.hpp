#pragma once
#include "Component.hpp"
#include <iostream>

class Entity {
	friend class EntityManager;

	std::string m_tag{ "default" };
	bool m_active{ true };
	size_t m_id{ 0 };

	//constrostor and destructor
	Entity(const size_t m_id, const std::string m_tag);
public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CLifeSpan> cLifeSpan; 

	bool isActive() const;
	void destory();
	const std::string& tag() const;
	const size_t id() const;

};
