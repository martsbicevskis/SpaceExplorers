#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Bullet
{
public:
	Bullet(float speed, float size, sf::Vector2f position, sf::Vector2f targetLocation);

	static void update(float deltaTime);
	static void drawAll(sf::RenderWindow& window);
	static void checkRemove(const sf::RenderWindow& window);
	static void checkCollisions(const sf::RenderWindow& window);
	static void hitRemove();
	static float trySpawn(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime);
	void move(float deltaTime);
	void draw(sf::RenderWindow& window);
	bool checkInsideBounds(const sf::RenderWindow& window) const;

public:
	static std::vector<Bullet> bulletList;
	sf::Vector2f moveDistance;
	sf::RectangleShape body;

private:
	float speed;
	float size;
	sf::Vector2f position;
	sf::Vector2f targetLocation;

};
