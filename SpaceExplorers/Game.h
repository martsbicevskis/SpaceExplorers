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
    int gameMoney = 0;
    int globalMoney = 0;
    float gameTime = 0;

private:
    //game states
    enum class GameState { MENU, PLAY, INSTRUCTIONS, PAUSE, SETTINGS, GAME_OVER, SHOP,
        GM_GAMEMENU, GM_LEVEL, GM_SHOP };

    enum class ShotMode { RAPID, SHOTGUN, BOMB, PIERCING };

    sf::RenderWindow window;
    sf::Font font;

    // Game Menu (GM)
    sf::Text GM_levelButton;
    sf::Text GM_shopButton;
    sf::Text GM_title;
    sf::Text GM_specialMisionsButton;
    sf::Text GM_backButton;

    // Game Menu (GM) Shop
    // Variables
    sf::Vector2f defaultPlayerModelPosition;
    sf::Vector2f GM_shopMousePos;
    sf::Vector2f GM_shopUpgradeButtonSize;
    bool GM_shopMouseHeld;

    // Buttons
    sf::Text GM_shopBackButton;
    sf::Text GM_shopMoneyCounter;

    sf::RectangleShape GM_shopPlayerModel;
    sf::RectangleShape GM_shopUpgradeButton1;
    sf::RectangleShape GM_shopUpgradeButton2;
    sf::RectangleShape GM_shopUpgradeButton3;

    // Game Menu (GM) Level
    sf::RectangleShape levelTablet;
    sf::Text levelBackButton;
    sf::Text levelPlayButtonEasy;
    sf::Text levelPlayButtonMedium;
    sf::Text levelPlayButtonHard;
    sf::Text levelPlayButtonInfinite;

    // Main Menu
    sf::Text playButton;
    sf::Text menuContinueButton;
    sf::Text controlsButton;
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

    // Instructions
    sf::Text instrAiming;
    sf::Text instrControls;
    sf::Text instrHealthBar;
    sf::Text instrManaAbility;
    sf::Text instrManaBar;
    sf::Text instrMovement;
    sf::Text instrPauseGame;
    sf::Text instrExitButton;
    sf::Text instrShooting;
    sf::Text instrShop;

    //Game elements
	sf::RectangleShape background;
	sf::RectangleShape shockwaveScreenOutline;
    sf::RectangleShape damageOutline;
    sf::RectangleShape gameOverOutline;
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
    sf::Texture enemyTexture;
    sf::Texture damageOutlineTexture;



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
	float damageOutlineRenderTime;
	float shopOpeningCooldown;
    int enemiesPerWave;
    float difficulty;
    bool continueAvailable;

	//timers
    float bossSpawnTimer;
    float bossSpawnTimerMax;
    float bossAbilityTimer;
    float bossAbilityTimerMax;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    float bulletSpawnTimer;
    float bulletSpawnTimerMax;
    float winningSurvivalTime;

    sf::Clock clock;

	//state updating methods
    void handleMenuInput();
    void handleSettingsInput();
    void handlePauseInput(float deltaTime);
    void handleInstructionsInput();
    void handleGameInput(float deltaTime);
    void handleGameOverInput();
	void handleShopInput(float deltaTime);
	void handleLevelInput();
    void handleGMGameMenuInput();
    void handleGMShopInput();

	//drawing and updating methods
	void initializeButtons();
	void initializeTextures();
	void initializeRectangles();
    void setDefaultGMShopObjectPositions();
    void drawHealthBar(sf::RenderWindow& window);
    void updateHealthBar();
    void drawManaBar(sf::RenderWindow& window);
    void updateManaBar();
	void drawMoneyText(sf::RenderWindow& window);
    void updateMoneyText();
	void hightlightHower(sf::Text& button, sf::Vector2f mousePos);
    float applyBorderDamage();
	void activateManaAbility();
    void resetGameSettings();
    void update(float deltaTime);

	//rendering methods
    void renderMenu();
    void renderSettings();
    void renderPause();
    void renderInstructions();
    void renderGame();
    void renderGameOver();
	void renderShop();
	void renderLevel();
    void renderGMGameMenu();
    void renderGMShop();

};

#endif

