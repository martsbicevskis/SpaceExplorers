#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Bullet.h"

class Game {
public:
    Game();
    void run();

public:
    //Constant game settings
    static constexpr int SCREEN_WIDTH = 1280;
    static constexpr int SCREEN_HEIGHT = 720;
    static constexpr float defaultPlayerHealth = 100.f;
	static constexpr float defaultPlayerSpeed = 100.f;
	static constexpr float defaultPlayerManaMax = 100.f;
    static constexpr int playerSize = 50;
	static constexpr float defaultBulletSize = 10.f;

	//global variables
    int playerMoney = 0;
    float gameTime = 0;

    //game states
private:
    enum class GameState { MENU, PLAY, PAUSE, SETTINGS, GAME_OVER, SHOP };


    sf::RenderWindow window;
    sf::Font font;

    // Main Menu
    sf::Text playButton;
    sf::Text settingsButton;
    sf::Text exitButton;
	sf::Text title;

    // Settings
    sf::Text backButton;

    // Pause
    sf::Text pauseTitle;
    sf::Text continueButton;
    sf::Text mainMenuButton;

    // Game Over
    sf::Text gameOverTitle;
    sf::Text restartButton;
    sf::Text gameOverMainMenuButton;

    // Shop
    sf::Text healthUpgradeButtonCost;
	sf::Text movementSpeedUpgradeButtonCost;
	sf::Text firingSpeedUpgradeButtonCost;
	sf::Text shopTitle;
	sf::Text healthUpgradeButton;
	sf::Text movementSpeedUpgradeButton;
	sf::Text firingSpeedUpgradeButton;
    sf::Text closeInfoText;

    //Game elements
	sf::RectangleShape background;
    sf::RectangleShape player;
	sf::RectangleShape healthBarBorder;
	sf::RectangleShape manaBarBorder;
	sf::RectangleShape healthBar;
	sf::RectangleShape manaBar;
    sf::CircleShape planet;
	sf::Text moneyText;
    sf::Texture playerTexture;
	sf::Texture backgroundTexture;

    GameState state;

    //shop costs
    int healthUpgradeCost;
	int movementSpeedUpgradeCost;
	int firingSpeedUpgradeCost;

    //player stats 
	float playerMana;
	float maxPlayerMana;
    float playerHealth;
    float playerSpeed;
	float borderDamage;

	//timers
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    float bulletSpawnTimer;
    float bulletSpawnTimerMax;

    sf::Clock clock;

	//state updating methods
    void handleMenuInput();
    void handleSettingsInput();
    void handlePauseInput();
    void handleGameInput(float deltaTime);
    void handleGameOverInput();
	void handleShopInput();

	//drawing and updating methods
    void drawHealthBar(sf::RenderWindow& window);
    void drawManaBar(sf::RenderWindow& window);
	void drawMoneyText(sf::RenderWindow& window);
    float applyBorderDamage();
	void activateManaAbility();
    void update(float deltaTime);

	//rendering methods
    void renderMenu();
    void renderSettings();
    void renderPause();
    void renderGame();
    void renderGameOver();
	void renderShop();
};

#endif

