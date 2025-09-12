#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Enemy.h"
#include "Bullet.h"
#include "Ad.h"
#include "Particles.h"

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
    static constexpr float openingCooldownMax = .2f;


	//global variables
    int playerMoney = 0;
    float gameTime = 0;

private:
    //game states
    enum class GameState { MENU, PLAY, PAUSE, SETTINGS, GAME_OVER, SHOP, LEVEL };
    enum class ShotMode { RAPID, SHOTGUN, BOMB };

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
	sf::RectangleShape shockwaveScreenOutline;
    sf::RectangleShape player;
	sf::RectangleShape healthBarBorder;
	sf::RectangleShape manaBarBorder;
	sf::RectangleShape healthBar;
	sf::RectangleShape manaBar;
    sf::CircleShape planet;
	sf::CircleShape shockwave;
	sf::Text healthText;
	sf::Text manaText;
	sf::Text moneyText;
	sf::Texture levelTabletTexture;
	sf::Texture shockwaveTexture;
    sf::Texture playerTexture;
	sf::Texture backgroundTexture;
    sf::Texture shockwaveScreenOutlineTexture;

	//Level elements
    sf::RectangleShape levelTablet;
    sf::Text levelPlayButton;
	sf::Text levelBackButton;
    sf::Text levelPlayButtonEasy;
    sf::Text levelPlayButtonMedium;
    sf::Text levelPlayButtonHard;
    sf::Text levelPlayButtonInfinite;



    GameState state;
    ShotMode shotMode;

    //shop costs
    int healthUpgradeCost;
	int movementSpeedUpgradeCost;
	int firingSpeedUpgradeCost;

    //player stats 
	float playerMana;
	float maxPlayerMana;
    float playerHealth;
    float playerSpeed;

    //game stats
	float borderDamage;
	float shockwaveRenderTime;
	float shopOpeningCooldown;
    int enemiesPerWave;

	//timers
    float bossSpawnTimer;
    float bossSpawnTimerMax;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    float bulletSpawnTimer;
    float bulletSpawnTimerMax;

    sf::Clock clock;

	//state updating methods
    void handleMenuInput();
    void handleSettingsInput();
    void handlePauseInput(float deltaTime);
    void handleGameInput(float deltaTime);
    void handleGameOverInput();
	void handleShopInput(float deltaTime);
	void handleLevelInput();

	//drawing and updating methods
	void initializeButtons();
	void initializeTextures();
	void initializeRectangles();
    void drawHealthBar(sf::RenderWindow& window);
    void drawManaBar(sf::RenderWindow& window);
	void drawMoneyText(sf::RenderWindow& window);
	void hightlightHower(sf::Text& button, sf::Vector2f mousePos);
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
	void renderLevel();
};

#endif

