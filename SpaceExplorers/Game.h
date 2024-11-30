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
    static constexpr int SCREEN_WIDTH = 1280;
    static constexpr int SCREEN_HEIGHT = 720;
    static constexpr float maxPlayerHealth = 100.f;

    int playerMoney = 0;

private:
    enum class GameState { MENU, PLAY, PAUSE, SETTINGS, GAME_OVER };


    sf::RenderWindow window;
    sf::Font font;

    // Menu Buttons
    sf::Text playButton;
    sf::Text settingsButton;
    sf::Text exitButton;

    // Settings Menu
    sf::Text backButton;

    // Pause Menu
    sf::Text pauseTitle;
    sf::Text continueButton;
    sf::Text mainMenuButton;

    // Game Over Menu
    sf::Text gameOverTitle;
    sf::Text restartButton;
    sf::Text gameOverMainMenuButton;

    sf::CircleShape planet;
    sf::RectangleShape player;
	sf::RectangleShape healthBarBorder;
	sf::RectangleShape manaBarBorder;
	sf::RectangleShape healthBar;
	sf::RectangleShape manaBar;
	sf::Text moneyText;
    sf::Texture playerTexture;

    GameState state;

    float playerHealth;
	float borderDamage;

    float enemySpawnTimer;
    float enemySpawnTimerMax;
    float bulletSpawnTimer;
    float bulletSpawnTimerMax;
	float gameTime;

    sf::Clock clock;

    void handleMenuInput();
    void handleSettingsInput();
    void handlePauseInput();
    void handleGameInput(float deltaTime);
    void handleGameOverInput();

    void drawHealthBar(sf::RenderWindow& window);
    void drawManaBar(sf::RenderWindow& window);
	void drawMoneyText(sf::RenderWindow& window);
    float applyBorderDamage();
    void update(float deltaTime);
    void renderMenu();
    void renderSettings();
    void renderPause();
    void renderGame();
    void renderGameOver();
};

#endif // GAME_H

