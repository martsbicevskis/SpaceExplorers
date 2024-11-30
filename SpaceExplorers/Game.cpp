#include "Game.h"
#include <iostream>

Game::Game() :
    window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Ekrâns", sf::Style::Default),
	gameTime(.0f),
	borderDamage(1.0f),
    planet(50.f),
	healthBarBorder(sf::Vector2f(100.0f, 20.0f)),
	manaBarBorder(sf::Vector2f(100.0f, 20.0f)),
	healthBar(sf::Vector2f(100.0f, 20.0f)),
	manaBar(sf::Vector2f(100.0f, 20.0f)),
    player(sf::Vector2f(50.0f, 50.0f)),
    playerHealth(maxPlayerHealth),
    playerMoney(0),
    enemySpawnTimer(4.f),
    enemySpawnTimerMax(5.f),
    bulletSpawnTimer(0.f),
    bulletSpawnTimerMax(0.05f),
    state(GameState::MENU)

{
    player.setPosition(SCREEN_WIDTH / 2 - player.getSize().x / 2, SCREEN_HEIGHT / 2 - player.getSize().y / 2);
	
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


    // Load font
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    moneyText.setFont(font);
    moneyText.setString("Play");
    moneyText.setCharacterSize(30);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setOutlineThickness(2);
    moneyText.setOutlineColor(sf::Color::White);
    moneyText.setPosition(SCREEN_WIDTH - 100, 100);

    // Set up Play button
    playButton.setFont(font);
    playButton.setString("Play");
    playButton.setCharacterSize(50);
    playButton.setFillColor(sf::Color::White);
    playButton.setOutlineThickness(2);
    playButton.setOutlineColor(sf::Color::White);
    playButton.setPosition(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 75);

    // Set up Settings button
    settingsButton.setFont(font);
    settingsButton.setString("Settings");
    settingsButton.setCharacterSize(50);
    settingsButton.setFillColor(sf::Color::White);
    settingsButton.setOutlineThickness(2);
    settingsButton.setOutlineColor(sf::Color::White);
    settingsButton.setPosition(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2);

    // Set up Exit button
    exitButton.setFont(font);
    exitButton.setString("Exit");
    exitButton.setCharacterSize(50);
    exitButton.setFillColor(sf::Color::White);
    exitButton.setOutlineThickness(2);
    exitButton.setOutlineColor(sf::Color::White);
    exitButton.setPosition(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 75);

    // Set up Back button for Settings
    backButton.setFont(font);
    backButton.setString("Back");
    backButton.setCharacterSize(50);
    backButton.setFillColor(sf::Color::White);
    backButton.setOutlineThickness(2);
    backButton.setOutlineColor(sf::Color::White);
    backButton.setPosition(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2);

    // Pause Menu UI
    pauseTitle.setFont(font);
    pauseTitle.setString("Game Paused");
    pauseTitle.setCharacterSize(60);
    pauseTitle.setFillColor(sf::Color::White);
    pauseTitle.setPosition(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 150);

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

    // Game Over Menu UI
    gameOverTitle.setFont(font);
    gameOverTitle.setString("Game Over!");
    gameOverTitle.setCharacterSize(60);
    gameOverTitle.setFillColor(sf::Color::White);
    gameOverTitle.setPosition(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 150);

    restartButton.setFont(font);
    restartButton.setString("Restart");
    restartButton.setCharacterSize(50);
    restartButton.setFillColor(sf::Color::White);
    restartButton.setOutlineThickness(2);
    restartButton.setOutlineColor(sf::Color::White);
    restartButton.setPosition(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50);

    gameOverMainMenuButton.setFont(font);
    gameOverMainMenuButton.setString("Main Menu");
    gameOverMainMenuButton.setCharacterSize(50);
    gameOverMainMenuButton.setFillColor(sf::Color::White);
    gameOverMainMenuButton.setOutlineThickness(2);
    gameOverMainMenuButton.setOutlineColor(sf::Color::White);
    gameOverMainMenuButton.setPosition(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 50);
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        switch (state) {
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
        }
    }
}

void Game::handleMenuInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Detect mouse clicks on buttons
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (playButton.getGlobalBounds().contains(mousePos)) {
                state = GameState::PLAY; // Switch to gameplay
            }
            else if (settingsButton.getGlobalBounds().contains(mousePos)) {
                state = GameState::SETTINGS; // Switch to settings menu
            }
            else if (exitButton.getGlobalBounds().contains(mousePos)) {
                window.close(); // Exit the game
            }
        }
    }
}

void Game::handlePauseInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            state = GameState::PLAY;
        }
        // Detect mouse clicks on Pause Menu buttons
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (continueButton.getGlobalBounds().contains(mousePos)) {
                state = GameState::PLAY; // Resume the game
            }
            else if (mainMenuButton.getGlobalBounds().contains(mousePos)) {
                state = GameState::MENU; // Go back to main menu
            }
        }
    }
}

void Game::handleGameOverInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Detect mouse clicks on Game Over Menu buttons
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (restartButton.getGlobalBounds().contains(mousePos)) {
                state = GameState::PLAY; // Restart the game
				gameTime = .0f;
				borderDamage = 1.0f;
                playerHealth = maxPlayerHealth;
                playerMoney = 0;
                enemySpawnTimer = 0.f;
                bulletSpawnTimer = 0.f;
                player.setPosition(SCREEN_WIDTH / 2 - player.getSize().x / 2, SCREEN_HEIGHT / 2 - player.getSize().y / 2);
                Enemy::enemyList.clear();
                Bullet::bulletList.clear();
            }
            else if (gameOverMainMenuButton.getGlobalBounds().contains(mousePos)) {
                state = GameState::MENU; // Go back to main menu
            }
        }
    }
}

void Game::renderPause() {
    window.clear();
    window.draw(pauseTitle);
    window.draw(continueButton);
    window.draw(mainMenuButton);
    window.display();
}

void Game::renderGameOver() {
    window.clear();
    window.draw(gameOverTitle);
    window.draw(restartButton);
    window.draw(gameOverMainMenuButton);
    window.display();
}

void Game::handleSettingsInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Detect Back button click
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (backButton.getGlobalBounds().contains(mousePos)) {
                state = GameState::MENU; // Return to menu
            }
        }
    }
}

void Game::handleGameInput(float deltaTime) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Pause the game when Esc is pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            state = GameState::PAUSE;
        }
    }

    // Game-specific input logic (player movement, shooting, etc.)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        player.move(-500.0f * deltaTime, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        player.move(0.0f, 500.0f * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        player.move(500.0f * deltaTime, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        player.move(0.0f, -500.0f * deltaTime);
    }

    // Spawn bullets
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        bulletSpawnTimer += Bullet::trySpawn(player.getPosition(), window, bulletSpawnTimer, bulletSpawnTimerMax, deltaTime);
    }
}

void Game::drawHealthBar(sf::RenderWindow& window)
{
	healthBar.setFillColor(sf::Color::Red);
	window.draw(healthBar);
    if (playerHealth > maxPlayerHealth)
    {
        healthBar.setSize(sf::Vector2f(healthBarBorder.getSize().x, healthBarBorder.getSize().y));
    }
    else
    {
        healthBar.setSize(sf::Vector2f(manaBarBorder.getSize().x * (playerHealth / maxPlayerHealth), healthBar.getSize().y));
    }
    healthBar.setFillColor(sf::Color::Green);
	window.draw(healthBar);
	window.draw(healthBarBorder);
}

void Game::drawManaBar(sf::RenderWindow& window)
{
	manaBar.setFillColor(sf::Color::Blue);
	window.draw(manaBar);
	manaBar.setSize(sf::Vector2f(manaBarBorder.getSize().x * (playerMoney / maxPlayerHealth), manaBar.getSize().y));
	manaBar.setFillColor(sf::Color::Cyan);
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
	if (player.getPosition().x < 0.0f)
	{
		damage = .1f * borderDamage;
	}
	else if (player.getPosition().x + player.getSize().x > SCREEN_WIDTH)
	{
		damage = .1f * borderDamage;
	}
	if (player.getPosition().y < 0.0f)
	{
		damage = .1f * borderDamage;
	}
	else if (player.getPosition().y + player.getSize().y > SCREEN_HEIGHT)
	{
		damage = .1f * borderDamage;
	}
	return damage;
}

void Game::update(float deltaTime) {
    // Update enemies and bullets
    Enemy::update(deltaTime, player.getPosition());
    playerHealth -= Enemy::checkPlayerTouch(player, playerHealth);
	playerHealth -= applyBorderDamage();
    enemySpawnTimer += Enemy::trySpawn(enemySpawnTimer, enemySpawnTimerMax, deltaTime);


    Bullet::update(deltaTime);
    Bullet::checkRemove(window);
    playerMoney += Bullet::checkCollisions(window);

    // Check for game over
    if (playerHealth <= 0) {
        state = GameState::GAME_OVER;
    }

	gameTime += deltaTime;
    std::cout << gameTime << std::endl;
}

void Game::renderMenu() {
    window.clear();
    window.draw(playButton);
    window.draw(settingsButton);
    window.draw(exitButton);
    window.display();
}

void Game::renderSettings() {
    window.clear();
    window.draw(backButton);
    window.display();
}

void Game::renderGame() {
    window.clear();

    // Draw game objects
    window.draw(planet);
    window.draw(player);
	drawHealthBar(window);
	drawManaBar(window);
	drawMoneyText(window);
    Enemy::drawAll(window);
    Bullet::drawAll(window);

    window.display();
}

