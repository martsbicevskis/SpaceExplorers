#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "enemy.h"

class Bullet
{
public:
	//constructor
    Bullet(float speed, float size, sf::Vector2f position, sf::Vector2f targetLocation, bool isExplosive, bool isPiercing);

	//Bullet functions
    static void update(float deltaTime);
    static void drawAll(sf::RenderWindow& window);
    static void checkRemove(const sf::RenderWindow& window);
    static int checkCollisions(const sf::RenderWindow& window);
    static void hitRemove();
    static void spawn(const sf::Vector2f& position, sf::RenderWindow& window, bool isTripleShotUpgradeBought, bool isExplosiveShotUpgradeBought, bool isPiercingShotUpgradeBought);

    static void explode(sf::Vector2f location);
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
    bool isExplosive;
    bool isPiercing;
    sf::Vector2f position;
    sf::Vector2f targetLocation;
	sf::Texture bulletTexture;
    std::vector<Enemy*> hitList;
};


/*
* 
* ------- OLD BULLET SPAWNING FUNCTIONS -------
*
    static float trySpawnRapid(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime);
    static float trySpawnShotgun(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime);
    static float trySpawnBomb(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime);
    static float trySpawnPiercing(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime);
    
*/