#include <SFML/Graphics.hpp>
#include <iostream>
#include "Enemy.h"
#include "Bullet.h"

int main()
{
	static const int SCREEN_WIDTH = 1280;
	static const int SCREEN_HEIGHT = 720;

	static float enemySpawnTimer = 0.f;
	static float enemySpawnTimerMax = .05f;
	static float bulletSpawnTimer = 0.f;
	static float bulletSpawnTimerMax = .05f;
	float deltaTime = 0.0f;
	sf::Clock clock;

	sf::RectangleShape supply(sf::Vector2f(20.0f, 20.0f));
	supply.setFillColor(sf::Color::Cyan);
	supply.setPosition(sf::Vector2f(100.0f, 100.0f));

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Ekrâns", sf::Style::Default);
	sf::RectangleShape player(sf::Vector2f(50.0f, 50.0f));
	player.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	sf::Texture tex;
	tex.loadFromFile("2r.bmp");
	player.setTexture(&tex);

	Enemy enemy1(100, 20, sf::Vector2f(400.f, 400.f));
	Enemy enemy2(100, 20, sf::Vector2f(450.f, 400.f));


	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		sf::Event e;
		while (window.pollEvent(e))
		{

			if (e.type == e.Closed)
			{
				window.close();
			}
			/*
			//strâdâ tikai ðajâ while cilpâ
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
			{
				new Bullet(1000.f, 15.f, player.getPosition(), static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
			}
			*/
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) 
		{
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			player.move(-500.0f * deltaTime, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			player.move(0.0f, 500.0f * deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			player.move(500.0f * deltaTime, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			player.move(0.0f, -500.0f * deltaTime);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			new Enemy(100.f, 20.f, sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)));
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			bulletSpawnTimer += Bullet::trySpawn(player.getPosition(), window, bulletSpawnTimer, bulletSpawnTimerMax, deltaTime);
		}
		
		window.clear();

		Enemy::drawAll(window);
		Enemy::update(deltaTime, player.getPosition());
		Enemy::checkPlayerTouch(player);
		enemySpawnTimer += Enemy::trySpawn(enemySpawnTimer, enemySpawnTimerMax, deltaTime);
		Bullet::drawAll(window);
		Bullet::update(deltaTime);
		Bullet::checkRemove(window);
		Bullet::checkCollisions(window);
		window.draw(supply);
		window.draw(player);

		window.display();
	}


	return 0;
}