#pragma once
#include <SFML/Window.hpp>

class Input
{
public:
	static bool KeyPressed(sf::Keyboard::Key key)
	{
		return sf::Keyboard::isKeyPressed(key);
	}

	static bool ButtonPressed(sf::Mouse::Button button)
	{
		return sf::Mouse::isButtonPressed(button);
	}
};
