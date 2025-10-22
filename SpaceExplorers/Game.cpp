#include "Game.h"
#include <iostream>

// Game constructor (called when the game is created)
Game::Game() :
    //initializing the game variables
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
    shockwaveRenderTime(0.f),
    shopOpeningCooldown(0.f),
    difficulty(1.f),
    enemiesPerWave(5),
    bossSpawnTimer(0.f),
    bossSpawnTimerMax(30.f),
    enemySpawnTimer(2.f),
    enemySpawnTimerMax(3.f),
    bulletSpawnTimer(0.f),
    bulletSpawnTimerMax(0.5f),
    healthUpgradeCost(10.f),
    movementSpeedUpgradeCost(10.f),
    firingSpeedUpgradeCost(10.f),
    winningSurvivalTime(70.f),
    continueAvailable(false),
    shotMode(ShotMode::RAPID),
    state(GameState::MENU)

{
    initializeTextures();
    initializeButtons();
	initializeRectangles();    
}

// Main Game Loop
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
            handlePauseInput(deltaTime);
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
			handleShopInput(deltaTime);
			renderShop();
			break;
        case GameState::LEVEL:
            handleLevelInput();
            renderLevel();
            break;
        }
    }
}

// Input handling method (Menu)
void Game::handleMenuInput()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (playButton.getGlobalBounds().contains(mousePos)) 
            {
                Ad(1, shockwaveScreenOutlineTexture);
                Ad(2, shockwaveScreenOutlineTexture);
                Ad(3, shockwaveScreenOutlineTexture);
                Ad(4, shockwaveScreenOutlineTexture);

                state = GameState::LEVEL;
            }
            else if (menuContinueButton.getGlobalBounds().contains(mousePos) && continueAvailable)
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

    if (continueAvailable) hightlightHower(menuContinueButton, mousePos);
	hightlightHower(playButton, mousePos);
	hightlightHower(settingsButton, mousePos);
	hightlightHower(exitButton, mousePos);
}

// Input handling method (Pause)
void Game::handlePauseInput(float deltaTime) 
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
            if (shopOpeningCooldown >= openingCooldownMax)
            {
                state = GameState::PLAY;
                shopOpeningCooldown = 0.f;
            }
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (continueButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::PLAY; 
            }
            else if (mainMenuButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::MENU; 
            }
        }
    }
    sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));
    
	hightlightHower(continueButton, mousePos);
	hightlightHower(mainMenuButton, mousePos);

    if (shopOpeningCooldown < openingCooldownMax)
    {
        shopOpeningCooldown += deltaTime;
    }
}

// Input handling method (Game Over)
void Game::handleGameOverInput() 
{
    sf::Event event;
    while (window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (restartButton.getGlobalBounds().contains(mousePos)) 
            {
                resetGameSettings();
                state = GameState::PLAY; 
            }
            else if (gameOverMainMenuButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::MENU; 
            }
        }
    }
	sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));

	hightlightHower(restartButton, mousePos);
	hightlightHower(gameOverMainMenuButton, mousePos);
}

// Input handling method (Shop)
void Game::handleShopInput(float deltaTime)
{
    sf::Event event;
    while (window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
        {
            window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        {
            if (shopOpeningCooldown >= openingCooldownMax)
            {
                state = GameState::PLAY;
				shopOpeningCooldown = 0.f;
            }
        }

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
                    bulletSpawnTimerMax *= .5f;
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
    if (shopOpeningCooldown < openingCooldownMax)
    {
        shopOpeningCooldown += deltaTime;
    }

}

void Game::handleLevelInput()
{

    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (levelBackButton.getGlobalBounds().contains(mousePos))
            {
                state = GameState::MENU;
            }
            if (levelPlayButton.getGlobalBounds().contains(mousePos))
            {
                winningSurvivalTime = 70.f;
                continueAvailable = true;
                resetGameSettings();
                state = GameState::PLAY;
            }
            if (levelPlayButtonEasy.getGlobalBounds().contains(mousePos))
            {
                difficulty = 1.f;
                winningSurvivalTime = 70.f;
                continueAvailable = true;
                resetGameSettings();
                state = GameState::PLAY;
            }
            if (levelPlayButtonMedium.getGlobalBounds().contains(mousePos))
            {
                difficulty = 2.f;
                winningSurvivalTime = 70.f;
                continueAvailable = true;
                resetGameSettings();
                state = GameState::PLAY;
            }
            if (levelPlayButtonHard.getGlobalBounds().contains(mousePos))
            {
                difficulty = 3.f;
                winningSurvivalTime = 70.f;
                continueAvailable = true;
                resetGameSettings();
                state = GameState::PLAY;
            }
            if (levelPlayButtonInfinite.getGlobalBounds().contains(mousePos))
            {
                difficulty = 3.f;
                winningSurvivalTime = std::numeric_limits<float>::max();
                continueAvailable = true;
                resetGameSettings();
                state = GameState::PLAY;
            }
        }
    }
    sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));

	hightlightHower(levelBackButton, mousePos);
	hightlightHower(levelPlayButton, mousePos);
}



// Input handling method (Settings)
void Game::handleSettingsInput() 
{
    sf::Event event;
    while (window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (backButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::MENU; 
            }
        }
    }
	sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));

	hightlightHower(backButton, mousePos);
}

// Input handling method (Game)
void Game::handleGameInput(float deltaTime) 
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
            if (shopOpeningCooldown >= openingCooldownMax)
            {
                state = GameState::PAUSE;
                shopOpeningCooldown = 0.f;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
        {
            if (shopOpeningCooldown >= openingCooldownMax)
            {
                state = GameState::SHOP;
				shopOpeningCooldown = 0.f;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) 
        {
            activateManaAbility();

        }
    }

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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
    {
        shotMode = ShotMode::RAPID;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
    {
        shotMode = ShotMode::SHOTGUN;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
    {
        shotMode = ShotMode::BOMB;
    }


    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        switch (shotMode)
        {
        case Game::ShotMode::RAPID:
            bulletSpawnTimer += Bullet::trySpawnRapid(sf::Vector2f(player.getPosition().x + (playerSize - defaultBulletSize) / 2, player.getPosition().y + (playerSize - defaultBulletSize) / 2), window, bulletSpawnTimer, bulletSpawnTimerMax, deltaTime, false);
            break;
        case Game::ShotMode::SHOTGUN:
            bulletSpawnTimer += Bullet::trySpawnShotgun(sf::Vector2f(player.getPosition().x + (playerSize - defaultBulletSize) / 2, player.getPosition().y + (playerSize - defaultBulletSize) / 2), window, bulletSpawnTimer, bulletSpawnTimerMax, deltaTime, false);
            break;
        case Game::ShotMode::BOMB:
            bulletSpawnTimer += Bullet::trySpawnBomb(sf::Vector2f(player.getPosition().x + (playerSize - defaultBulletSize) / 2, player.getPosition().y + (playerSize - defaultBulletSize) / 2), window, bulletSpawnTimer, bulletSpawnTimerMax, deltaTime, true);
            break;
        default:
            break;
        }
    }
	if (shopOpeningCooldown < openingCooldownMax)
	{
		shopOpeningCooldown += deltaTime;
	}
}

void Game::initializeButtons()
{
    // Money Display (Game)
    moneyText.setFont(font);
    moneyText.setCharacterSize(30);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setOutlineThickness(2);
    moneyText.setOutlineColor(sf::Color::White);
    moneyText.setPosition(SCREEN_WIDTH - 100, 100);

    // Title display (Main Menu)
    title.setFont(font);
    title.setString("Space Explorers");
    title.setCharacterSize(100);
    title.setFillColor(sf::Color::White);
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color::Cyan);
    title.setPosition(SCREEN_WIDTH * 0.22, SCREEN_HEIGHT / 8);

    // Continue button (Main Menu)
    menuContinueButton.setFont(font);
    menuContinueButton.setString("Continue");
    menuContinueButton.setCharacterSize(50);
    menuContinueButton.setFillColor(sf::Color::White);
    menuContinueButton.setOutlineThickness(2);
    menuContinueButton.setOutlineColor(sf::Color::White);
    menuContinueButton.setPosition(SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 - 80);

    // Play button (Main Menu)
    playButton.setFont(font);
    playButton.setString("New Game");
    playButton.setCharacterSize(50);
    playButton.setFillColor(sf::Color::White);
    playButton.setOutlineThickness(2);
    playButton.setOutlineColor(sf::Color::White);
    playButton.setPosition(SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 - 0);

    // Settings button (Main Menu)
    settingsButton.setFont(font);
    settingsButton.setString("Settings");
    settingsButton.setCharacterSize(50);
    settingsButton.setFillColor(sf::Color::White);
    settingsButton.setOutlineThickness(2);
    settingsButton.setOutlineColor(sf::Color::White);
    settingsButton.setPosition(SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 + 80);

    // Exit button (Main Menu)
    exitButton.setFont(font);
    exitButton.setString("Exit");
    exitButton.setCharacterSize(50);
    exitButton.setFillColor(sf::Color::White);
    exitButton.setOutlineThickness(2);
    exitButton.setOutlineColor(sf::Color::White);
    exitButton.setPosition(SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 + 160);

    // Back button (Settings)
    backButton.setFont(font);
    backButton.setString("Back");
    backButton.setCharacterSize(50);
    backButton.setFillColor(sf::Color::White);
    backButton.setOutlineThickness(2);
    backButton.setOutlineColor(sf::Color::White);
    backButton.setPosition(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2);

    // Pause Menu (Pause)
    pauseTitle.setFont(font);
    pauseTitle.setString("Game Paused");
    pauseTitle.setCharacterSize(60);
    pauseTitle.setFillColor(sf::Color::White);
    pauseTitle.setOutlineThickness(2);
    pauseTitle.setOutlineColor(sf::Color::Cyan);
    pauseTitle.setPosition(SCREEN_WIDTH / 2 - 190, SCREEN_HEIGHT / 2 - 150);

    // Continue button (Pause)
    continueButton.setFont(font);
    continueButton.setString("Continue");
    continueButton.setCharacterSize(50);
    continueButton.setFillColor(sf::Color::White);
    continueButton.setOutlineThickness(2);
    continueButton.setOutlineColor(sf::Color::White);
    continueButton.setPosition(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50);

    // Main Menu button (Pause)
    mainMenuButton.setFont(font);
    mainMenuButton.setString("Main Menu");
    mainMenuButton.setCharacterSize(50);
    mainMenuButton.setFillColor(sf::Color::White);
    mainMenuButton.setOutlineThickness(2);
    mainMenuButton.setOutlineColor(sf::Color::White);
    mainMenuButton.setPosition(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 50);

    // Game Over title (Game Over)
    gameOverTitle.setFont(font);
    gameOverTitle.setString("Game Over!");
    gameOverTitle.setCharacterSize(80);
    gameOverTitle.setFillColor(sf::Color::White);
    gameOverTitle.setOutlineThickness(2);
    gameOverTitle.setOutlineColor(sf::Color::Red);
    gameOverTitle.setPosition(SCREEN_WIDTH * 0.32, SCREEN_HEIGHT / 2 - 180);

    // Restart button (Game Over)
    restartButton.setFont(font);
    restartButton.setString("Restart");
    restartButton.setCharacterSize(50);
    restartButton.setFillColor(sf::Color::White);
    restartButton.setOutlineThickness(2);
    restartButton.setOutlineColor(sf::Color::White);
    restartButton.setPosition(SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2 - 50);

    // Main Menu button (Game Over)
    gameOverMainMenuButton.setFont(font);
    gameOverMainMenuButton.setString("Main Menu");
    gameOverMainMenuButton.setCharacterSize(50);
    gameOverMainMenuButton.setFillColor(sf::Color::White);
    gameOverMainMenuButton.setOutlineThickness(2);
    gameOverMainMenuButton.setOutlineColor(sf::Color::White);
    gameOverMainMenuButton.setPosition(SCREEN_WIDTH * 0.4, SCREEN_HEIGHT / 2 + 50);

    // Shop title (Shop)
    shopTitle.setFont(font);
    shopTitle.setString("Shop");
    shopTitle.setCharacterSize(70);
    shopTitle.setFillColor(sf::Color::White);
    shopTitle.setOutlineThickness(2);
    shopTitle.setOutlineColor(sf::Color::Yellow);
    shopTitle.setPosition(SCREEN_WIDTH / 9 * 4, SCREEN_HEIGHT / 10);

    // Close info text (Shop)
    closeInfoText.setFont(font);
    closeInfoText.setString("[Press 'E' to close]");
    closeInfoText.setCharacterSize(20);
    closeInfoText.setFillColor(sf::Color::White);
    closeInfoText.setPosition(SCREEN_WIDTH / 9 * 4, SCREEN_HEIGHT / 10 - 10);

    // Health Upgrade button (Shop)
    healthUpgradeButton.setFont(font);
    healthUpgradeButton.setString("Upgrade Health(" + std::to_string(static_cast<int>(playerHealth)) + ")");
    healthUpgradeButton.setCharacterSize(50);
    healthUpgradeButton.setFillColor(sf::Color::White);
    healthUpgradeButton.setOutlineThickness(1);
    healthUpgradeButton.setOutlineColor(sf::Color::White);
    healthUpgradeButton.setPosition(SCREEN_WIDTH / 5, SCREEN_HEIGHT / 2);

    // Movement Speed Upgrade button (Shop)
    movementSpeedUpgradeButton.setFont(font);
    movementSpeedUpgradeButton.setString("Upgrade Movement Speed(" + std::to_string(static_cast<int>(playerSpeed)) + ")");
    movementSpeedUpgradeButton.setCharacterSize(50);
    movementSpeedUpgradeButton.setFillColor(sf::Color::White);
    movementSpeedUpgradeButton.setOutlineThickness(1);
    movementSpeedUpgradeButton.setOutlineColor(sf::Color::White);
    movementSpeedUpgradeButton.setPosition(SCREEN_WIDTH / 5, SCREEN_HEIGHT / 2 + 50);

    // Firing Speed Upgrade button (Shop)
    firingSpeedUpgradeButton.setFont(font);
    firingSpeedUpgradeButton.setString("Reduce Firing Cooldown(" + std::to_string(static_cast<int>(bulletSpawnTimerMax * 1000)) + " ms)");
    firingSpeedUpgradeButton.setCharacterSize(50);
    firingSpeedUpgradeButton.setFillColor(sf::Color::White);
    firingSpeedUpgradeButton.setOutlineThickness(1);
    firingSpeedUpgradeButton.setOutlineColor(sf::Color::White);
    firingSpeedUpgradeButton.setPosition(SCREEN_WIDTH / 5, SCREEN_HEIGHT / 2 + 100);

    // Health Upgrade button cost (Shop)
    healthUpgradeButtonCost.setFont(font);
    healthUpgradeButtonCost.setString(std::to_string(static_cast<int>(healthUpgradeCost)));
    healthUpgradeButtonCost.setCharacterSize(50);
    healthUpgradeButtonCost.setFillColor(sf::Color::Yellow);
    healthUpgradeButtonCost.setOutlineThickness(2);
    healthUpgradeButtonCost.setOutlineColor(sf::Color::White);
    healthUpgradeButtonCost.setPosition(SCREEN_WIDTH / 5 * 4, SCREEN_HEIGHT / 2);

    // Movement Speed Upgrade button cost (Shop)
    movementSpeedUpgradeButtonCost.setFont(font);
    movementSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(movementSpeedUpgradeCost)));
    movementSpeedUpgradeButtonCost.setCharacterSize(50);
    movementSpeedUpgradeButtonCost.setFillColor(sf::Color::White);
    movementSpeedUpgradeButtonCost.setOutlineThickness(2);
    movementSpeedUpgradeButtonCost.setOutlineColor(sf::Color::Yellow);
    movementSpeedUpgradeButtonCost.setPosition(SCREEN_WIDTH / 5 * 4, SCREEN_HEIGHT / 2 + 50);

    // Firing Speed Upgrade button cost (Shop)
    firingSpeedUpgradeButtonCost.setFont(font);
    firingSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(firingSpeedUpgradeCost)));
    firingSpeedUpgradeButtonCost.setCharacterSize(50);
    firingSpeedUpgradeButtonCost.setFillColor(sf::Color::White);
    firingSpeedUpgradeButtonCost.setOutlineThickness(2);
    firingSpeedUpgradeButtonCost.setOutlineColor(sf::Color::Yellow);
    firingSpeedUpgradeButtonCost.setPosition(SCREEN_WIDTH / 5 * 4, SCREEN_HEIGHT / 2 + 100);

	// Play Button (Level)
	levelPlayButton.setFont(font);
	levelPlayButton.setString("Play");
	levelPlayButton.setCharacterSize(50);
	levelPlayButton.setFillColor(sf::Color::White);
	levelPlayButton.setOutlineThickness(2);
	levelPlayButton.setOutlineColor(sf::Color::White);
	levelPlayButton.setPosition(SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.9);
    

	// Back button (Level)
	levelBackButton.setFont(font);
	levelBackButton.setString("Back");
	levelBackButton.setCharacterSize(50);
	levelBackButton.setFillColor(sf::Color::White);
	levelBackButton.setOutlineThickness(2);
	levelBackButton.setOutlineColor(sf::Color::White);
	levelBackButton.setPosition(SCREEN_WIDTH * 0.1, SCREEN_HEIGHT * 0.9);

	//Level Title (Level)
	levelPlayButtonEasy.setFont(font);
    levelPlayButtonEasy.setString("Easy");
    levelPlayButtonEasy.setCharacterSize(40);
    levelPlayButtonEasy.setFillColor(sf::Color::White);
    levelPlayButtonEasy.setOutlineThickness(2);
    levelPlayButtonEasy.setOutlineColor(sf::Color::Yellow);
    levelPlayButtonEasy.setPosition(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.25);

    levelPlayButtonMedium.setFont(font);
    levelPlayButtonMedium.setString("Medium");
    levelPlayButtonMedium.setCharacterSize(40);
    levelPlayButtonMedium.setFillColor(sf::Color::White);
    levelPlayButtonMedium.setOutlineThickness(2);
    levelPlayButtonMedium.setOutlineColor(sf::Color::Yellow);
    levelPlayButtonMedium.setPosition(SCREEN_WIDTH * 0.65, SCREEN_HEIGHT * 0.25);

    levelPlayButtonHard.setFont(font);
    levelPlayButtonHard.setString("Hard");
    levelPlayButtonHard.setCharacterSize(40);
    levelPlayButtonHard.setFillColor(sf::Color::White);
    levelPlayButtonHard.setOutlineThickness(2);
    levelPlayButtonHard.setOutlineColor(sf::Color::Yellow);
    levelPlayButtonHard.setPosition(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.65);

    levelPlayButtonInfinite.setFont(font);
    levelPlayButtonInfinite.setString("Infinite");
    levelPlayButtonInfinite.setCharacterSize(40);
    levelPlayButtonInfinite.setFillColor(sf::Color::White);
    levelPlayButtonInfinite.setOutlineThickness(2);
    levelPlayButtonInfinite.setOutlineColor(sf::Color::Yellow);
    levelPlayButtonInfinite.setPosition(SCREEN_WIDTH * 0.65, SCREEN_HEIGHT * 0.65);

}

void Game::initializeTextures()
{
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }
    if (!backgroundTexture.loadFromFile("Background.png"))
    {
        std::cerr << "Failed to load background!" << std::endl;
    }
    if (!playerTexture.loadFromFile("player.png"))
    {
        std::cerr << "Failed to load player texture!" << std::endl;
    }
    if (!shockwaveTexture.loadFromFile("shockwave.png"))
    {
        std::cerr << "Failed to load shockwave texture!" << std::endl;
    }
    if (!shockwaveScreenOutlineTexture.loadFromFile("ShockwaveScreenOutlineTexture.png"))
    {
        std::cerr << "Failed to load shockwave screen outline texture!" << std::endl;
    }
    if (!levelTabletTexture.loadFromFile("levelTabletUpscaled.png"))
    {
        std::cerr << "Failed to load tablet texture!" << std::endl;
    }
}

void Game::initializeRectangles()
{
    background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    background.setTexture(&backgroundTexture);

    player.setPosition(SCREEN_WIDTH / 2 - player.getSize().x / 2, SCREEN_HEIGHT / 2 - player.getSize().y / 2);
    player.setTexture(&playerTexture);

    shockwave.setRadius(player.getSize().x * 2);
    shockwave.setPosition(player.getPosition().x - 1.5 * player.getSize().x, player.getPosition().y - 1.5 * player.getSize().y);
    shockwave.setTexture(&shockwaveTexture);

    shockwaveScreenOutline.setPosition(0, 0);
    shockwaveScreenOutline.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    shockwaveScreenOutline.setTexture(&shockwaveScreenOutlineTexture);

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

    healthText.setFont(font);
    healthText.setCharacterSize(15);
    healthText.setFillColor(sf::Color::White);
    healthText.setOutlineColor(sf::Color::Black);
    healthText.setPosition(healthBarBorder.getPosition().x + healthBarBorder.getSize().x * 0.4, healthBarBorder.getPosition().y);
    healthText.setString(std::to_string(static_cast<int>(playerHealth)));

    manaText.setFont(font);
    manaText.setCharacterSize(15);
    manaText.setFillColor(sf::Color::White);
    manaText.setOutlineColor(sf::Color::Black);
    manaText.setPosition(manaBarBorder.getPosition().x + manaBarBorder.getSize().x * 0.4, manaBarBorder.getPosition().y);
    manaText.setString(std::to_string(static_cast<int>(playerMana)));

    moneyText.setPosition(healthBarBorder.getPosition().x, healthBarBorder.getPosition().y + 60);

    levelTablet.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    levelTablet.setPosition(0, 0);
	levelTablet.setTexture(&levelTabletTexture);
}

// Drawing health bar
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
	healthText.setString(std::to_string(static_cast<int>(playerHealth)));
	window.draw(healthBar);
	window.draw(healthBarBorder);
	window.draw(healthText);
}

// Drawing mana bar
void Game::drawManaBar(sf::RenderWindow& window)
{
    manaBar.setSize(sf::Vector2f(manaBarBorder.getSize().x * (playerMana / maxPlayerMana), manaBar.getSize().y));
    manaBar.setFillColor((playerMana >= maxPlayerMana) ? sf::Color::Magenta : sf::Color::Cyan);
	manaText.setString(std::to_string(static_cast<int>(playerMana)));
	window.draw(manaBar);
	window.draw(manaBarBorder); 
	window.draw(manaText);
}

void Game::drawMoneyText(sf::RenderWindow& window)
{
	moneyText.setString(std::to_string(playerMoney) + " $");
	window.draw(moneyText);
}

void Game::hightlightHower(sf::Text& button, sf::Vector2f mousePos)
{
	if (button.getGlobalBounds().contains(mousePos))
	{
		button.setOutlineColor(sf::Color::Green);
	}
	else
	{
		button.setOutlineColor(sf::Color::White);
	}
}


// Applying border damage
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

// Executing mana ability
void Game::activateManaAbility()
{
	if (playerMana >= maxPlayerMana)
	{
		playerMana = 0.f;
		Enemy::manaAbilityDamage(player.getPosition());
		shockwaveRenderTime = gameTime + .5f;
	}
}

void Game::resetGameSettings()
{
    gameTime = .0f;
    borderDamage = 1.0f;
    playerHealth = defaultPlayerHealth;
    playerSpeed = defaultPlayerSpeed;
    playerMoney = 0;
    playerMana = 0;
    maxPlayerMana = defaultPlayerManaMax;
    enemiesPerWave = 3;
    enemySpawnTimer = 9.f;
    enemySpawnTimerMax = 10.f;
    bulletSpawnTimer = 0.f;
    bulletSpawnTimerMax = 0.5f;
    enemiesPerWave = 5;
    bossSpawnTimer = 0.f;
    bossSpawnTimerMax = 30.f;
    enemySpawnTimer = 0.f;
    shockwaveRenderTime = 0.f;
    shopOpeningCooldown = 0.f;
    healthUpgradeCost = 10.f;
    movementSpeedUpgradeCost = 10.f;
    firingSpeedUpgradeCost = 10.f;
    healthUpgradeButtonCost.setString(std::to_string(static_cast<int>(healthUpgradeCost)));
    movementSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(movementSpeedUpgradeCost)));
    firingSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(firingSpeedUpgradeCost)));
    player.setPosition(SCREEN_WIDTH / 2 - player.getSize().x / 2, SCREEN_HEIGHT / 2 - player.getSize().y / 2);
    Enemy::enemyList.clear();
    Bullet::bulletList.clear();
}

// Updating the game (in game state)
void Game::update(float deltaTime) 
{
    Enemy::update(deltaTime, player.getPosition());
    playerHealth -= Enemy::checkPlayerTouch(player, playerHealth);
	playerHealth -= applyBorderDamage();
    enemySpawnTimerMax = 4.f - 3 * (gameTime / 60.f);
    if (gameTime < winningSurvivalTime)
    {
        enemySpawnTimer += Enemy::trySpawn(enemySpawnTimer, enemySpawnTimerMax, deltaTime, enemiesPerWave, difficulty, gameTime);
    }

    if (bossSpawnTimer >= bossSpawnTimerMax)
    {
        new Enemy(50, 100, sf::Vector2f(-SCREEN_WIDTH / 10, SCREEN_HEIGHT / 2), 1000);
        bossSpawnTimer = 0.f;
    }
    else
    {
        bossSpawnTimer += deltaTime;
    }
    if (gameTime >= winningSurvivalTime && Enemy::enemyList.size() == 0 || sf::Keyboard::isKeyPressed(sf::Keyboard::F))
    {
        gameOverTitle.setOutlineColor(sf::Color::Green);
        gameOverTitle.setString("You win!");
        gameOverTitle.setPosition(SCREEN_WIDTH * 0.38, SCREEN_HEIGHT / 2 - 180);
        state = GameState::GAME_OVER;
    }

    Bullet::update(deltaTime);
    Bullet::checkRemove(window);
    Bullet::checkCollisions(window);
    playerMoney += Enemy::hitRemove();
    playerMana += Bullet::hitRemove();

	if (playerMana > maxPlayerMana)
	{
		playerMana = maxPlayerMana;
	}

    if (playerHealth <= 0) 
    {
        gameOverTitle.setOutlineColor(sf::Color::Red);
        gameOverTitle.setString("Game Over!");
        gameOverTitle.setPosition(SCREEN_WIDTH * 0.33, SCREEN_HEIGHT / 2 - 180);
        state = GameState::GAME_OVER;
    }

	gameTime += deltaTime;
}

// Rendering GameStates
void Game::renderMenu() 
{
    window.clear();

    menuContinueButton.setFillColor(continueAvailable ? sf::Color::White : sf::Color::Black);

	window.draw(background);
	window.draw(title);
    window.draw(menuContinueButton);
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

    window.draw(background);
    if (gameTime <= shockwaveRenderTime)
    {
        shockwave.setPosition(player.getPosition().x - 1.5 * player.getSize().x, player.getPosition().y - 1.5 * player.getSize().y);
        window.draw(shockwave);
		window.draw(shockwaveScreenOutline);
    }
    window.draw(player);
    Enemy::drawAll(window);
    Bullet::drawAll(window);
    Particles::drawAll(window);
	drawHealthBar(window);
	drawManaBar(window);
	drawMoneyText(window);

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

void Game::renderLevel()
{
	window.clear();
	window.draw(background);
	window.draw(levelTablet);
	window.draw(levelBackButton);
	window.draw(levelPlayButton);
    window.draw(levelPlayButtonEasy);
    window.draw(levelPlayButtonMedium);
    window.draw(levelPlayButtonHard);
    window.draw(levelPlayButtonInfinite);
    Ad::drawAll(window);
	window.display();
}

