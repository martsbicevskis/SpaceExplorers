#include "Ad.h"

std::vector<Ad> Ad::adList;

Ad::Ad(int slot, sf::Texture& adTexture) : slot(slot), adTexture(adTexture)
{
	switch (slot)
	{
	case 1:
		body.setPosition(Game::SCREEN_WIDTH * 0.15, Game::SCREEN_HEIGHT * 0.15);
		break;
	case 2:
		body.setPosition(Game::SCREEN_WIDTH * 0.55, Game::SCREEN_HEIGHT * 0.15);
		break;
	case 3:
		body.setPosition(Game::SCREEN_WIDTH * 0.15, Game::SCREEN_HEIGHT * 0.55);
		break;
	case 4:
		body.setPosition(Game::SCREEN_WIDTH * 0.55, Game::SCREEN_HEIGHT * 0.55);
		break;
	}
	body.setSize(sf::Vector2f(Game::SCREEN_WIDTH * 0.3, Game::SCREEN_HEIGHT * 0.3));
	body.setTexture(&adTexture);
	adList.push_back(*this);
}

Ad::~Ad()
{
}

void Ad::drawAll(sf::RenderWindow& window)
{
	for (auto& a : adList)
	{
		window.draw(a.body);
	}
}
