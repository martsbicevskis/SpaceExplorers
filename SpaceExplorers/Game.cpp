#include "Game.h"
#include <iostream>
#include <cmath>

// -------------------------------------------- Game constructor --------------------------------------------

Game::Game() :
    //initializing the game variables
    window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Ekrāns", sf::Style::Default),
    planet(50.f),
    healthBarBorder(sf::Vector2f(100.0f, 20.0f)),
    manaBarBorder(sf::Vector2f(100.0f, 20.0f)),
    healthBar(sf::Vector2f(100.0f, 20.0f)),
    manaBar(sf::Vector2f(100.0f, 20.0f)),
    player(sf::Vector2f(playerSize, playerSize)),
    globalMoney(1000),
    difficulty(1.f),
    winningSurvivalTime(70.f),
    exitPanelOpeningCooldown(0.f),
    GM_shopMouseHeld(false),
    continueAvailable(false),
    exitPanelOpen(false),
    GM_shopUpgradeButtonSize(
        SCREEN_WIDTH / 8,
        SCREEN_HEIGHT / 8),
    defaultPlayerModelPosition(
        SCREEN_WIDTH / 2 - playerSize / 2, 
        SCREEN_HEIGHT / 5 * 4 - playerSize / 2),
    shotMode(ShotMode::RAPID),
    state(GameState::MENU)
        
{
    resetGameSettings();
    initializeTextures();
    initializeButtons();
	initializeRectangles();    
}

// -------------------------------------------- Main Game Loop --------------------------------------------

void Game::run()
{
    while (window.isOpen()) 
    {
        float deltaTime = clock.restart().asSeconds();

        switch (state) 
        {
        case GameState::MENU:
            handleMenuInput(deltaTime);
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
        case GameState::CONTROLS:
            handleControlsInput();
            renderControls();
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
        case GameState::GM_GAMEMENU:
            handleGMGameMenuInput();
            renderGMGameMenu();
            break;
        case GameState::GM_LEVEL:
            handleLevelInput();
            renderLevel();
            break;
        case GameState::GM_SHOP:
            handleGMShopInput();
            renderGMShop();
            break;
        }
    }
}
// -------------------------------------------- INPUT HANDLING --------------------------------------------

// --------------------------------------------Input handling (MENU)-----------------------------------------

void Game::handleMenuInput(float deltaTime)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && exitPanelOpeningCooldown > globalPanelOpeningCooldownMax)
        {
            exitPanelOpeningCooldown = 0.f;
            if (!exitPanelOpen) exitPanelOpen = true;
            else                exitPanelOpen = false;     
        }
        
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) && 
            exitPanelOpen)
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (playButton.getGlobalBounds().contains(mousePos)) 
            {
                state = GameState::GM_GAMEMENU;
            }
            else if (menuContinueButton.getGlobalBounds().contains(mousePos) && continueAvailable)
            {
                state = GameState::PLAY;
            }
            else if (controlsButton.getGlobalBounds().contains(mousePos))
            {
                state = GameState::CONTROLS;
            }
            else if (settingsButton.getGlobalBounds().contains(mousePos))
            {
                state = GameState::SETTINGS;
            }
            else if (exitButton.getGlobalBounds().contains(mousePos)) 
            {
                exitPanelOpen = true;
                
            }
            if (exitPanelOpen)
            {
                if (exitPanelYesButton.getGlobalBounds().contains(mousePos))
                {
                    window.close();
                }
                if (exitPanelNoButton.getGlobalBounds().contains(mousePos))
                {
                    exitPanelOpen = false;
                }
            }
        }
    }
    if (exitPanelOpeningCooldown < globalPanelOpeningCooldownMax)
    {
        exitPanelOpeningCooldown += deltaTime;
    }
	sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));

    if (continueAvailable) hightlightHower(menuContinueButton, mousePos);
	hightlightHower(playButton, mousePos);
	hightlightHower(settingsButton, mousePos);
    hightlightHower(controlsButton, mousePos);
	hightlightHower(exitButton, mousePos);
    hightlightHower(exitPanelYesButton, mousePos);
    hightlightHower(exitPanelNoButton, mousePos);
}

//-------------------------------------------- Input handling (PAUSE) --------------------------------------------

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
            if (shopOpeningCooldown >= globalPanelOpeningCooldownMax)
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

    if (shopOpeningCooldown < globalPanelOpeningCooldownMax)
    {
        shopOpeningCooldown += deltaTime;
    }
}

void Game::handleControlsInput()
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
			state = GameState::MENU;
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (controlsExitButton.getGlobalBounds().contains(mousePos))
            {
                state = GameState::MENU;
            }
        }
    }
 
    sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));

    hightlightHower(controlsExitButton, mousePos);
}

// -------------------------------------------- Input handling (GAME OVER) --------------------------------------------

void Game::handleGameOverInput() 
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
            state = GameState::MENU;
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

// -------------------------------------------- Input handling (SHOP) --------------------------------------------

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
            if (shopOpeningCooldown >= globalPanelOpeningCooldownMax)
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
                if (gameMoney >= healthUpgradeCost) 
                {
                    gameMoney -= healthUpgradeCost;
                    healthUpgradeCost *= 1.2;
                    playerHealth += 20;
					healthUpgradeButtonCost.setString(std::to_string(static_cast<int>(healthUpgradeCost)));
                }
            }
            else if (movementSpeedUpgradeButton.getGlobalBounds().contains(mousePos) || movementSpeedUpgradeButtonCost.getGlobalBounds().contains(mousePos))
            {
                if (gameMoney >= movementSpeedUpgradeCost) 
                {
                    gameMoney -= movementSpeedUpgradeCost;
                    movementSpeedUpgradeCost *= 1.2;
                    playerSpeed += 50;
					movementSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(movementSpeedUpgradeCost)));
                }
			}
            else if (firingSpeedUpgradeButton.getGlobalBounds().contains(mousePos) || firingSpeedUpgradeButtonCost.getGlobalBounds().contains(mousePos))
            {
                if (gameMoney >= firingSpeedUpgradeCost) 
                {
                    gameMoney -= firingSpeedUpgradeCost;
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
        && gameMoney >= healthUpgradeCost)
	{
		healthUpgradeButton.setOutlineColor(sf::Color::Green);
	}
	else if(gameMoney >= healthUpgradeCost)
	{
		healthUpgradeButton.setOutlineColor(sf::Color::White);
	}
	else
	{
		healthUpgradeButton.setOutlineColor(sf::Color::Red);
	}
	if ((movementSpeedUpgradeButton.getGlobalBounds().contains(mousePos)
		|| movementSpeedUpgradeButtonCost.getGlobalBounds().contains(mousePos))
		&& gameMoney >= movementSpeedUpgradeCost)
	{
		movementSpeedUpgradeButton.setOutlineColor(sf::Color::Green);
	}
	else if (gameMoney >= movementSpeedUpgradeCost)
	{
		movementSpeedUpgradeButton.setOutlineColor(sf::Color::White);
	}
	else
	{
		movementSpeedUpgradeButton.setOutlineColor(sf::Color::Red);
	}
	if ((firingSpeedUpgradeButton.getGlobalBounds().contains(mousePos)
		|| firingSpeedUpgradeButtonCost.getGlobalBounds().contains(mousePos))
		&& gameMoney >= firingSpeedUpgradeCost)
	{
		firingSpeedUpgradeButton.setOutlineColor(sf::Color::Green);
	}
	else if (gameMoney >= firingSpeedUpgradeCost)
	{
		firingSpeedUpgradeButton.setOutlineColor(sf::Color::White);
	}
	else
	{
		firingSpeedUpgradeButton.setOutlineColor(sf::Color::Red);
	}
    if (shopOpeningCooldown < globalPanelOpeningCooldownMax)
    {
        shopOpeningCooldown += deltaTime;
    }
	updateMoneyText();
    updateHealthBar();
}
// -------------------------------------------- Input handling (LEVEL) --------------------------------------------

void Game::handleLevelInput()
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
            state = GameState::GM_GAMEMENU;
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (levelBackButton.getGlobalBounds().contains(mousePos))
            {
                state = GameState::GM_GAMEMENU;
            }
            if (Ad::adList[0].body.getGlobalBounds().contains(mousePos))
            {
                difficulty = 1.f;
                winningSurvivalTime = 70.f;
                continueAvailable = true;
                resetGameSettings();
                state = GameState::PLAY;
            }
            if (Ad::adList[1].body.getGlobalBounds().contains(mousePos))
            {
                difficulty = 2.f;
                winningSurvivalTime = 70.f;
                continueAvailable = true;
                resetGameSettings();
                state = GameState::PLAY;
            }
            if (Ad::adList[2].body.getGlobalBounds().contains(mousePos))
            {
                difficulty = 3.f;
                winningSurvivalTime = 70.f;
                continueAvailable = true;
                resetGameSettings();
                state = GameState::PLAY;
            }
            if (Ad::adList[3].body.getGlobalBounds().contains(mousePos))
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
}

// -------------------------------------------- Input handling (SETTINGS) --------------------------------------------


void Game::handleSettingsInput() 
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
            state = GameState::MENU;
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


// -------------------------------------------- Input handling (GM_gameMenu) --------------------------------------------

void Game::handleGMGameMenuInput()
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
            state = GameState::MENU;
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (GM_levelButton.getGlobalBounds().contains(mousePos))
            {
                Ad(1, shockwaveScreenOutlineTexture);
                Ad(2, shockwaveScreenOutlineTexture);
                Ad(3, shockwaveScreenOutlineTexture);
                Ad(4, shockwaveScreenOutlineTexture);

                state = GameState::GM_LEVEL;
            }
            else if (GM_shopButton.getGlobalBounds().contains(mousePos))
            {
                setDefaultGMShopObjectPositions();
                state = GameState::GM_SHOP;
            }
            else if (GM_backButton.getGlobalBounds().contains(mousePos))
            {
                state = GameState::MENU;
            }
            else if (GM_specialMisionsButton.getGlobalBounds().contains(mousePos))
            {
                Ad(1, shockwaveScreenOutlineTexture);
                Ad(2, shockwaveScreenOutlineTexture);
                Ad(3, shockwaveScreenOutlineTexture);
                Ad(4, shockwaveScreenOutlineTexture);

                state = GameState::GM_LEVEL;
            }
        }
    }
    sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));

    hightlightHower(GM_levelButton, mousePos);
    hightlightHower(GM_shopButton, mousePos);
    hightlightHower(GM_backButton, mousePos);
    hightlightHower(GM_specialMisionsButton, mousePos);
}


// -------------------------------------------- Input handling (GM_shop) --------------------------------------------

void Game::handleGMShopInput()
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
            state = GameState::GM_GAMEMENU;
        }

        // Left mouse pressed -> either click UI or start dragging
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            // If Back button clicked, go back to GM menu and do not start dragging
            if (GM_shopBackButton.getGlobalBounds().contains(mousePos))
            {
                state = GameState::GM_GAMEMENU;
                GM_shopMouseHeld = false;
            }
            else
            {
                // Start dragging and store current mouse position
                GM_shopMouseHeld = true;
                GM_shopMousePos = mousePos;
            }
        }
        // Left mouse released -> stop dragging
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            GM_shopMouseHeld = false;
        }
        // Mouse moved -> if dragging, move model by delta
        else if (event.type == sf::Event::MouseMoved)
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));

            if (GM_shopMouseHeld)
            {
                sf::Vector2f delta = mousePos - GM_shopMousePos;
                GM_shopMousePos = mousePos;
                for (auto* i : GM_shopMovableList)
                {
                    i->move(delta);
                }
            }

            // highlight back button while moving
            hightlightHower(GM_shopBackButton, mousePos);
        }
    }

    // Also update hover state when there are no events
    sf::Vector2f tempMousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));
    hightlightHower(GM_shopBackButton, tempMousePos);
}

// -------------------------------------------- Input handling (Game) --------------------------------------------

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
            if (shopOpeningCooldown >= globalPanelOpeningCooldownMax)
            {
                state = GameState::PAUSE;
                shopOpeningCooldown = 0.f;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
        {
            if (shopOpeningCooldown >= globalPanelOpeningCooldownMax)
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
    {
        shotMode = ShotMode::PIERCING;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        // Defining bullets spawn position for ALL CASES
        sf::Vector2f spawnPos(
            player.getPosition().x + (playerSize - defaultBulletSize) / 2,
            player.getPosition().y + (playerSize - defaultBulletSize) / 2
        );

        switch (shotMode)
        {
            case Game::ShotMode::RAPID:
                bulletSpawnTimer += Bullet::trySpawnRapid(
                    spawnPos,
                    window,
                    bulletSpawnTimer,
                    bulletSpawnTimerMax,
                    deltaTime
                );
                break;


            case Game::ShotMode::SHOTGUN:
                bulletSpawnTimer += Bullet::trySpawnShotgun(
                    spawnPos,
                    window,
                    bulletSpawnTimer,
                    bulletSpawnTimerMax,
                    deltaTime
                );
                break;


            case Game::ShotMode::BOMB:
                bulletSpawnTimer += Bullet::trySpawnBomb(
                    spawnPos,
                    window, bulletSpawnTimer,
                    bulletSpawnTimerMax,
                    deltaTime
                );
                break;


            case Game::ShotMode::PIERCING:
                bulletSpawnTimer += Bullet::trySpawnPiercing(
                    spawnPos,
                    window,
                    bulletSpawnTimer,
                    bulletSpawnTimerMax,
                    deltaTime
                );
                break;


            default:
                break;
        }
    }
	if (shopOpeningCooldown < globalPanelOpeningCooldownMax)
	{
		shopOpeningCooldown += deltaTime;
	}
}
// -------------------------------------------- Initializing Buttons --------------------------------------------

void Game::initializeButtons()
{
    // Money Display (Game)
    moneyText.setFont(font);
    moneyText.setCharacterSize(30);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setOutlineThickness(2);
    moneyText.setOutlineColor(sf::Color::White);
    moneyText.setPosition(
        SCREEN_WIDTH - 100,
        100
    );

    // Title display (Main Menu)
    title.setFont(font);
    title.setString("Space Explorers");
    title.setCharacterSize(100);
    title.setFillColor(sf::Color::White);
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color::Cyan);
    title.setPosition(
        SCREEN_WIDTH * 0.22,
        SCREEN_HEIGHT / 8
    );

    // Continue button (Main Menu)
    menuContinueButton.setFont(font);
    menuContinueButton.setString("Continue");
    menuContinueButton.setCharacterSize(50);
    menuContinueButton.setFillColor(sf::Color::White);
    menuContinueButton.setOutlineThickness(2);
    menuContinueButton.setOutlineColor(sf::Color::White);
    menuContinueButton.setPosition(
        SCREEN_WIDTH / 2 - 70,
        SCREEN_HEIGHT / 2 - 80
    );

    // Play button (Main Menu)
    playButton.setFont(font);
    playButton.setString("New Game");
    playButton.setCharacterSize(50);
    playButton.setFillColor(sf::Color::White);
    playButton.setOutlineThickness(2);
    playButton.setOutlineColor(sf::Color::White);
    playButton.setPosition(
        SCREEN_WIDTH / 2 - 70,
        SCREEN_HEIGHT / 2 - 0
    );

    // Settings button (Main Menu)
    settingsButton.setFont(font);
    settingsButton.setString("Settings");
    settingsButton.setCharacterSize(50);
    settingsButton.setFillColor(sf::Color::White);
    settingsButton.setOutlineThickness(2);
    settingsButton.setOutlineColor(sf::Color::White);
    settingsButton.setPosition(
        SCREEN_WIDTH / 2 - 70,
        SCREEN_HEIGHT / 2 + 160
    );

    // Controls button (Main Menu)
    controlsButton.setFont(font);
    controlsButton.setString("Controls");
    controlsButton.setCharacterSize(50);
    controlsButton.setFillColor(sf::Color::White);
    controlsButton.setOutlineThickness(2);
    controlsButton.setOutlineColor(sf::Color::White);
    controlsButton.setPosition(
        SCREEN_WIDTH / 2 - 70,
        SCREEN_HEIGHT / 2 + 80
    );

    // Exit button (Main Menu)
    exitButton.setFont(font);
    exitButton.setString("Exit");
    exitButton.setCharacterSize(50);
    exitButton.setFillColor(sf::Color::White);
    exitButton.setOutlineThickness(2);
    exitButton.setOutlineColor(sf::Color::White);
    exitButton.setPosition(
        SCREEN_WIDTH / 2 - 70,
        SCREEN_HEIGHT / 2 + 240
    );

    // Continue button (Main Menu)
    exitPanelYesButton.setFont(font);
    exitPanelYesButton.setString("Yes");
    exitPanelYesButton.setCharacterSize(50);
    exitPanelYesButton.setFillColor(sf::Color::White);
    exitPanelYesButton.setOutlineThickness(2);
    exitPanelYesButton.setOutlineColor(sf::Color::White);
    exitPanelYesButton.setPosition(
        SCREEN_WIDTH / 100 * 40,
        SCREEN_HEIGHT / 100 * 50
    );

    // Continue button (Main Menu)
    exitPanelNoButton.setFont(font);
    exitPanelNoButton.setString("No");
    exitPanelNoButton.setCharacterSize(50);
    exitPanelNoButton.setFillColor(sf::Color::White);
    exitPanelNoButton.setOutlineThickness(2);
    exitPanelNoButton.setOutlineColor(sf::Color::White);
    exitPanelNoButton.setPosition(
        SCREEN_WIDTH / 100 * 60,
        SCREEN_HEIGHT / 100 * 50
    );

    // Continue button (Main Menu)
    exitPanelTitle.setFont(font);
    exitPanelTitle.setString("Are you sure you want to exit?");
    exitPanelTitle.setCharacterSize(42);
    exitPanelTitle.setFillColor(sf::Color::White);
    exitPanelTitle.setOutlineThickness(2);
    exitPanelTitle.setOutlineColor(sf::Color::White);
    exitPanelTitle.setPosition(
        SCREEN_WIDTH / 100 * 30,
        SCREEN_HEIGHT / 100 * 40
    );


    // Back button (Settings)
    backButton.setFont(font);
    backButton.setString("Back");
    backButton.setCharacterSize(50);
    backButton.setFillColor(sf::Color::White);
    backButton.setOutlineThickness(2);
    backButton.setOutlineColor(sf::Color::White);
    backButton.setPosition(
        SCREEN_WIDTH / 2 - 50,
        SCREEN_HEIGHT / 2
    );

    // Pause Menu (Pause)
    pauseTitle.setFont(font);
    pauseTitle.setString("Game Paused");
    pauseTitle.setCharacterSize(60);
    pauseTitle.setFillColor(sf::Color::White);
    pauseTitle.setOutlineThickness(2);
    pauseTitle.setOutlineColor(sf::Color::Cyan);
    pauseTitle.setPosition(
        SCREEN_WIDTH / 2 - 190,
        SCREEN_HEIGHT / 2 - 150
    );

    // Continue button (Pause)
    continueButton.setFont(font);
    continueButton.setString("Continue");
    continueButton.setCharacterSize(50);
    continueButton.setFillColor(sf::Color::White);
    continueButton.setOutlineThickness(2);
    continueButton.setOutlineColor(sf::Color::White);
    continueButton.setPosition(
        SCREEN_WIDTH / 2 - 100,
        SCREEN_HEIGHT / 2 - 50
    );

    // Main Menu button (Pause)
    mainMenuButton.setFont(font);
    mainMenuButton.setString("Main Menu");
    mainMenuButton.setCharacterSize(50);
    mainMenuButton.setFillColor(sf::Color::White);
    mainMenuButton.setOutlineThickness(2);
    mainMenuButton.setOutlineColor(sf::Color::White);
    mainMenuButton.setPosition(
        SCREEN_WIDTH / 2 - 120,
        SCREEN_HEIGHT / 2 + 50
    );

    // Game Over title (Game Over)
    gameOverTitle.setFont(font);
    gameOverTitle.setString("Game Over!");
    gameOverTitle.setCharacterSize(80);
    gameOverTitle.setFillColor(sf::Color::White);
    gameOverTitle.setOutlineThickness(2);
    gameOverTitle.setOutlineColor(sf::Color::Red);
    gameOverTitle.setPosition(
        SCREEN_WIDTH * 0.32,
        SCREEN_HEIGHT / 2 - 180
    );

    // Restart button (Game Over)
    restartButton.setFont(font);
    restartButton.setString("Restart");
    restartButton.setCharacterSize(50);
    restartButton.setFillColor(sf::Color::White);
    restartButton.setOutlineThickness(2);
    restartButton.setOutlineColor(sf::Color::White);
    restartButton.setPosition(
        SCREEN_WIDTH / 2 - 90,
        SCREEN_HEIGHT / 2 - 50
    );

    // Main Menu button (Game Over)
    gameOverMainMenuButton.setFont(font);
    gameOverMainMenuButton.setString("Main Menu");
    gameOverMainMenuButton.setCharacterSize(50);
    gameOverMainMenuButton.setFillColor(sf::Color::White);
    gameOverMainMenuButton.setOutlineThickness(2);
    gameOverMainMenuButton.setOutlineColor(sf::Color::White);
    gameOverMainMenuButton.setPosition(
        SCREEN_WIDTH * 0.4,
        SCREEN_HEIGHT / 2 + 50
    );

    // Shop title (Shop)
    shopTitle.setFont(font);
    shopTitle.setString("Shop");
    shopTitle.setCharacterSize(70);
    shopTitle.setFillColor(sf::Color::White);
    shopTitle.setOutlineThickness(2);
    shopTitle.setOutlineColor(sf::Color::Yellow);
    shopTitle.setPosition(
        SCREEN_WIDTH / 9 * 4,
        SCREEN_HEIGHT / 10
    );

    // Close info text (Shop)
    closeInfoText.setFont(font);
    closeInfoText.setString("[Press 'E' to close]");
    closeInfoText.setCharacterSize(20);
    closeInfoText.setFillColor(sf::Color::White);
    closeInfoText.setPosition(
        SCREEN_WIDTH / 9 * 4,
        SCREEN_HEIGHT / 10 - 10
    );

    // Health Upgrade button (Shop)
    healthUpgradeButton.setFont(font);
    healthUpgradeButton.setString("Upgrade Health(" + std::to_string(static_cast<int>(playerHealth)) + ")");
    healthUpgradeButton.setCharacterSize(50);
    healthUpgradeButton.setFillColor(sf::Color::White);
    healthUpgradeButton.setOutlineThickness(1);
    healthUpgradeButton.setOutlineColor(sf::Color::White);
    healthUpgradeButton.setPosition(
        SCREEN_WIDTH / 5,
        SCREEN_HEIGHT / 2
    );

    // Movement Speed Upgrade button (Shop)
    movementSpeedUpgradeButton.setFont(font);
    movementSpeedUpgradeButton.setString("Upgrade Movement Speed(" + std::to_string(static_cast<int>(playerSpeed)) + ")");
    movementSpeedUpgradeButton.setCharacterSize(50);
    movementSpeedUpgradeButton.setFillColor(sf::Color::White);
    movementSpeedUpgradeButton.setOutlineThickness(1);
    movementSpeedUpgradeButton.setOutlineColor(sf::Color::White);
    movementSpeedUpgradeButton.setPosition(
        SCREEN_WIDTH / 5,
        SCREEN_HEIGHT / 2 + 50
    );

    // Firing Speed Upgrade button (Shop)
    firingSpeedUpgradeButton.setFont(font);
    firingSpeedUpgradeButton.setString("Reduce Firing Cooldown(" + std::to_string(static_cast<int>(bulletSpawnTimerMax * 1000)) + " ms)");
    firingSpeedUpgradeButton.setCharacterSize(50);
    firingSpeedUpgradeButton.setFillColor(sf::Color::White);
    firingSpeedUpgradeButton.setOutlineThickness(1);
    firingSpeedUpgradeButton.setOutlineColor(sf::Color::White);
    firingSpeedUpgradeButton.setPosition(
        SCREEN_WIDTH / 5,
        SCREEN_HEIGHT / 2 + 100
    );

    // Health Upgrade button cost (Shop)
    healthUpgradeButtonCost.setFont(font);
    healthUpgradeButtonCost.setString(std::to_string(static_cast<int>(healthUpgradeCost)));
    healthUpgradeButtonCost.setCharacterSize(50);
    healthUpgradeButtonCost.setFillColor(sf::Color::Yellow);
    healthUpgradeButtonCost.setOutlineThickness(2);
    healthUpgradeButtonCost.setOutlineColor(sf::Color::White);
    healthUpgradeButtonCost.setPosition(
        SCREEN_WIDTH / 5 * 4,
        SCREEN_HEIGHT / 2
    );

    // Movement Speed Upgrade button cost (Shop)
    movementSpeedUpgradeButtonCost.setFont(font);
    movementSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(movementSpeedUpgradeCost)));
    movementSpeedUpgradeButtonCost.setCharacterSize(50);
    movementSpeedUpgradeButtonCost.setFillColor(sf::Color::White);
    movementSpeedUpgradeButtonCost.setOutlineThickness(2);
    movementSpeedUpgradeButtonCost.setOutlineColor(sf::Color::Yellow);
    movementSpeedUpgradeButtonCost.setPosition(
        SCREEN_WIDTH / 5 * 4,
        SCREEN_HEIGHT / 2 + 50
    );

    // Firing Speed Upgrade button cost (Shop)
    firingSpeedUpgradeButtonCost.setFont(font);
    firingSpeedUpgradeButtonCost.setString(std::to_string(static_cast<int>(firingSpeedUpgradeCost)));
    firingSpeedUpgradeButtonCost.setCharacterSize(50);
    firingSpeedUpgradeButtonCost.setFillColor(sf::Color::White);
    firingSpeedUpgradeButtonCost.setOutlineThickness(2);
    firingSpeedUpgradeButtonCost.setOutlineColor(sf::Color::Yellow);
    firingSpeedUpgradeButtonCost.setPosition(
        SCREEN_WIDTH / 5 * 4,
        SCREEN_HEIGHT / 2 + 100
    );

    // Back button (Level)
    levelBackButton.setFont(font);
    levelBackButton.setString("Back");
    levelBackButton.setCharacterSize(50);
    levelBackButton.setFillColor(sf::Color::White);
    levelBackButton.setOutlineThickness(2);
    levelBackButton.setOutlineColor(sf::Color::White);
    levelBackButton.setPosition(
        SCREEN_WIDTH * 0.1,
        SCREEN_HEIGHT * 0.9
    );

    //Play easy button (Level)
    levelPlayButtonEasy.setFont(font);
    levelPlayButtonEasy.setString("Easy");
    levelPlayButtonEasy.setCharacterSize(40);
    levelPlayButtonEasy.setFillColor(sf::Color::White);
    levelPlayButtonEasy.setOutlineThickness(2);
    levelPlayButtonEasy.setOutlineColor(sf::Color::Yellow);
    levelPlayButtonEasy.setPosition(
        SCREEN_WIDTH * 0.25,
        SCREEN_HEIGHT * 0.25
    );

    //Play medium button (Level)
    levelPlayButtonMedium.setFont(font);
    levelPlayButtonMedium.setString("Medium");
    levelPlayButtonMedium.setCharacterSize(40);
    levelPlayButtonMedium.setFillColor(sf::Color::White);
    levelPlayButtonMedium.setOutlineThickness(2);
    levelPlayButtonMedium.setOutlineColor(sf::Color::Yellow);
    levelPlayButtonMedium.setPosition(
        SCREEN_WIDTH * 0.65,
        SCREEN_HEIGHT * 0.25
    );

    //Play hard button (Level)
    levelPlayButtonHard.setFont(font);
    levelPlayButtonHard.setString("Hard");
    levelPlayButtonHard.setCharacterSize(40);
    levelPlayButtonHard.setFillColor(sf::Color::White);
    levelPlayButtonHard.setOutlineThickness(2);
    levelPlayButtonHard.setOutlineColor(sf::Color::Yellow);
    levelPlayButtonHard.setPosition(
        SCREEN_WIDTH * 0.25,
        SCREEN_HEIGHT * 0.65
    );

    //Play infinite button (Level)
    levelPlayButtonInfinite.setFont(font);
    levelPlayButtonInfinite.setString("Infinite");
    levelPlayButtonInfinite.setCharacterSize(40);
    levelPlayButtonInfinite.setFillColor(sf::Color::White);
    levelPlayButtonInfinite.setOutlineThickness(2);
    levelPlayButtonInfinite.setOutlineColor(sf::Color::Yellow);
    levelPlayButtonInfinite.setPosition(
        SCREEN_WIDTH * 0.65,
        SCREEN_HEIGHT * 0.65
    );

    // Aiming (Instructions)
    controlsAiming.setFont(font);
    controlsAiming.setString("Aiming: Move Mouse");
    controlsAiming.setCharacterSize(50);
    controlsAiming.setFillColor(sf::Color::White);
    controlsAiming.setOutlineThickness(2);
    controlsAiming.setOutlineColor(sf::Color::Cyan);
    controlsAiming.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 30
    );

    // Controls (Instructions)
    controlsTitle.setFont(font);
    controlsTitle.setString("Controls:");
    controlsTitle.setCharacterSize(70);
    controlsTitle.setFillColor(sf::Color::White);
    controlsTitle.setOutlineThickness(2);
    controlsTitle.setOutlineColor(sf::Color::White);
    controlsTitle.setPosition(
        SCREEN_WIDTH / 100 * 42,
        SCREEN_HEIGHT / 100 * 5
    );

    // Health bar (Instructions)
    controlsHealthBar.setFont(font);
    controlsHealthBar.setString("Health --->");
    controlsHealthBar.setCharacterSize(25);
    controlsHealthBar.setFillColor(sf::Color::White);
    controlsHealthBar.setOutlineThickness(2);
    controlsHealthBar.setOutlineColor(sf::Color::Green);
    controlsHealthBar.setPosition(
        SCREEN_WIDTH / 100 * 85,
        SCREEN_HEIGHT / 100 * 2
    );

    // Mana ability (Instructions)
    controlsManaAbility.setFont(font);
    controlsManaAbility.setString("Mana ability: SPACE");
    controlsManaAbility.setCharacterSize(50);
    controlsManaAbility.setFillColor(sf::Color::White);
    controlsManaAbility.setOutlineThickness(2);
    controlsManaAbility.setOutlineColor(sf::Color::Cyan);
    controlsManaAbility.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 50
    );

    // Mana bar (Instructions)
    controlsManaBar.setFont(font);
    controlsManaBar.setString("Mana bar --->");
    controlsManaBar.setCharacterSize(25);
    controlsManaBar.setFillColor(sf::Color::White);
    controlsManaBar.setOutlineThickness(2);
    controlsManaBar.setOutlineColor(sf::Color::Cyan);
    controlsManaBar.setPosition(
        SCREEN_WIDTH / 100 * 82,
        SCREEN_HEIGHT / 100 * 7
    );

    // Movement (Instructions)
    controlsMovement.setFont(font);
    controlsMovement.setString("Move: WASD");
    controlsMovement.setCharacterSize(50);
    controlsMovement.setFillColor(sf::Color::White);
    controlsMovement.setOutlineThickness(2);
    controlsMovement.setOutlineColor(sf::Color::Cyan);
    controlsMovement.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 20
    );

    // Game pausing (Instructions)
    controlsPauseGame.setFont(font);
    controlsPauseGame.setString("Pause: ESC");
    controlsPauseGame.setCharacterSize(50);
    controlsPauseGame.setFillColor(sf::Color::White);
    controlsPauseGame.setOutlineThickness(2);
    controlsPauseGame.setOutlineColor(sf::Color::Cyan);
    controlsPauseGame.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 70
    );

    // Start game button (Instructions)
    controlsExitButton.setFont(font);
    controlsExitButton.setString("Back");
    controlsExitButton.setCharacterSize(50);
    controlsExitButton.setFillColor(sf::Color::White);
    controlsExitButton.setOutlineThickness(2);
    controlsExitButton.setOutlineColor(sf::Color::White);
    controlsExitButton.setPosition(
        SCREEN_WIDTH / 100 * 45,
        SCREEN_HEIGHT / 100 * 85
    );

    // Shooting (Instructions)
    controlsShooting.setFont(font);
    controlsShooting.setString("Shoot: LEFT CLICK");
    controlsShooting.setCharacterSize(50);
    controlsShooting.setFillColor(sf::Color::White);
    controlsShooting.setOutlineThickness(2);
    controlsShooting.setOutlineColor(sf::Color::Cyan);
    controlsShooting.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 40
    );

    // Shop (Instructions)
    controlsShop.setFont(font);
    controlsShop.setString("Open shop: E");
    controlsShop.setCharacterSize(50);
    controlsShop.setFillColor(sf::Color::White);
    controlsShop.setOutlineThickness(2);
    controlsShop.setOutlineColor(sf::Color::Cyan);
    controlsShop.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 60
    );

    // GM_title
    GM_title.setFont(font);
    GM_title.setString("Space Explorererers");
    GM_title.setCharacterSize(100);
    GM_title.setFillColor(sf::Color::White);
    GM_title.setOutlineThickness(2);
    GM_title.setOutlineColor(sf::Color::Cyan);
    GM_title.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 20
    );

    // GM_levelButton
    GM_levelButton.setFont(font);
    GM_levelButton.setString("Select Level");
    GM_levelButton.setCharacterSize(50);
    GM_levelButton.setFillColor(sf::Color::White);
    GM_levelButton.setOutlineThickness(2);
    GM_levelButton.setOutlineColor(sf::Color::White);
    GM_levelButton.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 40
    );

    // GM_shopButton
    GM_shopButton.setFont(font);
    GM_shopButton.setString("Shop");
    GM_shopButton.setCharacterSize(50);
    GM_shopButton.setFillColor(sf::Color::White);
    GM_shopButton.setOutlineThickness(2);
    GM_shopButton.setOutlineColor(sf::Color::White);
    GM_shopButton.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 50
    );

    // GM_specialMisionsButton
    GM_specialMisionsButton.setFont(font);
    GM_specialMisionsButton.setString("Special Misions");
    GM_specialMisionsButton.setCharacterSize(50);
    GM_specialMisionsButton.setFillColor(sf::Color::White);
    GM_specialMisionsButton.setOutlineThickness(2);
    GM_specialMisionsButton.setOutlineColor(sf::Color::White);
    GM_specialMisionsButton.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 60
    );

    // GM_backButton
    GM_backButton.setFont(font);
    GM_backButton.setString("Main Menu");
    GM_backButton.setCharacterSize(50);
    GM_backButton.setFillColor(sf::Color::White);
    GM_backButton.setOutlineThickness(2);
    GM_backButton.setOutlineColor(sf::Color::White);
    GM_backButton.setPosition(
        SCREEN_WIDTH / 100 * 35,
        SCREEN_HEIGHT / 100 * 70
    );

    // GMSHOP_BackButton
    GM_shopBackButton.setFont(font);
    GM_shopBackButton.setString("Back");
    GM_shopBackButton.setCharacterSize(50);
    GM_shopBackButton.setFillColor(sf::Color::White);
    GM_shopBackButton.setOutlineThickness(2);
    GM_shopBackButton.setOutlineColor(sf::Color::White);
    GM_shopBackButton.setPosition(
        SCREEN_WIDTH / 100 * 95,
        0
    );

    // GMSHOP_MoneyCounter
    GM_shopMoneyCounter.setFont(font);
    GM_shopMoneyCounter.setString(std::to_string(static_cast<int>(globalMoney)) + " $");
    GM_shopMoneyCounter.setCharacterSize(50);
    GM_shopMoneyCounter.setFillColor(sf::Color::White);
    GM_shopMoneyCounter.setOutlineThickness(2);
    GM_shopMoneyCounter.setOutlineColor(sf::Color::Yellow);
    GM_shopMoneyCounter.setPosition(
        0,
        0
    );
}


// -------------------------------------------- Initializing Textures --------------------------------------------

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
    if (!enemyTexture.loadFromFile("orangeEnemy.png"))
    {
        std::cerr << "Failed to load enemy texture!" << std::endl;
    }
    if (!damageOutlineTexture.loadFromFile("damageOutlineTexture.png"))
    {
        std::cerr << "Failed to load damage texture!" << std::endl;
    }

    // Tell Enemy to use this texture
    Enemy::setTexture(enemyTexture);
}
// -------------------------------------------- Initializing Rectangles with textures --------------------------------------------

void Game::initializeRectangles()
{
    background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    background.setTexture(&backgroundTexture);

    exitPanelOutline.setSize(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4));
    exitPanelOutline.setPosition(sf::Vector2f(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8 * 3));
    exitPanelOutline.setOutlineColor(sf::Color::Green);
    exitPanelOutline.setOutlineThickness(5);
    exitPanelOutline.setFillColor(sf::Color::Black);

    player.setPosition(SCREEN_WIDTH / 2 - player.getSize().x / 2, SCREEN_HEIGHT / 2 - player.getSize().y / 2);
    player.setTexture(&playerTexture);

    shockwave.setRadius(player.getSize().x * 2);
    shockwave.setPosition(player.getPosition().x - 1.5 * player.getSize().x, player.getPosition().y - 1.5 * player.getSize().y);
    shockwave.setTexture(&shockwaveTexture);

    shockwaveScreenOutline.setPosition(0, 0);
    shockwaveScreenOutline.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    shockwaveScreenOutline.setTexture(&shockwaveScreenOutlineTexture);

    damageOutline.setPosition(0, 0);
    damageOutline.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    damageOutline.setTexture(&damageOutlineTexture);

    gameOverOutline.setPosition(0, 0);
    gameOverOutline.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

    healthBarBorder.setOutlineColor(sf::Color::White);
    healthBarBorder.setOutlineThickness(2);
    healthBarBorder.setFillColor(sf::Color::Transparent);
    healthBarBorder.setPosition(sf::Vector2f(SCREEN_WIDTH - healthBarBorder.getSize().x - 10, 20));

    manaBarBorder.setOutlineColor(sf::Color::White);
    manaBarBorder.setOutlineThickness(2);
    manaBarBorder.setFillColor(sf::Color::Transparent);
    manaBarBorder.setPosition(sf::Vector2f(healthBarBorder.getPosition().x, healthBarBorder.getPosition().y + 40));

    healthBar.setPosition(sf::Vector2f(healthBarBorder.getPosition()));
    healthBar.setFillColor(sf::Color::Green);

    manaBar.setPosition(sf::Vector2f(healthBarBorder.getPosition().x, healthBarBorder.getPosition().y + 40));
    manaBar.setFillColor(sf::Color::Cyan);


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
    

    // GM_shop objects
    setDefaultGMShopObjectPositions();

    GM_shopPlayerModel.setTexture(&playerTexture);
    GM_shopPlayerModel.setSize(sf::Vector2f(playerSize, playerSize));
    GM_shopMovableList.push_back(&GM_shopPlayerModel);

	GM_shopUpgradeButton1.setTexture(&levelTabletTexture);
    GM_shopUpgradeButton1.setSize(GM_shopUpgradeButtonSize);
    GM_shopMovableList.push_back(&GM_shopUpgradeButton1);

    GM_shopUpgradeButton2.setTexture(&levelTabletTexture);
    GM_shopUpgradeButton2.setSize(GM_shopUpgradeButtonSize);
    GM_shopMovableList.push_back(&GM_shopUpgradeButton2);

    GM_shopUpgradeButton3.setTexture(&levelTabletTexture);
    GM_shopUpgradeButton3.setSize(GM_shopUpgradeButtonSize);
    GM_shopMovableList.push_back(&GM_shopUpgradeButton3);

    GM_shopUpgradeButton4.setTexture(&levelTabletTexture);
    GM_shopUpgradeButton4.setSize(GM_shopUpgradeButtonSize);
    GM_shopMovableList.push_back(&GM_shopUpgradeButton4);

    GM_shopUpgradeButton5.setTexture(&levelTabletTexture);
    GM_shopUpgradeButton5.setSize(GM_shopUpgradeButtonSize);
    GM_shopMovableList.push_back(&GM_shopUpgradeButton5);

    GM_shopUpgradeButton6.setTexture(&levelTabletTexture);
    GM_shopUpgradeButton6.setSize(GM_shopUpgradeButtonSize);
    GM_shopMovableList.push_back(&GM_shopUpgradeButton6);
}

// -------------------------------------------- Reset GM shop object positions --------------------------------------------

void Game::setDefaultGMShopObjectPositions()
{
    GM_shopPlayerModel.setPosition(
        defaultPlayerModelPosition
    );
    GM_shopUpgradeButton1.setPosition(
        defaultPlayerModelPosition.x - SCREEN_WIDTH / 4 - GM_shopUpgradeButton1.getSize().x / 2 + playerSize / 2, 
        defaultPlayerModelPosition.y - SCREEN_HEIGHT / 4
    );
    GM_shopUpgradeButton2.setPosition(
        defaultPlayerModelPosition.x - GM_shopUpgradeButton1.getSize().x / 2 + playerSize / 2, 
        defaultPlayerModelPosition.y - SCREEN_HEIGHT / 4
    );
    GM_shopUpgradeButton3.setPosition(
        defaultPlayerModelPosition.x + SCREEN_WIDTH / 4 - GM_shopUpgradeButton1.getSize().x / 2 + playerSize / 2, 
        defaultPlayerModelPosition.y - SCREEN_HEIGHT / 4
    );
    GM_shopUpgradeButton4.setPosition(
        defaultPlayerModelPosition.x - SCREEN_WIDTH / 4 - GM_shopUpgradeButton1.getSize().x / 2 + playerSize / 2,
        defaultPlayerModelPosition.y - SCREEN_HEIGHT / 4 * 3
    );
    GM_shopUpgradeButton5.setPosition(
        defaultPlayerModelPosition.x - GM_shopUpgradeButton1.getSize().x / 2 + playerSize / 2,
        defaultPlayerModelPosition.y - SCREEN_HEIGHT / 4 * 3
    );
    GM_shopUpgradeButton6.setPosition(
        defaultPlayerModelPosition.x + SCREEN_WIDTH / 4 - GM_shopUpgradeButton1.getSize().x / 2 + playerSize / 2,
        defaultPlayerModelPosition.y - SCREEN_HEIGHT / 4 * 3
    );
}


// -------------------------------------------- Updating and Drawing health bar --------------------------------------------

void Game::updateHealthBar()
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
}

void Game::drawHealthBar(sf::RenderWindow& window)
{

	window.draw(healthBar);
	window.draw(healthBarBorder);
	window.draw(healthText);
}


// -------------------------------------------- Undating and Drawing Mana Bar --------------------------------------------

void Game::updateManaBar()
{
    manaBar.setSize(sf::Vector2f(manaBarBorder.getSize().x * (playerMana / maxPlayerMana), manaBar.getSize().y));
    manaBar.setFillColor((playerMana >= maxPlayerMana) ? sf::Color::Magenta : sf::Color::Cyan);
    manaText.setString(std::to_string(static_cast<int>(playerMana)));
}

void Game::drawManaBar(sf::RenderWindow& window)
{
	window.draw(manaBar);
	window.draw(manaBarBorder); 
	window.draw(manaText);
}


// -------------------------------------------- Undating and Drawing Money Text --------------------------------------------

void Game::updateMoneyText()
{
    moneyText.setString(std::to_string(gameMoney) + " $");
}

void Game::drawMoneyText(sf::RenderWindow& window)
{
	window.draw(moneyText);
}



// -------------------------------------------- Button Highlighting Method --------------------------------------------

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


// -------------------------------------------- Applying border damage --------------------------------------------

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

// -------------------------------------------- Executing mana ability --------------------------------------------

void Game::activateManaAbility()
{
	if (playerMana >= maxPlayerMana)
	{
		playerMana = 0.f;
		Enemy::manaAbilityDamage(player.getPosition());
		shockwaveRenderTime = gameTime + .5f;
	}
}
// -------------------------------------------- Resetting Game settings --------------------------------------------

void Game::resetGameSettings()
{
    gameTime = .0f;
    borderDamage = 1.0f;
    playerHealth = defaultPlayerHealth;
    playerSpeed = defaultPlayerSpeed;
    gameMoney = 0;
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
    bossAbilityTimer = 0.f;
    bossAbilityTimerMax = 3.f;
    enemySpawnTimer = 0.f;
    shockwaveRenderTime = 0.f;
    damageOutlineRenderTime = 0.f;
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

// -------------------------------------------- Updating the game (every frame in Game state) --------------------------------------------

void Game::update(float deltaTime) 
{
    Enemy::update(deltaTime, player.getPosition());

    if (Enemy::checkPlayerTouch(player, playerHealth) > 0 || applyBorderDamage() > 0)
    {
        damageOutlineRenderTime = gameTime + 0.1f;
    }

    playerHealth -= Enemy::checkPlayerTouch(player, playerHealth);
	playerHealth -= applyBorderDamage();
    updateHealthBar();
    updateManaBar();
    updateMoneyText();
    enemySpawnTimerMax = 4.f - 3 * (gameTime / 60.f);

    if (gameTime < winningSurvivalTime)
    {
        enemySpawnTimer += Enemy::trySpawn(enemySpawnTimer, enemySpawnTimerMax, deltaTime, enemiesPerWave, difficulty, gameTime);
    }

    if (bossSpawnTimer >= bossSpawnTimerMax)
    {
        new Enemy(50, 100, sf::Vector2f(-SCREEN_WIDTH / 10, SCREEN_HEIGHT / 2), 1000, true);
        bossSpawnTimer = 0.f;
    }
    else
    {
        bossSpawnTimer += deltaTime;
    }

    Enemy::tryActivateBossAbility(deltaTime, &bossAbilityTimer, &bossAbilityTimerMax);

    if (gameTime >= winningSurvivalTime && Enemy::enemyList.size() == 0 || sf::Keyboard::isKeyPressed(sf::Keyboard::F))
    {
        gameOverTitle.setOutlineColor(sf::Color::Green);
        gameOverTitle.setString("You win!");
        gameOverTitle.setPosition(SCREEN_WIDTH * 0.38, SCREEN_HEIGHT / 2 - 180);
        gameOverOutline.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        gameOverOutline.setTexture(&shockwaveScreenOutlineTexture);
        state = GameState::GAME_OVER;
    }

    Bullet::update(deltaTime);
    Bullet::checkRemove(window);
    Bullet::checkCollisions(window);
    gameMoney += Enemy::hitRemove();

    // SALABOT MANAS SKAITĪŠANU (NESTRĀDĀ AR PIERCING BULLETS)
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
        gameOverOutline.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        gameOverOutline.setTexture(&damageOutlineTexture);
        state = GameState::GAME_OVER;
    }

	gameTime += deltaTime;
}

// -------------------------------------------- Rendering GameStates --------------------------------------------

// -------------------------------------------- MENU --------------------------------------------

void Game::renderMenu() 
{
    window.clear();

    menuContinueButton.setFillColor(continueAvailable ? sf::Color::White : sf::Color::Black);

	window.draw(background);
	window.draw(title);
    window.draw(menuContinueButton);
    window.draw(playButton);
    window.draw(controlsButton);
    window.draw(settingsButton);
    window.draw(exitButton);

    if (exitPanelOpen)
    {
        window.draw(exitPanelOutline);
        window.draw(exitPanelYesButton);
        window.draw(exitPanelNoButton);
        window.draw(exitPanelTitle);
    }

    window.display();
}
// -------------------------------------------- SETTINGS --------------------------------------------

void Game::renderSettings() 
{
    window.clear();
    window.draw(background);
    window.draw(backButton);
    window.display();
}
// -------------------------------------------- GAME --------------------------------------------

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
    if (gameTime <= damageOutlineRenderTime)
    {
        window.draw(damageOutline);
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
// -------------------------------------------- PAUSE --------------------------------------------

void Game::renderPause() 
{
    window.clear();
    window.draw(background);
    window.draw(pauseTitle);
    window.draw(continueButton);
    window.draw(mainMenuButton);
    window.display();
}
// -------------------------------------------- INSTRUCTIONS --------------------------------------------

void Game::renderControls()
{
    window.clear();
    window.draw(background);
    window.draw(healthBarBorder);
    window.draw(healthBar);
    window.draw(manaBarBorder);
    window.draw(manaBar);
    window.draw(controlsAiming);
    window.draw(controlsTitle);
    window.draw(controlsHealthBar);
    window.draw(controlsManaAbility);
    window.draw(controlsManaBar);
    window.draw(controlsMovement);
    window.draw(controlsPauseGame);
    window.draw(controlsExitButton);
    window.draw(controlsShooting);
    window.draw(controlsShop);
    window.display();
}
// -------------------------------------------- GAME OVER --------------------------------------------

void Game::renderGameOver() 
{
    window.clear();
    window.draw(background);
    window.draw(gameOverOutline);
    window.draw(gameOverTitle);
    window.draw(restartButton);
    window.draw(gameOverMainMenuButton);
    window.display();
}
// -------------------------------------------- SHOP --------------------------------------------

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
// -------------------------------------------- Game Menu (GM) LEVEL --------------------------------------------

void Game::renderLevel()
{
	window.clear();
	window.draw(background);
	window.draw(levelTablet);
	window.draw(levelBackButton);
    window.draw(levelPlayButtonEasy);
    window.draw(levelPlayButtonMedium);
    window.draw(levelPlayButtonHard);
    window.draw(levelPlayButtonInfinite);
    Ad::drawAll(window);
	window.display();
}

// -------------------------------------------- Game Menu (GM) Game Menu --------------------------------------------

void Game::renderGMGameMenu()
{
    window.clear();

    window.draw(background);
    window.draw(GM_levelButton);
    window.draw(GM_shopButton);
    window.draw(GM_title);
    window.draw(GM_specialMisionsButton);
    window.draw(GM_backButton);

    window.display();
}

// -------------------------------------------- Game Menu (GM) SHOP --------------------------------------------

void Game::renderGMShop()
{
    window.clear();

    window.draw(background);
    window.draw(GM_shopMoneyCounter);
    window.draw(GM_shopBackButton);
    window.draw(GM_shopPlayerModel);
    window.draw(GM_shopUpgradeButton1);
    window.draw(GM_shopUpgradeButton2);
    window.draw(GM_shopUpgradeButton3);
    window.draw(GM_shopUpgradeButton4);
    window.draw(GM_shopUpgradeButton5);
    window.draw(GM_shopUpgradeButton6);

    window.display();
}


