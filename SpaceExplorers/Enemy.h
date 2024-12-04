#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Enemy
{
public:

	//constructor
    Enemy(float speed, float size, sf::Vector2f location, float health = 100.0f, bool isBoss = false);

	//Enemy functions
    static float trySpawn(float spawnTimer, float spawnTimerMax, float deltaTime);
    static void update(float deltaTime, sf::Vector2f playerPosition);
    static void drawAll(sf::RenderWindow& window);
    static int hitRemove();
    static float checkPlayerTouch(sf::RectangleShape player, float playerHealth);
	static void manaAbilityDamage();
	static bool isBossAlive();
    void draw(sf::RenderWindow& window);
    void drawHealthBar(sf::RenderWindow& window); 
    void takeDamage(float damage);

public:
	//global variables
    static std::vector<Enemy> enemyList;
    static int enemiesPerWave; 
    sf::RectangleShape body;

private:
	//Enemy variables
    float speed;
    float size;
    sf::Vector2f location;
    float health;
    float maxHealth; 
    sf::Texture enemyTexture;
    bool isBoss;
};


