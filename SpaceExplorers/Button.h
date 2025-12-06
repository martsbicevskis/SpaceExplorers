#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

class Button : public sf::Drawable, public sf::Transformable
{
public:
    // Note: font is not copied; pass a reference to a font owned by Game (e.g. Game::font)
    Button(
        const sf::Font& font,
        const std::string& text,
        float charSize,
        sf::Color fillColor,
        sf::Color outlineColor,
        float outlineThickness,
        sf::Vector2f position
    );
    ~Button();

    // Non-copyable to keep registry correctness
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;

    static void drawButtons(sf::RenderWindow& window);

    bool contains(sf::Vector2f point) const;
    void setCallback(std::function<void()> cb) { callback = std::move(cb); }

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    const sf::Font* fontPtr;
    sf::Text label;
    sf::RectangleShape background;
    std::function<void()> callback;

public:
    static std::vector<Button*> buttonList;
};