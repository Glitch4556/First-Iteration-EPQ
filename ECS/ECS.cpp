#include "../ECS/ECS.hpp"

// Creates object list and object queue 
std::vector<std::unique_ptr<Behaviour>> Behaviour::objects;   
static std::vector<std::unique_ptr<Behaviour>> objQueue;      

// Adds a new object to the creation queue
void Behaviour::CreateObject(std::unique_ptr<Behaviour> obj)
{
    // Move the unique pointer into the queue to be added next frame
    objQueue.push_back(std::move(obj));
}
// Defines RenderAll function, renders all updated objects after clearing window
void Behaviour::RenderAll(EngineContext& context)
{
    context.window.clear(sf::Color::Black);
	for (auto& object : Behaviour::objects)
	{
		object->Render(context);
	}
	context.window.display();
}

void Behaviour::UpdateAll(EngineContext& context)
{
	// Closes window when escape key or X is pressed
	while (const std::optional event = context.window.pollEvent())
	{
        // Close window if the "X" button is clicked
		if (event->is<sf::Event::Closed>()) {
			context.window.close();
		}

        // Check for key press events
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
            // Close window when ESC key is pressed
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				context.window.close();
		}
	}

	// Calls update function, running onUpdate function on all objects
	for (auto& object : Behaviour::objects)
	{
		object->Update(context);
	}

	// Move any newly created objects from the queue into the active object list, so they appear next frame
	std::move(objQueue.begin(), objQueue.end(), std::back_inserter(Behaviour::objects));

	// Clear the queue after transferring the objects
	objQueue.clear();
}
