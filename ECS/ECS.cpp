#include "../ECS/ECS.hpp"

// Creates object list and object queue 
std::vector<std::unique_ptr<Behaviour>> Behaviour::objects;   // Holds all active Behaviour objects currently in the scene
static std::vector<std::unique_ptr<Behaviour>> objQueue;      // Temporary storage for new objects to be added after updates

// Adds a new object to the creation queue
void Behaviour::CreateObject(std::unique_ptr<Behaviour> obj)
{
    // Move the unique pointer into the queue to be added next frame
    objQueue.push_back(std::move(obj));
}

// Handles rendering for all Behaviour objects in the scene
void Behaviour::RenderAll(sf::RenderWindow& window)
{
    // Clear the screen before drawing anything
    window.clear(sf::Color::Black);

    // Render each active object
	for (auto& object : Behaviour::objects)
	{
		object->Render(window);
	}

    // Display everything drawn this frame
	window.display();
}

// Handles event polling and updates all Behaviour objects
void Behaviour::UpdateAll(sf::RenderWindow& window)
{
    // Poll for all window events (close, key presses, mouse input, etc.)
	while (const std::optional event = window.pollEvent())
	{
        // Close window if the "X" button is clicked
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}

        // Check for key press events
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
            // Close window when ESC key is pressed
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				window.close();
		}

        // Check for mouse button presses
		else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
            // Detect if left mouse button is pressed (currently unused)
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				// Placeholder for mouse interaction logic
			}
		}
	}

    // Update all Behaviour objects currently in the scene
	for (auto& object : Behaviour::objects)
	{
		object->Update(window);
	}

	// Move any newly created objects from the queue into
    // the active object list, so they appear next frame
	std::move(objQueue.begin(), objQueue.end(), std::back_inserter(Behaviour::objects));

	// Clear the queue after transferring the objects
	objQueue.clear();
}
