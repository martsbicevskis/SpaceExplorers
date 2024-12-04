#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Bullet
{
public:
	//constructor
    Bullet(float speed, float size, sf::Vector2f position, sf::Vector2f targetLocation);

	//Bullet functions
    static void update(float deltaTime);
    static void drawAll(sf::RenderWindow& window);
    static void checkRemove(const sf::RenderWindow& window);
    static void checkCollisions(const sf::RenderWindow& window);
    static int hitRemove();
    static float trySpawn(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime);
    void move(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool checkInsideBounds(const sf::RenderWindow& window) const;

public:
	//global variables
    static std::vector<Bullet> bulletList;
    sf::Vector2f moveDistance;
    sf::CircleShape body;

private:
	//Bullet variables
    float speed;
    float size;
    sf::Vector2f position;
    sf::Vector2f targetLocation;
	sf::Texture bulletTexture;
};
