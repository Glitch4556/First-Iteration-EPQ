#include "input.hpp"

std::array<bool, sf::Mouse::ButtonCount> Input::previousMouseState = {};
std::array<bool, sf::Keyboard::KeyCount> Input::previousKeyState = {};

bool Input::MousePressedOnce(sf::Mouse::Button button)
{
    std::size_t index = static_cast<std::size_t>(button);

    bool currentlyPressed = sf::Mouse::isButtonPressed(button);
    bool pressedOnce = (currentlyPressed && !previousMouseState[index]);

    previousMouseState[index] = currentlyPressed;

    return pressedOnce;
}

bool Input::KeyPressedOnce(sf::Keyboard::Key key)
{
    std::size_t index = static_cast<std::size_t>(key);

    bool currentlyPressed = sf::Keyboard::isKeyPressed(key);
    bool pressedOnce = (currentlyPressed && !previousKeyState[index]);

    previousKeyState[index] = currentlyPressed;

    return pressedOnce;
}
