#include "Game.h"
#include "Enemy.h"
#include <algorithm>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

std::vector<Enemy> Enemy::enemyList;
int Enemy::enemiesPerWave = 5; // Initialize the number of enemies per wave

Enemy::Enemy(float speed, float size, sf::Vector2f location, float health)
    : speed(speed), size(size), location(location), health(health), maxHealth(health)
{
    body.setSize(sf::Vector2f(size, size));
    body.setPosition(location);
    if (!enemyTexture.loadFromFile("enemy.png")) {
        std::cerr << "Failed to load enemy texture!" << std::endl;
    }
	body.setTexture(&enemyTexture);
    enemyList.push_back(*this);
}

float Enemy::trySpawn(float spawnTimer, float spawnTimerMax, float deltaTime)
{
    if (spawnTimer >= spawnTimerMax)
    {
        for (int i = 0; i < enemiesPerWave; ++i)
        {
            // Randomly choose one of the four edges of the screen
            int edge = rand() % 4;
            sf::Vector2f spawnPosition;

            switch (edge)
            {
            case 0: // Top edge
                spawnPosition = sf::Vector2f(static_cast<float>(rand() % Game::SCREEN_WIDTH), -20.f);
                break;
            case 1: // Bottom edge
                spawnPosition = sf::Vector2f(static_cast<float>(rand() % Game::SCREEN_WIDTH), Game::SCREEN_HEIGHT + 20.f);
                break;
            case 2: // Left edge
                spawnPosition = sf::Vector2f(-20.f, static_cast<float>(rand() % Game::SCREEN_HEIGHT));
                break;
            case 3: // Right edge
                spawnPosition = sf::Vector2f(Game::SCREEN_WIDTH + 20.f, static_cast<float>(rand() % Game::SCREEN_HEIGHT));
                break;
            }

            new Enemy(25 + rand() % 50, 20, spawnPosition, 10 + rand() % 30);
        }
        return -spawnTimerMax;
    }
    else
    {
        return deltaTime;
    }
}

void Enemy::update(float deltaTime, sf::Vector2f playerPosition)
{
    for (auto& e : enemyList)
    {
        if (e.body.getPosition().x > playerPosition.x)
            e.body.move(deltaTime * -(e.speed), 0.f);
        if (e.body.getPosition().x < playerPosition.x)
            e.body.move(deltaTime * (e.speed), 0.f);
        if (e.body.getPosition().y > playerPosition.y)
            e.body.move(0.f, deltaTime * -(e.speed));
        if (e.body.getPosition().y < playerPosition.y)
            e.body.move(0.f, deltaTime * (e.speed));
    }
}

int Enemy::hitRemove()
{
    int removedCount = 0;

    auto it = std::remove_if(enemyList.begin(), enemyList.end(),
        [&removedCount](const Enemy& enemy) {
            if (enemy.body.getFillColor() == sf::Color::Transparent) {
                ++removedCount;
                return true;
            }
            return false;
        });
    enemyList.erase(it, enemyList.end());

	return removedCount;
}


float Enemy::checkPlayerTouch(sf::RectangleShape player, float playerHealth)
{
    float damage = 0.f;
    for (auto& e : enemyList)
    {
        if (e.body.getPosition().x + e.body.getSize().x > player.getPosition().x &&
            e.body.getPosition().x < player.getPosition().x + player.getSize().x &&
            e.body.getPosition().y + e.body.getSize().y > player.getPosition().y &&
            e.body.getPosition().y < player.getPosition().y + player.getSize().y)
        {
            damage += 0.1f;
        }
    }
    return damage;
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(body);
    drawHealthBar(window); // Draw the health bar
}

void Enemy::drawAll(sf::RenderWindow& window)
{
    for (auto& e : enemyList)
    {
        e.draw(window);
    }
}

void Enemy::drawHealthBar(sf::RenderWindow& window)
{
    sf::RectangleShape healthBar;
    healthBar.setSize(sf::Vector2f(size * (health / maxHealth), 5)); // Scale the health bar based on health
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(body.getPosition().x, body.getPosition().y - 10); // Position above the enemy
    window.draw(healthBar);
}

void Enemy::takeDamage(float damage)
{
    health -= damage;
    if (health <= 0)
    {
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::Transparent);
    }
}

void Enemy::manaAbilityDamage()
{
	for (auto& e : enemyList)
	{
		e.takeDamage(50.0f);
	}
}
