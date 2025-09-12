#include "Particles.h"

std::vector<Particles> Particles::particleList;

Particles::Particles(sf::Vector2f position, sf::Vector2f size, sf::Texture& texture, float expireTime) :
    position(position),
    size(size),
    texture(texture),
    expireTime(expireTime)
{
    body.setPosition(position);
    body.setSize(size);
    body.setTexture(&texture);
    body.setFillColor(sf::Color::White);
    particleList.push_back(*this);
}

void Particles::drawAll(sf::RenderWindow& window)
{
    for (auto& p : particleList)
    {
        window.draw(p.body);
    }
}

void Particles::updateExpireTime(float deltaTime)
{
    for (auto it = particleList.begin(); it != particleList.end();)
    {
        it->expireTime -= deltaTime;
        if (it->expireTime <= 0)
        {
            it = particleList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
