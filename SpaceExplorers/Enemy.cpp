#include "Enemy.h"

std::vector<Enemy> Enemy::enemyList;

Enemy::Enemy(float speed, float size, sf::Vector2f location)
{
	this->speed = speed;
	this->size = size;
	this->location = location;

	body.setSize(sf::Vector2f(size, size));
	body.setPosition(location);
	body.setFillColor(sf::Color::Cyan);
	body.setOutlineThickness(2);
	body.setOutlineColor(sf::Color::Blue);
	enemyList.push_back(*this);
}

float Enemy::trySpawn(float spawnTimer, float spawnTimerMax, float deltaTime)
{
	if (spawnTimer >= spawnTimerMax)
	{
		new Enemy(100, 20, sf::Vector2f(0.f, 0.f));
		return -spawnTimerMax;
	}
	else
	{
		return deltaTime;
	}


}

void Enemy::update(float deltaTime, sf::Vector2f playerPosition)
{
	float moveDistance = -100.f * deltaTime;
	for (auto& e : enemyList)
	{
		if (e.body.getPosition().x > playerPosition.x)
			e.body.move(moveDistance, 0.f);
		if (e.body.getPosition().x < playerPosition.x)
			e.body.move(-moveDistance, 0.f);
		if (e.body.getPosition().y > playerPosition.y)
			e.body.move(0.f, moveDistance);
		if (e.body.getPosition().y < playerPosition.y)
			e.body.move(0.f, -moveDistance);
	}
}

void Enemy::hitRemove()
{
	for (auto e : Enemy::enemyList)
	{
		Enemy::enemyList.erase(std::remove_if(enemyList.begin(), enemyList.end(),
			[](const Enemy enemy) { return enemy.body.getFillColor() == sf::Color::Transparent; }),
			enemyList.end());
	}
}

void Enemy::checkPlayerTouch(sf::RectangleShape player)
{
	for (auto e : enemyList)
	{
		if (e.body.getPosition().x + e.body.getSize().x > player.getPosition().x &&
			e.body.getPosition().x < player.getPosition().x + player.getSize().x &&
			e.body.getPosition().y + e.body.getSize().y > player.getPosition().y &&
			e.body.getPosition().y < player.getPosition().y + player.getSize().y)
		{
			std::cout << "rfe3f";
		}
	}
}

void Enemy::draw(sf::RenderWindow& window)
{
	window.draw(body);
}


void Enemy::drawAll(sf::RenderWindow& window)
{
	for (auto& e : Enemy::enemyList)
	{
		window.draw(e.body);
	}
}
