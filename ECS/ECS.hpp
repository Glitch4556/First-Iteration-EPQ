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

    // Called once per frame to update th
