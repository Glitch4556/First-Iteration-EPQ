#pragma once
#include <SFML/Window.hpp>
#include <array>

class Input
{
public:
    static bool MousePressedOnce(sf::Mouse::Button button);
    static bool KeyPressedOnce(sf::Keyboard::Key key);
    
    static bool MouseHeld(sf::Mouse::Button button)
    {
        if (sf::Mouse::isButtonPressed(button))
        {
            return true;
        }
    }

private:
    static std::array<bool, sf::Mouse::ButtonCount> previousMouseState;
    static std::array<bool, sf::Keyboard::KeyCount> previousKeyState;
};