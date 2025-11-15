#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <vector>
#include <optional>


struct EngineContext
{
	sf::RenderWindow& window;
	float deltaTime;

	EngineContext(sf::RenderWindow& _window)
		: window(_window), deltaTime(0)
	{
	}
};

// Component class which requires override of onUpdate
// This is to prevent having to loop through updating all objects
class Component
{
public:
	virtual void onUpdate(EngineContext& context) = 0;
};

// Base class for all entities that appear in the scene (e.g., game objects)
class Behaviour
{

private:
	// Initalises objects and components vectors
	static std::vector<std::unique_ptr<Behaviour>> objects;
	std::vector<std::unique_ptr<Component>> components;

protected:
	// Requires onUpdate and onRender to be defined by classes which inherit behaviour
	virtual void onUpdate(EngineContext&) = 0;
	virtual void onRender(EngineContext&) = 0;
	// Runs onUpdate on all components
	void Update(EngineContext& context)
	{
		onUpdate(context);
		for (auto& component : Behaviour::components)
		{
			component->onUpdate(context);
		}
	}
	// Renders all changes
	void Render(EngineContext& context)
	{
		onRender(context);
	}

public:
	// Weird cpp compiler stuff
	static void CreateObject(std::unique_ptr<Behaviour> obj);
	static void RenderAll(EngineContext& context);
	static void UpdateAll(EngineContext& context);

	template<typename T>
	T* AddComponent()
	{
		// Creates unqiue ptr of template type
		std::unique_ptr<T> component = std::make_unique<T>();
		// Variable "raw" which is of template type. Gets raw pointer from within component.
		T* raw = component.get();

		// Moves the component to components vector, allowing it to be updated
		components.push_back(std::move(component));
		// Returns raw, which is a pointer to the template type. Allows the modification of components.
		return raw;
	}
};
