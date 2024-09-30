#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Enemy
{
public:
	Enemy(float speed, float size, sf::Vector2f location);

	static float trySpawn(float spawnTimer, float spawnTimerMax, float deltaTime);
	static void update(float deltaTime, sf::Vector2f position);
	static void drawAll(sf::RenderWindow& window);
	static void hitRemove();
	static void checkPlayerTouch(sf::RectangleShape player);
	void draw(sf::RenderWindow& window);

public:
	static std::vector<Enemy> enemyList;
	sf::RectangleShape body;

private:
	float speed;
	float size;
	sf::Vector2f location;
};

