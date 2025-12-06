#include "Button.h"
#include <algorithm>

std::vector<Button*> Button::buttonList;

Button::Button(
    const sf::Font& font,
    const std::string& text,
    float charSize,
    sf::Color fillColor,
    sf::Color outlineColor,
    float outlineThickness,
    sf::Vector2f position)
    : fontPtr(&font)
{
    // Configure text
    label.setFont(*fontPtr);
    label.setString(text);
    label.setCharacterSize(static_cast<unsigned int>(charSize));
    label.setFillColor(sf::Color::White);
    label.setOutlineColor(outlineColor);
    label.setOutlineThickness(outlineThickness);

    // Compute background size with padding
    const float paddingX = 16.f;
    const float paddingY = 8.f;
    sf::FloatRect tb = label.getLocalBounds();
    background.setSize(sf::Vector2f(tb.width + paddingX * 2.f, tb.height + paddingY * 2.f));
    background.setFillColor(fillColor);
    background.setOutlineColor(outlineColor);
    background.setOutlineThickness(outlineThickness);

    // Position background and label, accounting for label local bounds
    background.setPosition(position);
    label.setPosition(position.x + paddingX - tb.left, position.y + paddingY - tb.top);

    // Register
    buttonList.push_back(this);
}

Button::~Button()
{
    // Remove from registry to avoid dangling pointers
    auto it = std::find(buttonList.begin(), buttonList.end(), this);
    if (it != buttonList.end())
    {
        buttonList.erase(it);
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(background, states);
    target.draw(label, states);
}

void Button::drawButtons(sf::RenderWindow& window)
{
    for (auto b : buttonList)
    {
        if (b) window.draw(*b);
    }
}

bool Button::contains(sf::Vector2f point) const
{
    return background.getGlobalBounds().contains(point);
}

