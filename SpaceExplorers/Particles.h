#pragma once

#include "Game.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Particles
{
	Particles(sf::RectangleShape body, sf::Vector2f position, sf::Vector2f size, sf::Texture texture, float expireTime);

public:
	static std::vector<Particles> particleList;
	static void drawAll(sf::RenderWindow& window);
	static void updateExpireTime(float deltaTime);
	static void remove();
	void draw(sf::RenderWindow& window);

private:
	sf::RectangleShape body;
	sf::Vector2f position;
	sf::Vector2f size;
	sf::Texture texture;
	float expireTime;
};

