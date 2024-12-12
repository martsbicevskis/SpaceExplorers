#include "Bullet.h"
#include "Enemy.h"
#include "Game.h"
#include <cstdlib>
#include <iostream>

//global bullet list
std::vector<Bullet> Bullet::bulletList;

//constructor
Bullet::Bullet(float speed, float size, sf::Vector2f position, sf::Vector2f targetLocation, bool isExplosive)
{
    this->speed = speed;
    this->size = size;
    this->position = position;
    this->targetLocation = targetLocation;
    this->isExplosive = isExplosive;

    body.setRadius(size);
    body.setPosition(position);
	body.setFillColor(sf::Color::Green);
	body.setOutlineThickness(2);
	body.setOutlineColor(sf::Color::Cyan);

	//calculate the direction for bullets
    float xMoveDistance = targetLocation.x - (size / 2) - position.x;
    float yMoveDistance = targetLocation.y - (size / 2) - position.y;

    if (abs(xMoveDistance) > abs(yMoveDistance) && xMoveDistance != 0)
    {
        yMoveDistance /= abs(xMoveDistance);
        xMoveDistance /= abs(xMoveDistance);
    }
    else if (yMoveDistance != 0)
    {
        xMoveDistance /= abs(yMoveDistance);
        yMoveDistance /= abs(yMoveDistance);
    }
    moveDistance = sf::Vector2f(xMoveDistance, yMoveDistance);

    bulletList.push_back(*this);
}

//moving the bullets
void Bullet::update(float deltaTime)
{
    for (auto& b : bulletList)
    {
        b.move(deltaTime);
    }
}



void Bullet::move(float deltaTime)
{
    body.move(moveDistance * deltaTime * speed);
}

//drawing the bullets
void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(body);
}

void Bullet::drawAll(sf::RenderWindow& window)
{
    for (auto& b : Bullet::bulletList)
    {
        b.draw(window);
    }
}

//removing bullets that are outside the window
void Bullet::checkRemove(const sf::RenderWindow& window)
{
    bulletList.erase(std::remove_if(bulletList.begin(), bulletList.end(),
        [&window](const Bullet& bullet)
        {
            return (bullet.checkInsideBounds(window));
        }),
        bulletList.end());
}

//checking for collisions with enemies
void Bullet::checkCollisions(const sf::RenderWindow& window)
{
    for (auto& e : Enemy::enemyList)
    {
        for (auto& b : Bullet::bulletList)
        {
            float xDistance = e.body.getPosition().x - b.body.getPosition().x;
            float yDistance = e.body.getPosition().y - b.body.getPosition().y;

            if ((xDistance < b.body.getRadius() * 2 && xDistance > -e.body.getSize().x) &&
                (yDistance < b.body.getRadius() * 2 && yDistance > -e.body.getSize().y))
            {
                e.takeDamage(10.0f);
                if (b.isExplosive) explode(sf::Vector2f(b.body.getPosition().x + b.body.getRadius(), b.body.getPosition().y + b.body.getRadius()));
                b.body.setFillColor(sf::Color::Transparent);
                b.body.setOutlineColor(sf::Color::Transparent);
            }
        }
    }

}

//removing bullets if they hit enemies
int Bullet::hitRemove()
{
    int removedCount = 0;

    auto it = std::remove_if(bulletList.begin(), bulletList.end(),
        [&removedCount](const Bullet& bullet) {
            if (bullet.body.getFillColor() == sf::Color::Transparent) {
                ++removedCount;
                return true;
            }
            return false;
        });
    bulletList.erase(it, bulletList.end());

    return removedCount;
}

//updating the bullet spawn timer and spawning bullets
float Bullet::trySpawnRapid(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime, bool isExplosive)
{
    if (bulletSpawnTimer >= bulletSpawnTimerMax)
    {
        new Bullet(1000.f, 10.f, sf::Vector2f(playerLocation), static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)), isExplosive);
        return -bulletSpawnTimer;
    }
    else
    {
        return deltatime;
    }
}

float Bullet::trySpawnShotgun(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime, bool isExplosive)
{
    if (bulletSpawnTimer >= bulletSpawnTimerMax)
    {
        new Bullet(1000.f, 10.f, sf::Vector2f(playerLocation), sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y), isExplosive);
        new Bullet(1000.f, 10.f, sf::Vector2f(playerLocation), sf::Vector2f(sf::Mouse::getPosition(window).x - Game::SCREEN_WIDTH / 20, sf::Mouse::getPosition(window).y - Game::SCREEN_HEIGHT / 20), isExplosive);
        new Bullet(1000.f, 10.f, sf::Vector2f(playerLocation), sf::Vector2f(sf::Mouse::getPosition(window).x + Game::SCREEN_WIDTH / 20, sf::Mouse::getPosition(window).y + Game::SCREEN_HEIGHT / 20), isExplosive);

        return -bulletSpawnTimer;
    }
    else
    {
        return deltatime;
    }
}

float Bullet::trySpawnBomb(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime, bool isExplosive)
{
    if (bulletSpawnTimer >= bulletSpawnTimerMax)
    {
        new Bullet(1000.f, 10.f, sf::Vector2f(playerLocation), static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)), isExplosive);
        return -bulletSpawnTimer;
    }
    else
    {
        return deltatime;
    }
}

void Bullet::explode(sf::Vector2f location)
{
    for (auto& e : Enemy::enemyList)
    {

        float xDistance = e.body.getPosition().x + e.body.getSize().x / 2 - location.x;
        float yDistance = e.body.getPosition().y + e.body.getSize().y / 2 - location.y;

		if (sqrt(xDistance * xDistance + yDistance * yDistance) < 100)
		{
			e.takeDamage(20.0f);
		}

    }
}

//checking if the bullet is inside the window
bool Bullet::checkInsideBounds(const sf::RenderWindow& window) const
{
    sf::Vector2f position = body.getPosition();

    return (position.x < 0 || position.x > window.getSize().x ||
        position.y < 0 || position.y > window.getSize().y);
}
