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

	body.setSize(sf::Vector2f(size, size));
	body.setPosition(position);
	body.setFillColor(sf::Color::Green);
	body.setOutlineThickness(2);
	body.setOutlineColor(sf::Color::Blue);


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

			if ((xDistance < b.body.getSize().x && xDistance > -e.body.getSize().x) &&
				(yDistance < b.body.getSize().y && yDistance > -e.body.getSize().y))
			{
				e.body.setFillColor(sf::Color::Transparent);
				e.body.setOutlineColor(sf::Color::Transparent);
				b.body.setFillColor(sf::Color::Transparent);
				b.body.setOutlineColor(sf::Color::Transparent);
			}

		}
	}
	Bullet::hitRemove();
	Enemy::hitRemove();
}

void Bullet::hitRemove()
{
	for (auto b : Bullet::bulletList)
	{
		Bullet::bulletList.erase(std::remove_if(bulletList.begin(), bulletList.end(),
			[](const Bullet bullet) { return bullet.body.getFillColor() == sf::Color::Transparent; }),
			bulletList.end());
	}
}

float Bullet::trySpawn(sf::Vector2f playerLocation, sf::RenderWindow& window, float bulletSpawnTimer, float bulletSpawnTimerMax, float deltatime)
{
	std::cout << bulletSpawnTimer << std::endl;
	if (bulletSpawnTimer >= bulletSpawnTimerMax)
	{
		new Bullet(1000.f, 20.f, playerLocation, static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
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


