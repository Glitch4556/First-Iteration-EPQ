#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <vector>
#include <optional>

// Base class for all components that can be attached to Behaviours
class Component
{
public:
    virtual ~Component() = default;

    // Called every frame for logic updates specific to this component
    virtual void onUpdate() {}

    // Determines whether this component should currently be active
    bool active = true;
};

// Base class for all entities that appear in the scene (e.g., game objects)
class Behaviour
{
public:
    // Holds all active Behaviour objects currently in the scene
    static std::vector<std::unique_ptr<Behaviour>> objects;

    virtual ~Behaviour() = default;

    // Called once per frame to update this object
    virtual void onUpdate(sf::RenderWindow& window) {}

    // Called once per frame to render this object
    virtual void onRender(sf::RenderWindow& window) {}

    // Adds a component of type T to this Behaviour and returns a pointer to it
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        // Create the new component and store it in the components list
        std::unique_ptr<T> comp = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = comp.get();
        components.push_back(std::move(comp));
        return ptr;
    }

    // Queues a new Behaviour object for creation (added at the end of the frame)
    static void CreateObject(std::unique_ptr<Behaviour> obj);

    // Renders all active objects in the scene
    static void RenderAll(sf::RenderWindow& window);

    // Updates all active objects and handles window events
    static void UpdateAll(sf::RenderWindow& window);

    // Calls onUpdate for all attached components, then for the Behaviour itself
    void Update(sf::RenderWindow& window)
    {
        for (auto& c : components)
        {
            if (c->active)
                c->onUpdate();
        }
        onUpdate(window);
    }

    // Calls onRender for this Behaviour
    void Render(sf::RenderWindow& window)
    {
        onRender(window);
    }

private:
    // List of components attached to this Behaviour
    std::vector<std::unique_ptr<Component>> components;
};
