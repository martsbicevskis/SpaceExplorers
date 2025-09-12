#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Enemy
{
public:

	//constructor
    Enemy(float speed, float size, sf::Vector2f location, float health = 100.0f);

	//Enemy functions
    static float trySpawn(float spawnTimer, float spawnTimerMax, int enemiesPerWave, float deltaTime, float gameTime);
    static void update(float deltaTime, sf::Vector2f playerPosition);
    static void drawAll(sf::RenderWindow& window);
    static int hitRemove();
    static float checkPlayerTouch(sf::RectangleShape player, float playerHealth);
	static void manaAbilityDamage(sf::Vector2f playerPosition);
    void draw(sf::RenderWindow& window);
    void drawHealthBar(sf::RenderWindow& window); 
    void takeDamage(float damage);

public:
	//global variables
    static std::vector<Enemy> enemyList;
	static float abilityDamageDistance;
    sf::RectangleShape body;

private:
	//Enemy variables
    float speed;
    float size;
    sf::Vector2f location;
    float health;
    float maxHealth; 
    sf::Texture enemyTexture;
};


