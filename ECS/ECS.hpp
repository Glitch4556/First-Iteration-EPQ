#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Component
{
public:
	virtual void onUpdate() = 0;
};

class Behaviour
{
private:
	static std::vector<std::unique_ptr<Behaviour>> objects;
	std::vector<std::unique_ptr<Component>> components;

protected:
	virtual void onUpdate(sf::RenderWindow&) = 0;
	virtual void onRender(sf::RenderWindow&) = 0;
	void Update(sf::RenderWindow& window)
	{
		onUpdate(window);
		for (auto& component : Behaviour::components)
		{
			component->onUpdate();
		}
	}
	void Render(sf::RenderWindow& window)
	{
		onRender(window);
	}

public:
	static void CreateObject(std::unique_ptr<Behaviour> obj);
	static void RenderAll(sf::RenderWindow& window);
	static void UpdateAll(sf::RenderWindow& window);

	template<typename T>
	T* AddComponent()
	{
		components.push_back(std::make_unique<T>());
		return (T*)components.back().get();
	}
};
//67 67 67 67 67 67 67 67 