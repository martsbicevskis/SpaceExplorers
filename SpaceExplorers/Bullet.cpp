#include "Bullet.h"
#include "Enemy.h"
#include <cstdlib>
#include <iostream>

std::vector<Bullet> Bullet::bulletList;

Bullet::Bullet(float speed, float size, sf::Vector2f position, sf::Vector2f targetLocation)
{
    this->speed = speed;
    this->size = size;
    this->position = position;
    this->targetLocation = targetLocation;

    body.setRadius(size);
    body.setPosition(position);
	body.setFillColor(sf::Color::Green);
	body.setOutlineThickness(2);
	body.setOutlineColor(sf::Color::Cyan);

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

void Bullet::checkRemove(const sf::RenderWindow& window)
{
    bulletList.erase(std::remove_if(bulletList.begin(), bulletList.end(),
        [&window](const Bullet& bullet)
        {
            return (bullet.checkInsideBounds(window));
        }),
        bulletList.end());
}

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
                e.takeDamage(10.0f); // Apply damage to the enemy
                b.body.setFillColor(sf::Color::Transparent);
                b.body.setOutlineColor(sf::Color::Transparent);
            }
        }
    }

}

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

float Bullet::trySpawn(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime)
{
    if (bulletSpawnTimer >= bulletSpawnTimerMax)
    {
        new Bullet(1000.f, 10.f, sf::Vector2f(playerLocation), static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
        return -bulletSpawnTimer;
    }
    else
    {
        return deltatime;
    }
}

bool Bullet::checkInsideBounds(const sf::RenderWindow& window) const
{
    sf::Vector2f position = body.getPosition();

    return (position.x < 0 || position.x > window.getSize().x ||
        position.y < 0 || position.y > window.getSize().y);
}
