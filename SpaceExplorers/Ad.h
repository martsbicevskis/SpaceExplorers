#pragma once
#include "Game.h"

class Ad
{
public:
	Ad(int slot, sf::Texture& adTexture);
	~Ad();

	static std::vector<Ad> adList;
	static void drawAll(sf::RenderWindow& window);

private:
	int slot;
	sf::Texture& adTexture;

public:
	sf::RectangleShape body;
};

