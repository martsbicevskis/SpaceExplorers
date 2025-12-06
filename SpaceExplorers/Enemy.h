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
    static float trySpawn(float spawnTimer, float spawnTimerMax, float deltaTime, int enemiesPerWave, float difficulty, float gameTime);
    static void update(float deltaTime, sf::Vector2f playerPosition);
    static void drawAll(sf::RenderWindow& window);
    static int hitRemove();
    static float checkPlayerTouch(sf::RectangleShape player, float playerHealth);
	static void manaAbilityDamage(sf::Vector2f playerPosition);
    void modifySpeed(float multiplier);
    void draw(sf::RenderWindow& window);
    void drawHealthBar(sf::RenderWindow& window); 
    void takeDamage(float damage);
    static void setTexture(const sf::Texture& texture);
    bool isBossEnemy() const;
    

public:
	//global variables
    static std::vector<Enemy> enemyList;
    static std::vector<Enemy> enemyBossList;
	static float abilityDamageDistance;
    sf::RectangleShape body;

private:
	//Enemy variables
    float speed;
    float size;
    sf::Vector2f location;
    float health;
    float maxHealth; 
    static const sf::Texture* s_texture;   
    bool isBoss;
};
