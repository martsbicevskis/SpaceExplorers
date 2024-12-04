#include "Game.h"
#include <iostream>

Game::Game() :
    window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Ekrāns", sf::Style::Default),
    gameTime(.0f),
    borderDamage(1.0f),
    planet(50.f),
	healthBarBorder(sf::Vector2f(100.0f, 20.0f)),
	manaBarBorder(sf::Vector2f(100.0f, 20.0f)),
	healthBar(sf::Vector2f(100.0f, 20.0f)),
	manaBar(sf::Vector2f(100.0f, 20.0f)),
    player(sf::Vector2f(playerSize, playerSize)),
    playerHealth(defaultPlayerHealth),
	playerSpeed(defaultPlayerSpeed),
    playerMoney(0),
	playerMana(0),
	maxPlayerMana(defaultPlayerManaMax),
    enemySpawnTimer(4.f),
    enemySpawnTimerMax(5.f),
    bulletSpawnTimer(0.f),
    bulletSpawnTimerMax(0.2f),
	healthUpgradeCost(10.f),
	movementSpeedUpgradeCost(10.f),
	firingSpeedUpgradeCost(10.f),
    state(GameState::MENU)

{
    if (!font.loadFromFile("arial.ttf")) 
    {
        std::cerr << "Failed to load font!" << std::endl;
    }
	if (!backgroundTexture.loadFromFile("Background.png"))
	{
		std::cerr << "Failed to load background!" << std::endl;
	}
	background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	background.setTexture(&backgroundTexture);

	if (!playerTexture.loadFromFile("player.png")) 
    {
		std::cerr << "Failed to load player texture!" << std::endl;
	}
    player.setPosition(SCREEN_WIDTH / 2 - player.getSize().x / 2, SCREEN_HEIGHT / 2 - player.getSize().y / 2);
	player.setTexture(&playerTexture);
	
    healthBarBorder.setOutlineColor(sf::Color::White);
	healthBarBorder.setOutlineThickness(2);
	healthBarBorder.setFillColor(sf::Color::Transparent);
	healthBarBorder.setPosition(sf::Vector2f(SCREEN_WIDTH - healthBarBorder.getSize().x - 10, 20));

	manaBarBorder.setOutlineColor(sf::Color::White);
	manaBarBorder.setOutlineThickness(2);
	manaBarBorder.setFillColor(sf::Color::Transparent);
    manaBarBorder.setPosition(sf::Vector2f(healthBarBorder.getPosition().x, healthBarBorder.getPosition().y + 40));

    healthBar.setPosition(sf::Vector2f(healthBarBorder.getPosition()));

    manaBar.setPosition(sf::Vector2f(healthBarBorder.getPosition().x, healthBarBorder.getPosition().y + 40));
    
    moneyText.setPosition(healthBarBorder.getPosition().x, healthBarBorder.getPosition().y + 60);

    planet.setPosition(0, 0);

	// Money Display
    moneyText.setFont(font);
    moneyText.setCharacterSize(30);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setOutlineThickness(2);
    moneyText.setOutlineColor(sf::Color::White);
    moneyText.setPosition(SCREEN_WIDTH - 100, 100);


    title.setFont(font);
    title.setString("Space Explorers");
    title.setCharacterSize(100);
    title.setFillColor(sf::Color::White);
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color::Cyan);
    title.setPosition(SCREEN_WIDTH * 0.22, SCREEN_HEIGHT / 8);

    // Play button
    playButton.setFont(font);
    playButton.setString("Play");
    playButton.setCharacterSize(50);
    playButton.setFillColor(sf::Color::White);
    playButton.setOutlineThickness(2);
    playButton.setOutlineColor(sf::Color::White);
    playButton.setPosition(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 75);

    // Settings button
    settingsButton.setFont(font);
    settingsButton.setString("Settings");
    settingsButton.setCharacterSize(50);
    settingsButton.setFillColor(sf::Color::White);
    settingsButton.setOutlineThickness(2);
    settingsButton.setOutlineColor(sf::Color::White);
    settingsButton.setPosition(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2);

    // Exit button
    exitButton.setFont(font);
    exitButton.setString("Exit");
    exitButton.setCharacterSize(50);
    exitButton.setFillColor(sf::Color::White);
    exitButton.setOutlineThickness(2);
    exitButton.setOutlineColor(sf::Color::White);
    exitButton.setPosition(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 75);

    // Back button for Settings
    backButton.setFont(font);
    backButton.setString("Back");
    backButton.setCharacterSize(50);
    backButton.setFillColor(sf::Color::White);
    backButton.setOutlineThickness(2);
    backButton.setOutlineColor(sf::Color::White);
    backButton.setPosition(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2);

    // Pause Menu
    pauseTitle.setFont(font);
    pauseTitle.setString("Game Paused");
    pauseTitle.setCharacterSize(60);
    pauseTitle.setFillColor(sf::Color::White);
    pauseTitle.setOutlineThickness(2);
    pauseTitle.setOutlineColor(sf::Color::Cyan);
    pauseTitle.setPosition(SCREEN_WIDTH / 2 - 190, SCREEN_HEIGHT / 2 - 150);

    continueButton.setFont(font);
    continueButton.setString("Continue");
    continueButton.setCharacterSize(50);
    continueButton.setFillColor(sf::Color::White);
    continueButton.setOutlineThickness(2);
    continueButton.setOutlineColor(sf::Color::White);
    continueButton.setPosition(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50);

    mainMenuButton.setFont(font);
    mainMenuButton.setString("Main Menu");
    mainMenuButton.setCharacterSize(50);
    mainMenuButton.setFillColor(sf::Color::White);
    mainMenuButton.setOutlineThickness(2);
    mainMenuButton.setOutlineColor(sf::Color::White);
    mainMenuButton.setPosition(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 50);

    // Game Over Menu
    gameOverTitle.setFont(font);
    gameOverTitle.setString("Game Over!");
    gameOverTitle.setCharacterSize(80);
    gameOverTitle.setFillColor(sf::Color::White);
    gameOverTitle.setOutlineThickness(2);
    gameOverTitle.setOutlineColor(sf::Color::Red);
    gameOverTitle.setPosition(SCREEN_WIDTH * 0.32, SCREEN_HEIGHT / 2 - 180);

    restartButton.setFont(font);
    restartButton.setString("Restart");
    restartButton.setCharacterSize(50);
    restartButton.setFillColor(sf::Color::White);
    restartButton.setOutlineThickness(2);
    restartButton.setOutlineColor(sf::Color::White);
    restartButton.setPosition(SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2 - 50);

    gameOverMainMenuButton.setFont(font);
    gameOverMainMenuButton.setString("Main Menu");
    gameOverMainMenuButton.setCharacterSize(50);
    gameOverMainMenuButton.setFillColor(sf::Color::White);
    gameOverMainMenuButton.setOutlineThickness(2);
    gameOverMainMenuButton.setOutlineColor(sf::Color::White);
    gameOverMainMenuButton.setPosition(SCREEN_WIDTH * 0.4, SCREEN_HEIGHT / 2 + 50);

    shopTitle.setFont(font);
    shopTitle.setString("Shop");
    shopTitle.setCharacterSize(70);
    shopTitle.setFillColor(sf::Color::White);
    shopTitle.setOutlineThickness(2);
    shopTitle.setOutlineColor(sf::Color::Yellow);
    shopTitle.setPosition(SCREEN_WIDTH / 9 * 4, SCREEN_HEIGHT / 10);

    closeInfoText.setFont(font);
    closeInfoText.setString("[Press 'E' to close]");
    closeInfoText.setCharacterSize(20);
    closeInfoText.setFillColor(sf::Color::White);
    closeInfoText.setPosition(SCREEN_WIDTH / 9 * 4, SCREEN_HEIGHT / 10 - 10);

	healthUpgradeButton.setFont(font);
	healthUpgradeButton.setString("Upgrade Health(" + std::to_string(static_cast<int>(playerHealth)) + ")");
	healthUpgradeButton.setCharacterSize(50);
	healthUpgradeButton.setFillColor(sf::Color::White);
	healthUpgradeButton.setOutlineThickness(1);
	healthUpgradeButton.setOutlineColor(sf::Color::White);
	healthUpgradeButton.setPosition(SCREEN_WIDTH / 5, SCREEN_HEIGHT / 2);

	movementSpeedUpgradeButton.setFont(font);
	movementSpeedUpgradeButton.setString("Upgrade Movement Speed(" + std::to_string(static_cast<int>(playerSpeed)) + ")");
	movementSpeedUpgradeButton.setCharacterSize(50);
	movementSpeedUpgradeButton.setFillColor(sf::Color::White);
	movementSpeedUpgradeButton.setOutlineThickness(1);
	movementSpeedUpgradeButton.setOutlineColor(sf::Color::White);
	movementSpeedUpgradeButton.setPosition(SCREEN_WIDTH / 5, SCREEN_HEIGHT / 2 + 50);

	firingSpeedUpgradeButton.setFont(font);
	firingSpeedUpgradeButton.setString("Reduce Firing Cooldown(" + std::to_string(static_cast<int>(bulletSpawnTimerMax * 1000)) + " ms)");
	firingSpeedUpgradeButton.setCharacterSize(50);
	firingSpeedUpgradeButton.setFillColor(sf::Color::White);
	firingSpeedUpgradeButton.setOutlineThickness(1);
    firingSpeedUpgradeButton.setOutlineColor(sf::Color::White);
	firingSpeedUpgradeButton.setPosition(SCREEN_WIDTH / 5, SCREEN_HEIGHT / 2 + 100);

    healthUpgradeButtonCost.setFont(font);
    healthUpgradeButtonCost.setString(std::to_string(static_cast<int>(healthUpgradeCost)));
    healthUpgradeButtonCost.setCharacterSize(50);
    healthUpgradeButtonCost.setFillColor(sf::Color::Yellow);
    healthUpgradeButtonCost.setOutlineThickness(2);
    healthUpgradeButtonCost.setOutlineColor(sf::Color::White);
    healthUpgradeButtonCost.setPosition(SCREEN_WIDTH / 5 * 4, SCREEN_HEIGHT / 2);

    movementSpeedUpgradeButtonCost.setFont(font);
    movementSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(movementSpeedUpgradeCost)));
    movementSpeedUpgradeButtonCost.setCharacterSize(50);
    movementSpeedUpgradeButtonCost.setFillColor(sf::Color::White);
    movementSpeedUpgradeButtonCost.setOutlineThickness(2);
    movementSpeedUpgradeButtonCost.setOutlineColor(sf::Color::Yellow);
    movementSpeedUpgradeButtonCost.setPosition(SCREEN_WIDTH / 5 * 4, SCREEN_HEIGHT / 2 + 50);

    firingSpeedUpgradeButtonCost.setFont(font);
    firingSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(firingSpeedUpgradeCost)));
    firingSpeedUpgradeButtonCost.setCharacterSize(50);
    firingSpeedUpgradeButtonCost.setFillColor(sf::Color::White);
    firingSpeedUpgradeButtonCost.setOutlineThickness(2);
    firingSpeedUpgradeButtonCost.setOutlineColor(sf::Color::Yellow);
    firingSpeedUpgradeButtonCost.setPosition(SCREEN_WIDTH / 5 * 4, SCREEN_HEIGHT / 2 + 100);

}

void Game::run()
{
    while (window.isOpen()) 
    {
        float deltaTime = clock.restart().asSeconds();

        switch (state) 
        {
        case GameState::MENU:
            handleMenuInput();
            renderMenu();
            break;
        case GameState::SETTINGS:
            handleSettingsInput();
            renderSettings();
            break;
        case GameState::PAUSE:
            handlePauseInput();
            renderPause();
            break;
        case GameState::PLAY:
            handleGameInput(deltaTime);
            update(deltaTime);
            renderGame();
            break;
        case GameState::GAME_OVER:
            handleGameOverInput();
            renderGameOver();
            break;
		case GameState::SHOP:
			handleShopInput();
			renderShop();
			break;
        }
    }
}

void Game::handleMenuInput()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        // Detect mouse clicks on buttons
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (playButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::PLAY;
            }
            else if (settingsButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::SETTINGS; 
            }
            else if (exitButton.getGlobalBounds().contains(mousePos)) 
            {
                window.close(); 
            }
        }
    }
	sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));
    if (playButton.getGlobalBounds().contains(mousePos)) 
    {
		playButton.setOutlineColor(sf::Color::Green);
    }
    else
    {
		playButton.setOutlineColor(sf::Color::White);
    }
    if (settingsButton.getGlobalBounds().contains(mousePos))
    {
		settingsButton.setOutlineColor(sf::Color::Green);
    }
	else
	{
		settingsButton.setOutlineColor(sf::Color::White);
	}
    if (exitButton.getGlobalBounds().contains(mousePos)) 
    {
		exitButton.setOutlineColor(sf::Color::Green);
    }
	else
	{
		exitButton.setOutlineColor(sf::Color::White);
	}
}

void Game::handlePauseInput() 
{
    sf::Event event;
    while (window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
        {
            window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        {
            state = GameState::PLAY;
        }
        // Detect mouse clicks on Pause Menu buttons
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (continueButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::PLAY; // Resume the game
            }
            else if (mainMenuButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::MENU; // Go back to main menu
            }
        }
    }
    sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));
    if (continueButton.getGlobalBounds().contains(mousePos))
    {
        continueButton.setOutlineColor(sf::Color::Green);
    }
    else
    {
        continueButton.setOutlineColor(sf::Color::White);
    }
    if (mainMenuButton.getGlobalBounds().contains(mousePos))
    {
        mainMenuButton.setOutlineColor(sf::Color::Green);
    }
    else
    {
        mainMenuButton.setOutlineColor(sf::Color::White);
    }
}

void Game::handleGameOverInput() 
{
    sf::Event event;
    while (window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
        {
            window.close();
        }

        // Detect mouse clicks on Game Over Menu buttons
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (restartButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::PLAY; // Restart the game
				gameTime = .0f;
				borderDamage = 1.0f;
                playerHealth = defaultPlayerHealth;
				playerSpeed = defaultPlayerSpeed;
                playerMoney = 0;
				playerMana = 0;
				maxPlayerMana = defaultPlayerManaMax;
				enemySpawnTimer = 4.f;
				enemySpawnTimerMax = 5.f;
				bulletSpawnTimer = 0.f;
				bulletSpawnTimerMax = 0.2f;
                enemySpawnTimer = 0.f;
                bulletSpawnTimer = 0.f;
				healthUpgradeCost = 10.f;
				movementSpeedUpgradeCost = 10.f;
				firingSpeedUpgradeCost = 10.f;
                player.setPosition(SCREEN_WIDTH / 2 - player.getSize().x / 2, SCREEN_HEIGHT / 2 - player.getSize().y / 2);
                Enemy::enemyList.clear();
                Bullet::bulletList.clear();
            }
            else if (gameOverMainMenuButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::MENU; // Go back to main menu
            }
        }
    }
	sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));
	if (restartButton.getGlobalBounds().contains(mousePos))
	{
		restartButton.setOutlineColor(sf::Color::Red);
	}
	else
	{
		restartButton.setOutlineColor(sf::Color::White);
	}
	if (gameOverMainMenuButton.getGlobalBounds().contains(mousePos))
	{
		gameOverMainMenuButton.setOutlineColor(sf::Color::Red);
	}
	else
	{
		gameOverMainMenuButton.setOutlineColor(sf::Color::White);
	}
}

void Game::handleShopInput()
{
    sf::Event event;
    while (window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
        {
            window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) 
        {
            state = GameState::PLAY;
        }

        // Detect mouse clicks on Shop Menu buttons
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (healthUpgradeButton.getGlobalBounds().contains(mousePos) || healthUpgradeButtonCost.getGlobalBounds().contains(mousePos)) 
            {
                if (playerMoney >= healthUpgradeCost) 
                {
                    playerMoney -= healthUpgradeCost;
                    healthUpgradeCost *= 1.2;
                    playerHealth += 20;
					healthUpgradeButtonCost.setString(std::to_string(static_cast<int>(healthUpgradeCost)));
                }
            }
            else if (movementSpeedUpgradeButton.getGlobalBounds().contains(mousePos) || movementSpeedUpgradeButtonCost.getGlobalBounds().contains(mousePos))
            {
                if (playerMoney >= movementSpeedUpgradeCost) 
                {
                    playerMoney -= movementSpeedUpgradeCost;
                    movementSpeedUpgradeCost *= 1.2;
                    playerSpeed += 50;
					movementSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(movementSpeedUpgradeCost)));
                }
			}
            else if (firingSpeedUpgradeButton.getGlobalBounds().contains(mousePos) || firingSpeedUpgradeButtonCost.getGlobalBounds().contains(mousePos))
            {
                if (playerMoney >= firingSpeedUpgradeCost) 
                {
                    playerMoney -= firingSpeedUpgradeCost;
                    firingSpeedUpgradeCost *= 1.2;
                    bulletSpawnTimerMax -= .02f;
					firingSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(firingSpeedUpgradeCost)));
                }
            }
        }
        firingSpeedUpgradeButton.setString("Reduce Firing Cooldown(" + std::to_string(static_cast<int>(bulletSpawnTimerMax * 1000)) + " ms)");
        movementSpeedUpgradeButton.setString("Upgrade Movement Speed(" + std::to_string(static_cast<int>(playerSpeed)) + ")");
        healthUpgradeButton.setString("Upgrade Health(" + std::to_string(static_cast<int>(playerHealth)) + ")");    
    }
	sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));
	if ((healthUpgradeButton.getGlobalBounds().contains(mousePos) 
        || healthUpgradeButtonCost.getGlobalBounds().contains(mousePos)) 
        && playerMoney >= healthUpgradeCost)
	{
		healthUpgradeButton.setOutlineColor(sf::Color::Green);
	}
	else if(playerMoney >= healthUpgradeCost)
	{
		healthUpgradeButton.setOutlineColor(sf::Color::White);
	}
	else
	{
		healthUpgradeButton.setOutlineColor(sf::Color::Red);
	}
	if ((movementSpeedUpgradeButton.getGlobalBounds().contains(mousePos)
		|| movementSpeedUpgradeButtonCost.getGlobalBounds().contains(mousePos))
		&& playerMoney >= movementSpeedUpgradeCost)
	{
		movementSpeedUpgradeButton.setOutlineColor(sf::Color::Green);
	}
	else if (playerMoney >= movementSpeedUpgradeCost)
	{
		movementSpeedUpgradeButton.setOutlineColor(sf::Color::White);
	}
	else
	{
		movementSpeedUpgradeButton.setOutlineColor(sf::Color::Red);
	}
	if ((firingSpeedUpgradeButton.getGlobalBounds().contains(mousePos)
		|| firingSpeedUpgradeButtonCost.getGlobalBounds().contains(mousePos))
		&& playerMoney >= firingSpeedUpgradeCost)
	{
		firingSpeedUpgradeButton.setOutlineColor(sf::Color::Green);
	}
	else if (playerMoney >= firingSpeedUpgradeCost)
	{
		firingSpeedUpgradeButton.setOutlineColor(sf::Color::White);
	}
	else
	{
		firingSpeedUpgradeButton.setOutlineColor(sf::Color::Red);
	}

}



void Game::handleSettingsInput() 
{
    sf::Event event;
    while (window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
        {
            window.close();
        }

        // Detect Back button click
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (backButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::MENU; // Return to menu
            }
        }
    }
}

void Game::handleGameInput(float deltaTime) 
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        // Pause the game when Esc is pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) 
        {
            state = GameState::PAUSE;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
        {
            state = GameState::SHOP;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) 
        {
            activateManaAbility();
        }
    }

    // Game-specific input logic (player movement, shooting, etc.)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) 
    {
        player.move(-playerSpeed * deltaTime, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        player.move(0.0f, playerSpeed * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) 
    {
        player.move(playerSpeed * deltaTime, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        player.move(0.0f, -playerSpeed * deltaTime);
    }

    // Spawn bullets
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        bulletSpawnTimer += Bullet::trySpawn(sf::Vector2f(player.getPosition().x + (playerSize - defaultBulletSize) / 2, player.getPosition().y + (playerSize - defaultBulletSize) / 2), window, bulletSpawnTimer, bulletSpawnTimerMax, deltaTime);
    }
}

void Game::drawHealthBar(sf::RenderWindow& window)
{
    if (playerHealth > defaultPlayerHealth)
    {
        healthBar.setSize(sf::Vector2f(healthBarBorder.getSize()));
    }
    else
    {
        healthBar.setSize(sf::Vector2f(manaBarBorder.getSize().x * (playerHealth / defaultPlayerHealth), healthBar.getSize().y));
    }
    healthBar.setFillColor(sf::Color::Green);
	window.draw(healthBar);
	window.draw(healthBarBorder);
}

void Game::drawManaBar(sf::RenderWindow& window)
{
    manaBar.setSize(sf::Vector2f(manaBarBorder.getSize().x * (playerMana / maxPlayerMana), manaBar.getSize().y));
    manaBar.setFillColor((playerMana >= maxPlayerMana) ? sf::Color::Magenta : sf::Color::Cyan);
	window.draw(manaBar);
	window.draw(manaBarBorder); 
}

void Game::drawMoneyText(sf::RenderWindow& window)
{
	moneyText.setString(std::to_string(playerMoney) + " $");
	window.draw(moneyText);
}

float Game::applyBorderDamage()
{
	float damage = 0.0f;
	if (player.getPosition().x < -player.getSize().x ||
        player.getPosition().x > SCREEN_WIDTH ||
        player.getPosition().y < -player.getSize().y ||
        player.getPosition().y > SCREEN_HEIGHT)
	{
		damage = .1f * borderDamage;
	}
	return damage;
}

void Game::activateManaAbility()
{
	if (playerMana >= maxPlayerMana)
	{
		playerMana = 0.f;
		Enemy::manaAbilityDamage();
	}
}

void Game::update(float deltaTime) 
{
    // Update enemies and bullets
    Enemy::update(deltaTime, player.getPosition());
    playerHealth -= Enemy::checkPlayerTouch(player, playerHealth);
	playerHealth -= applyBorderDamage();
    enemySpawnTimer += Enemy::trySpawn(enemySpawnTimer, enemySpawnTimerMax, deltaTime);
	enemySpawnTimerMax = 5.f - (gameTime / 60.f);


    Bullet::update(deltaTime);
    Bullet::checkRemove(window);
    Bullet::checkCollisions(window);
    playerMoney += Enemy::hitRemove();
    playerMana += Bullet::hitRemove();

	if (playerMana > maxPlayerMana)
	{
		playerMana = maxPlayerMana;
	}

    // Check for game over
    if (playerHealth <= 0) 
    {
        state = GameState::GAME_OVER;
    }

	gameTime += deltaTime;
}

void Game::renderMenu() 
{
    window.clear();
	window.draw(background);
	window.draw(title);
    window.draw(playButton);
    window.draw(settingsButton);
    window.draw(exitButton);
    window.display();
}

void Game::renderSettings() 
{
    window.clear();
    window.draw(background);
    window.draw(backButton);
    window.display();
}

void Game::renderGame()
{
    window.clear();

    // Draw game objects
    //window.draw(planet);
    window.draw(background);
    window.draw(player);
	drawHealthBar(window);
	drawManaBar(window);
	drawMoneyText(window);
    Enemy::drawAll(window);
    Bullet::drawAll(window);

    window.display();
}

void Game::renderPause() 
{
    window.clear();
    window.draw(background);
    window.draw(pauseTitle);
    window.draw(continueButton);
    window.draw(mainMenuButton);
    window.display();
}

void Game::renderGameOver() 
{
    window.clear();
    window.draw(background);
    window.draw(gameOverTitle);
    window.draw(restartButton);
    window.draw(gameOverMainMenuButton);
    window.display();
}

void Game::renderShop()
{
    window.clear();

    window.draw(background);
    window.draw(shopTitle);
    window.draw(healthUpgradeButton);
    window.draw(movementSpeedUpgradeButton);
    window.draw(firingSpeedUpgradeButton);
	window.draw(healthUpgradeButtonCost);
	window.draw(movementSpeedUpgradeButtonCost);
	window.draw(firingSpeedUpgradeButtonCost);
    window.draw(closeInfoText);

    drawHealthBar(window);
    drawManaBar(window);
    drawMoneyText(window);

    window.display();
}

