#pragma once
#include "EntityManger.hpp"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

class Game {
public:
	sf::RenderWindow window;
	sf::Clock m_deltaClock;
	EntityManager m_entities;
	sf::Clock clock; 

	Game(int height, int width);

	void GameLoop();

	void GameControl(sf::Keyboard::Scan::Scancode key);
	void GameControlRelease(sf::Keyboard::Scan::Scancode key);

	void Render();

	void Movement();

	void Collision();

	void EnemySpawner();

	void GUI();

	void SpawnBullet();

private:
	struct player {
		sf::Vector2f position;
		sf::Vector2f velocity;
		sf::Color FillColor;
		sf::Color OutLineColor;
		float angle;
	};
	bool m_running{ true };
	std::shared_ptr<Entity> mainCharacter;
	sf::Vector2f centerPoint;
	sf::Vector2i mousePosition;
};