#pragma once
#include <SFML\Graphics.hpp>

#include <iostream>

class CTransform {
public:
	sf::Vector2f  vector;
	sf::Vector2f velocity;
	float angle;
	sf::Vector2f initialPos;
	CTransform(sf::Vector2f position, sf::Vector2f velocity, float angle):
		vector(position), velocity(velocity), angle(angle), initialPos(position) {};
};


class CShape {
public:
	sf::CircleShape cricle;
	CShape(float radius, sf::Vector2f position, int outLine, sf::Color color, sf::Color outLineColor) {

		cricle.setRadius(radius);
		cricle.setFillColor(color);
		cricle.setOutlineThickness(outLine);
		cricle.setOutlineColor(outLineColor);
		cricle.setPosition(position);
	}
};

class CCollision {
public:
	float radius{ 0 };
	CCollision(float r): radius(r) {}
};

class CInput { 
public:
	bool up{ false };
	bool down{ false };
	bool left{ false };
	bool right{ false }; 

	CInput() {}
 };

class CScore {
public:
	int score{ 0 };
	CScore(int score) : score(score) {}
};

class CLifeSpan {
public:
	int lifeSpan{ 0 };
	int remaining{ 0 };

	CLifeSpan(int total): lifeSpan(total), remaining(total){}
};