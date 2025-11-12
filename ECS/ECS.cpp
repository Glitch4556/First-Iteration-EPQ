#include "../ECS/ECS.hpp"

// Creates object list and object queue 
std::vector<std::unique_ptr<Behaviour>> Behaviour::objects;
static std::vector<std::unique_ptr<Behaviour>> objQueue;

void Behaviour::CreateObject(std::unique_ptr<Behaviour> obj)
{
    objQueue.push_back(std::move(obj));
}

void Behaviour::RenderAll(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
	for (auto& object : Behaviour::objects)
	{
		object->Render(window);
	}
	window.display();
}

void Behaviour::UpdateAll(sf::RenderWindow& window)
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				window.close();
		}
		else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				
			}
		}
	}

	for (auto& object : Behaviour::objects)
	{
		object->Update(window);
	}

	// Moves objects in the queue to the list of scene objects and clears the queue
	std::move(objQueue.begin(), objQueue.end(), std::back_inserter(Behaviour::objects));
	objQueue.clear();
}
