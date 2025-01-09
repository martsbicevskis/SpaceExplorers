#include "Particles.h"
#include <SFML/Graphics.hpp>

std::vector<Particles> Particles::particleList;

Particles::Particles(sf::RectangleShape body, sf::Vector2f position, sf::Vector2f size, sf::Texture texture, float expireTime) :
	body(body),
	position(position),
	size(size),
	texture(texture),
	expireTime(expireTime)
{
	body.setPosition(position);
	body.setSize(size);
	body.setTexture(&texture);
	particleList.push_back(*this);
}

void Particles::drawAll(sf::RenderWindow& window)
{
	for (auto& p : particleList)
	{
		p.draw(window);
	}
}

void Particles::updateExpireTime(float deltaTime)
{

}

void Particles::remove()
{

}

void Particles::draw(sf::RenderWindow& window)
{
	window.draw(body);
}
