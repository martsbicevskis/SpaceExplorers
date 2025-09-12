#include "Game.h"
#include "Enemy.h"
#include <algorithm>
#include <cstdlib> 
#include <ctime>  

std::vector<Enemy> Enemy::enemyList;
float Enemy::abilityDamageDistance = 300.f;

//constructor
Enemy::Enemy(float speed, float size, sf::Vector2f location, float health)
	: speed(speed), size(size), location(location), health(health), maxHealth(health)
{
    body.setSize(sf::Vector2f(size, size));
    body.setPosition(location);
    if (!enemyTexture.loadFromFile("orangeEnemy.png")) {
        std::cerr << "Failed to load enemy texture!" << std::endl;
    }
	body.setTexture(&enemyTexture);
    enemyList.push_back(*this);
}


//updating the enemy spawn timer and spawning enemies at randomized locations
float Enemy::trySpawn(float spawnTimer, float spawnTimerMax, float deltaTime, int enemiesPerWave)
{

    if (spawnTimer >= spawnTimerMax)
    {
        for (int i = 0; i < enemiesPerWave; ++i)
        {
            int edge = rand() % 4;
            sf::Vector2f spawnPosition;
            
            switch (edge)
            {
            case 0: 
                spawnPosition = sf::Vector2f(static_cast<float>(rand() % Game::SCREEN_WIDTH), -20.f);
                break;
            case 1: 
                spawnPosition = sf::Vector2f(static_cast<float>(rand() % Game::SCREEN_WIDTH), Game::SCREEN_HEIGHT + 20.f);
                break;
            case 2: 
                spawnPosition = sf::Vector2f(-20.f, static_cast<float>(rand() % Game::SCREEN_HEIGHT));
                break;
            case 3:
                spawnPosition = sf::Vector2f(Game::SCREEN_WIDTH + 20.f, static_cast<float>(rand() % Game::SCREEN_HEIGHT));
                break;
            }

            new Enemy(20 + rand() % 2, 20, spawnPosition, 1 + rand() % 2);
        }
        return -spawnTimerMax;
    }
    else
    {
        return deltaTime;
    }
}

//moving enemies towards the player
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

//removing enemies if they are hit
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

//checking if an enemy has touched the player
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

//drawing the enemies
void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(body);
    drawHealthBar(window);
}

void Enemy::drawAll(sf::RenderWindow& window)
{
    for (auto& e : enemyList)
    {
        e.draw(window);
    }
}

//drawing enemy health bar
void Enemy::drawHealthBar(sf::RenderWindow& window)
{
    sf::RectangleShape healthBar;
    healthBar.setSize(sf::Vector2f(size * (health / maxHealth), 5)); 
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(body.getPosition().x, body.getPosition().y - 10);
    window.draw(healthBar);
}

//applying damage to the enemy
void Enemy::takeDamage(float damage)
{
    health -= damage;
    if (health <= 0)
    {
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::Transparent);
    }
}

//applying mana ability damage to all enemies
void Enemy::manaAbilityDamage(sf::Vector2f playerPosition)
{

	for (auto& e : enemyList)
	{
		float xDis = playerPosition.x - e.body.getPosition().x;
		float yDis = playerPosition.y - e.body.getPosition().y;

        if (sqrt(xDis * xDis + yDis * yDis) < abilityDamageDistance)
        {
			e.takeDamage(50.0f);
        }
	}
}
