#include "Game.hpp"


#include <iostream>
#include <cstdlib>

#define MAX_DISTANCE 300.0f

Game::Game(int height, int width) {
	window.create(sf::VideoMode(height, width), "Open World");

	Game::player playerConfig{
		sf::Vector2f(height / 2, width / 2),
		sf::Vector2f(2, 2),
		sf::Color::Green,
		sf::Color::Red,
		0.0f
	};

	mainCharacter = m_entities.addEntity("Player");
	mainCharacter->cInput = std::make_shared<CInput>();
	mainCharacter->cTransform = std::make_shared<CTransform>(playerConfig.position, playerConfig.velocity, playerConfig.angle);
	mainCharacter->cShape = std::make_shared<CShape>(50.0f, playerConfig.position, 5, playerConfig.FillColor, playerConfig.OutLineColor);
	mainCharacter->cScore = std::make_shared<CScore>(0);
	mainCharacter->cLifeSpan = std::make_shared<CLifeSpan>(100);
	mainCharacter->cCollision = std::make_shared<CCollision>(50.0f);

	EnemySpawner();
}

// Keyboard input system
void Game::GameControl(sf::Keyboard::Scancode key) {
	if(key == sf::Keyboard::Scancode(sf::Keyboard::D)) {
		mainCharacter->cInput->right = true;
	}
	if (key == sf::Keyboard::Scancode(sf::Keyboard::A)) {
		mainCharacter->cInput->left = true;
	}
	if (key == sf::Keyboard::Scancode(sf::Keyboard::W)) {
		mainCharacter->cInput->up = true;
	}
	if (key == sf::Keyboard::Scancode(sf::Keyboard::S)) {
		mainCharacter->cInput->down = true;
	}
}


// Input Relase function 
void Game::GameControlRelease(sf::Keyboard::Scancode key) {
	if (key == sf::Keyboard::Scancode(sf::Keyboard::D)) {
		mainCharacter->cInput->right = false;
	}
	if (key == sf::Keyboard::Scancode(sf::Keyboard::A)) {
		mainCharacter->cInput->left = false;
	}
	if (key == sf::Keyboard::Scancode(sf::Keyboard::W)) {
		mainCharacter->cInput->up = false;
	}
	if (key == sf::Keyboard::Scancode(sf::Keyboard::S)) {
		mainCharacter->cInput->down = false;
	}
}

sf::Vector2f Normalize(const sf::Vector2f& vector) {
	float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (magnitude != 0) {
		return sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
	}
	return vector;
	}

// Charater Movement Function 
void Game::Movement() {
	mainCharacter->cTransform->velocity = sf::Vector2f(0, 0);
	if (mainCharacter->cInput->up) {
		mainCharacter->cTransform->velocity.y = -2;
	}
	if (mainCharacter->cInput->down) {
		mainCharacter->cTransform->velocity.y = 2;
	}
	if (mainCharacter->cInput->left) {
		mainCharacter->cTransform->velocity.x = -2;
	}
	if (mainCharacter->cInput->right) {
		mainCharacter->cTransform->velocity.x = 2;
	}

	if (mainCharacter->cTransform->velocity != sf::Vector2f(0, 0)) {
		mainCharacter->cTransform->velocity = Normalize(mainCharacter->cTransform->velocity);
		float speed = 2.0f;
		mainCharacter->cTransform->velocity = mainCharacter->cTransform->velocity * speed;
	}

	mainCharacter->cTransform->vector += mainCharacter->cTransform->velocity;


	
	//BulletMovement(mousePosition, bullets);
}


bool CollisionCalculator(std::shared_ptr<Entity>& character, std::shared_ptr<Entity>& collisionEntity) {
	// Get the bounding boxes of the character and the collision entity
	sf::FloatRect boundMainCharacter = character->cShape->cricle.getGlobalBounds();
	sf::FloatRect boundEntityCharacter = collisionEntity->cShape->cricle.getGlobalBounds();

	// Calculate the center points of the character and collision entity
	sf::Vector2f centerPoint(boundMainCharacter.left + boundMainCharacter.width / 2,
		boundMainCharacter.top + boundMainCharacter.height / 2);

	sf::Vector2f centerPointEntity(boundEntityCharacter.left + boundEntityCharacter.width / 2,
		boundEntityCharacter.top + boundEntityCharacter.height / 2);

	// Calculate the delta between the center points
	sf::Vector2f delta = centerPoint - centerPointEntity;

	// Calculate the overlap on the X and Y axes
	float overlapX = (boundMainCharacter.width / 2) + (boundEntityCharacter.width / 2) - std::abs(delta.x);
	float overlapY = (boundMainCharacter.height / 2) + (boundEntityCharacter.height / 2) - std::abs(delta.y);

	// If there's overlap on both axes, there is a collision
	if (overlapX > 0 && overlapY > 0) {
		return true;
	}

	return false;
}



void Game::SpawnBullet() {
	// Set the bullet's speed
	float bulletSpeed = 5.0f;

	// Get the mouse position at the time of firing
	mousePosition = sf::Mouse::getPosition(window);
	// Create a new bullet entity
	std::shared_ptr<Entity> bullet = m_entities.addEntity("Bullet");

	// Set the initial bullet position
	sf::Vector2f bulletPosition = sf::Vector2f(mainCharacter->cTransform->vector.x + 50, mainCharacter->cTransform->vector.y + 50);

	// Calculate the direction vector from the bullet's position to the mouse position
	sf::Vector2f direction = sf::Vector2f(mousePosition.x - bulletPosition.x, mousePosition.y - bulletPosition.y);
	// Normalize the direction vector
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	sf::Vector2f normalizedDirection = sf::Vector2f(direction.x / length, direction.y / length);

	// Set the bullet's velocity based on the normalized direction and speed
	sf::Vector2f velocity = sf::Vector2f(normalizedDirection.x * bulletSpeed, normalizedDirection.y * bulletSpeed);

	// Assign the transform and velocity to the bullet
	bullet->cTransform = std::make_shared<CTransform>(bulletPosition, velocity, 0.0f);
	bullet->cShape = std::make_shared<CShape>(10.0f, bulletPosition, 1, sf::Color::White, sf::Color::White);
}



void Game::Collision() {
	//This Will the game collision, Which will handle any kind of collision
	//Game Window Gaurd
	for (auto e : m_entities.getEntities("Player")) {
		// Constrain the entity's x position
		if (e->cTransform->vector.x < e->cShape->cricle.getRadius()) {
			e->cTransform->vector.x = e->cShape->cricle.getRadius();
		}
		if (e->cTransform->vector.x > (2460 - e->cShape->cricle.getRadius())) {
			e->cTransform->vector.x = (2460 - e->cShape->cricle.getRadius());
		}

		// Constrain the entity's y position
		if (e->cTransform->vector.y < e->cShape->cricle.getRadius()) {
			e->cTransform->vector.y = e->cShape->cricle.getRadius();
		}
		if (e->cTransform->vector.y > (1280 - e->cShape->cricle.getRadius())) {
			e->cTransform->vector.y = (1280 - e->cShape->cricle.getRadius()); 
		}
	}




	//comment line below is the implementation of the intersects methods
	/*sf::FloatRect globalBound = mainCharacter->cShape->cricle.getGlobalBounds();
		//if (globalBound.intersects(enemy->cShape->cricle.getGlobalBounds())) {
		//	mainCharacter->cTransform->vector = sf::Vector2f(2460 / 2, 1280 / 2);
		}*/
	EntityVec enemies = m_entities.getEntities("Enemy");
	for (auto enemy : enemies) {
		bool collide = CollisionCalculator(mainCharacter, enemy);
		if (collide) {
			mainCharacter->cTransform->vector = sf::Vector2f(2460 / 2, 1280 / 2);
		}

	}
}

double calculateDistance(std::shared_ptr<Entity>& bullet) {
	sf::Vector2f changedPosition = bullet->cTransform->vector - bullet->cTransform->initialPos;
	return std::sqrt(changedPosition.x * changedPosition.x + changedPosition.y * changedPosition.y);
}

//Ememy Spawner 
void Game::EnemySpawner() {
	std::shared_ptr<Entity> enemy = m_entities.addEntity("Enemy");
	enemy->cTransform = std::make_shared<CTransform>(sf::Vector2f(100, 100), sf::Vector2f(2, 2), 2.0f);
	enemy->cLifeSpan = std::make_shared<CLifeSpan>(100);
	enemy->cShape = std::make_shared<CShape>(50.0f, sf::Vector2f(100, 100), 5, sf::Color::Blue, sf::Color::Green);

}

//Game rendering system
void Game::Render() {
	window.clear(sf::Color::Black);
	float const t = static_cast<float>(clock.getElapsedTime().asMilliseconds());
	const EntityVec& entity =  Game::m_entities.getEntities();
	for (auto e : entity) {
		e->cShape->cricle.setPosition(e->cTransform->vector.x, e->cTransform->vector.y);
		
		e->cTransform->angle += 1.0f;
		e->cShape->cricle.setRotation(e->cTransform->angle);
		e->cShape->cricle.setOrigin(sf::Vector2f(50, 50));

		window.draw(e->cShape->cricle);
	}

	EntityVec bullets = m_entities.getEntities("Bullet");
	for (auto b : bullets) {
		b->cTransform->vector += b->cTransform->velocity;
		double distanceTraveled = calculateDistance(b);
		if (distanceTraveled > MAX_DISTANCE) {
			b->destory();
		}

	}

	EntityVec enemies = m_entities.getEntities("Enemy");

	for (auto e : enemies) {
		// Update enemy position based on its velocity
		e->cTransform->vector += e->cTransform->velocity;

		// Check collision with window edges
		if (e->cTransform->vector.x <= e->cShape->cricle.getRadius() || e->cTransform->vector.x >= (window.getSize().x - e->cShape->cricle.getRadius())) {
			e->cTransform->velocity.x *= -1; // Reverse x velocity
		}

		if (e->cTransform->vector.y <= e->cShape->cricle.getRadius() || e->cTransform->vector.y >= (window.getSize().y - e->cShape->cricle.getRadius())) {
			e->cTransform->velocity.y *= -1; // Reverse y velocity
		}
	}
	
	window.display();
}


// Main Game Loop
void Game::GameLoop() {
	window.setFramerateLimit(75);
	while (m_running) {
		
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				m_running = false;
			}
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				m_running = false;
				break;
			case sf::Event::KeyPressed:
				Game::GameControl(event.key.scancode);
				break;
			case sf::Event::KeyReleased:
				Game::GameControlRelease(event.key.scancode);
				break;
			case sf::Event::MouseButtonPressed:
				if (sf::Event::MouseLeft) {
					SpawnBullet();
				}
			default:
				break;
			}
		}
		Movement();
		Collision();
		m_entities.update();
		Game::Render();
		
	}
}