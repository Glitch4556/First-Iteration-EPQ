#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../ECS/ECS.hpp"
#include "../ECS/input.hpp"

class Movement : public Component
{
public:
	sf::Vector2f velocity;
	sf::Vector2f position;

	void onUpdate() override
	{
		position = position + velocity;
	}
};

class Neutron : public Behaviour
{
private:
	sf::CircleShape circle;
	Movement* component;

public:
	Neutron(sf::Vector2f pos)
	{
		circle = sf::CircleShape(5.f);
		circle.setFillColor(sf::Color(81, 87, 83));
		circle.setOrigin({ circle.getRadius(), circle.getRadius() });
		circle.setPosition(pos);
		component = AddComponent<Movement>();
		component->velocity = {6,7};
		component->position = pos;
	}

	void onUpdate(sf::RenderWindow& window) override
	{
		circle.setPosition(component->position);
	}

	void onRender(sf::RenderWindow& window) override
	{
		window.draw(circle);
	}
};

class Nucleus : public Behaviour
{
private:
	sf::CircleShape circle;
public:
	Nucleus(sf::Vector2f pos)
	{
		circle = sf::CircleShape(5.f);
		circle.setFillColor(sf::Color(0, 0, 255));
		circle.setOrigin({ circle.getRadius(), circle.getRadius() });
		circle.setPosition(pos);
	}

	void onUpdate(sf::RenderWindow& window) override
	{

	}

	void onRender(sf::RenderWindow& window) override
	{
		window.draw(circle);
	}
};

sf::Vector2f mousePos(sf::RenderWindow& window)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	return sf::Vector2f(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));
}

class ParticleSpawner : public Behaviour
{
private:

public:

	void onRender(sf::RenderWindow& window) override
	{

	}
	void onUpdate(sf::RenderWindow& window) override
	{
		if (Input::ButtonPressed(sf::Mouse::Button::Left))

		{
			std::unique_ptr<Neutron> neutron = std::make_unique<Neutron>(mousePos(window));
			Behaviour::CreateObject(std::move(neutron));
		}
		if (Input::ButtonPressed(sf::Mouse::Button::Right))
		{
			std::unique_ptr<Nucleus> nucleus = std::make_unique<Nucleus>(mousePos(window));
			Behaviour::CreateObject(std::move(nucleus));
		}
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML Window", sf::State::Windowed);

	std::unique_ptr<ParticleSpawner> spawner = std::make_unique<ParticleSpawner>();
	Behaviour::CreateObject(std::move(spawner));

	while (window.isOpen())
	{
		Behaviour::RenderAll(window);
		Behaviour::UpdateAll(window);
	}
}
