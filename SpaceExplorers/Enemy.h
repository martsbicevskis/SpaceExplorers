#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Enemy
{
public:
    Enemy(float speed, float size, sf::Vector2f location, float health = 100.0f);

    static float trySpawn(float spawnTimer, float spawnTimerMax, float deltaTime);
    static void update(float deltaTime, sf::Vector2f playerPosition);
    static void drawAll(sf::RenderWindow& window);
    static void hitRemove();
    static float checkPlayerTouch(sf::RectangleShape player, float playerHealth);
    void draw(sf::RenderWindow& window);
    void drawHealthBar(sf::RenderWindow& window); // New method to draw health bar
    void takeDamage(float damage);

public:
    static std::vector<Enemy> enemyList;
    static int enemiesPerWave; // New static member for the number of enemies per wave
    sf::RectangleShape body;

private:
    float speed;
    float size;
    sf::Vector2f location;
    float health;
    float maxHealth; // Store the maximum health for scaling the health bar
};


