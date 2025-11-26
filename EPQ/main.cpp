#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../ECS/ECS.hpp"
#include "../ECS/input.hpp"
#include  <random>

float randFloat(int range1, int range2)
{
	std::random_device rd;   // seeding
	std::mt19937 gen(rd());  // Mersenne Twister engine
	std::uniform_int_distribution<> dist(range1,range2);
	float randFloat = dist(gen);
	return randFloat;
}

class Movement : public Component
{
public:
	// Public empty variables of velocity and position initialised
	sf::Vector2f velocity;
	sf::Vector2f position;
	
	void onUpdate(EngineContext& context) override
	{
		// Updates pos by adding velocity to current pos
		position = position + (velocity * context.deltaTime);
		// Gets window size and converts it to Vector2f
		sf::Vector2u size = context.window.getSize();
		sf::Vector2f sizeF(static_cast<float>(size.x),static_cast<float>(size.y));
		// Manages collision with border of window
		if (position.x >= sizeF.x or position.x <= 0)
		{
			velocity = {-velocity.x,velocity.y};
			position = position + (velocity * context.deltaTime);
		}
		else if (position.y >= sizeF.y or position.y <= 0)
		{
			velocity = {velocity.x,-velocity.y};
			position = position + (velocity * context.deltaTime);
		}
	}
};

class CircleCollision : public Component
{
private:
	static std::vector<CircleCollision*> objects;

public:
	sf::CircleShape* collider;
	Movement* movement;

	void onCreation() override
	{
		objects.push_back(this);
	}

	static void checkCollision(EngineContext& context) 
	{
		for (int i = 0; i < objects.size(); i++)
		{
			CircleCollision* currentObject = objects[i];

			for (int k = i; k < objects.size(); k++)
			{
				CircleCollision* comparisonObject = objects[k];
				float sumOfRadii = currentObject->collider->getRadius() + comparisonObject->collider->getRadius();
				sf::Vector2f centre1 = currentObject->collider->getPosition();
				sf::Vector2f centre2 = comparisonObject->collider->getPosition();
				float distance = sqrt((centre2.x - centre1.x) * (centre2.x - centre1.x) + (centre2.y - centre1.y) * (centre2.y - centre1.y));

				if (distance <= sumOfRadii)
				{
					handleCollision(context, currentObject, comparisonObject);
				}
			}
		}
	}

	static void handleCollision(EngineContext& context, CircleCollision* object1, CircleCollision* object2)
	{
		float radius1 = object1->collider->getRadius();
		float radius2 = object2->collider->getRadius();
		sf::Vector2f centre1 = object1->collider->getGeometricCenter();
		sf::Vector2f centre2 = object2->collider->getGeometricCenter();
		sf::Vector2f normal;

		if (centre2 != centre1)
		{
			normal = (centre2 - centre1).normalized();
		}
		else
		{
			normal = { 1,0 };
		}

		sf::Vector2f tangent(-normal.y, normal.x);
		float distance = sqrt((centre2.x - centre1.x) * (centre2.x - centre1.x) + (centre2.y - centre1.y) * (centre2.y - centre1.y));

		// Separate circles if overlapping
		float overlap = radius1 + radius2 - distance;
		if (overlap > 0)
		{
			centre1 -= normal * (overlap * 0.5f);
			centre2 += normal * (overlap * 0.5f);
		}

	}

	void onUpdate(EngineContext& context)
	{
	}
};
std::vector<CircleCollision*> CircleCollision::objects;

class Neutron : public Behaviour
{
private:
	// Initalises circle variable and component variable 
	sf::CircleShape circle;
	Movement* component;
	CircleCollision* component2;

public:
	// Neutron constructor which creates the neutron object
	Neutron(sf::Vector2f pos)
	{
		// Fills in the empty information from variable "circle"
		circle = sf::CircleShape(5.f);
		circle.setFillColor(sf::Color(81, 87, 83));
		circle.setOrigin({ circle.getRadius(), circle.getRadius() });
		circle.setPosition(pos);

		// Adds and configures a Movement and collisioncomponent
		component = AddComponent<Movement>();
		component2 = AddComponent<CircleCollision>();
		component2->collider = &circle;
		component2->movement = component;
		// Sets velocity and position components
		sf::Vector2f randVector;
		randVector.x = randFloat(-500, 500);
		randVector.y = randFloat(-500, 500);
		component->velocity = randVector;
		component->position = pos;
	}
	// Updates the components every time main update function called
	void onUpdate(EngineContext& context) override
	{
		// Syncs circle’s position with its Movement component’s position
		circle.setPosition(component->position);
	}
	// Renders the updated object
	void onRender(EngineContext& context) override
	{
		context.window.draw(circle);
	}
};
// Same as neutron but without movement
class Nucleus : public Behaviour
{
private:
	// Simple static circle representation
	sf::CircleShape circle;

public:
	// Constructor that sets up circle position and visuals
	Nucleus(sf::Vector2f pos)
	{
		circle = sf::CircleShape(10.f);
		circle.setFillColor(sf::Color(0, 0, 255));
		circle.setOrigin({ circle.getRadius(), circle.getRadius() });
		circle.setPosition(pos);
	}

	void onUpdate(EngineContext& context) override
	{
		// Nucleus has no movement or update logic
	}

	void onRender(EngineContext& context) override
	{
		context.window.draw(circle);
	}
};
// Self explanatory
sf::Vector2f mousePos(sf::RenderWindow& window)
{
	// Get mouse position in pixel coordinates
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

	// Convert to floating-point vector for SFML usage
	return sf::Vector2f(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));
}
// Creates class which controls when particles are spawned in
class ParticleSpawner : public Behaviour
{
private:

public:

	void onRender(EngineContext& context) override
	{
	}
	// Creates objects when correct buttons are pressed
	void onUpdate(EngineContext& context) override
	{
		if (Input::MouseHeld(sf::Mouse::Button::Left))
		{
			for (int i = 0; i < 1; i++)
			{
				std::unique_ptr<Neutron> neutron = std::make_unique<Neutron>(mousePos(context.window));
				Behaviour::CreateObject(std::move(neutron));
			}
		}
		if (Input::MouseHeld(sf::Mouse::Button::Right))
		{
			std::unique_ptr<Nucleus> nucleus = std::make_unique<Nucleus>(mousePos(context.window));
			Behaviour::CreateObject(std::move(nucleus));
		}
	}
};

int main()
{
	// Creates SFML window using the desktop resolution
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML Window", sf::State::Windowed);
	window.setVerticalSyncEnabled(true);
	EngineContext context(window);
	std::unique_ptr<ParticleSpawner> spawner = std::make_unique<ParticleSpawner>();

	// Calls CreateObject, adding the spawner to the Behaviour system
	Behaviour::CreateObject(std::move(spawner));
	sf::Clock clock;

	// Main application loop
	while (window.isOpen())
	{
		context.deltaTime = clock.restart().asSeconds();
		Behaviour::RenderAll(context);
		Behaviour::UpdateAll(context);
		CircleCollision::checkCollision(context);
	}
}
