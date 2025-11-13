#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../ECS/ECS.hpp"
#include "../ECS/input.hpp"
// Creation of movement class, with base class of component
class Movement : public Component
{
public:
	// Public empty variables of velocity and position initialised
	sf::Vector2f velocity;
	sf::Vector2f position;
	// Overrides onUpdate function inherited from Component
	void onUpdate() override
	{
		// Updates position vector by adding velocity vector
		position = position + velocity;
	}
};

// Creation of Neutron class, with base class of Behaviour
class Neutron : public Behaviour
{
private:
	// Creates an empty "CircleShape" called circle
	// Creates "component", an empty ptr which can point to a movement object
	sf::CircleShape circle;
	Movement* component;

public:
	// Function which takes in a vector of pos
	Neutron(sf::Vector2f pos)
	{
		// Fills in the empty information from variable "circle"
		circle = sf::CircleShape(5.f);
		circle.setFillColor(sf::Color(81, 87, 83));
		circle.setOrigin({ circle.getRadius(), circle.getRadius() });
		circle.setPosition(pos);
		// fill this in later
		component = AddComponent<Movement>();
		component->velocity = {6,7};
		component->position = pos;
	}
	// Overrides for the onUpdate and onRender functions inherited from behaviour class
	void onUpdate(sf::RenderWindow& window) override
	{
		circle.setPosition(component->position);
	}

	void onRender(sf::RenderWindow& window) override
	{
		window.draw(circle);
	}
};
// Same as Neutron, except without movement
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
// Function to find mouse position in comaprison to window
sf::Vector2f mousePos(sf::RenderWindow& window)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	return sf::Vector2f(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));
}
// Controls when Neutrons and Nuclei are created
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
	// Creates "spawner", which is a unqiue ptr to the particle spawner class
	std::unique_ptr<ParticleSpawner> spawner = std::make_unique<ParticleSpawner>();
	// Calls function CreateObject, and passes the information inside the spawner ptr to the function
	Behaviour::CreateObject(std::move(spawner));

	while (window.isOpen())
	{
		Behaviour::RenderAll(window);
		Behaviour::UpdateAll(window);
	}
}
