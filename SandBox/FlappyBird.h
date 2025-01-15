#pragma once
#include "Harmony/Harmony.h"
#include <SFML/Graphics.hpp>

namespace FlappyBird
{
    // Bird class: Represents the player character
    class Bird : public Harmony::Core::SceneNode
    {
    public:
        Bird();

        void updateCurrent(const sf::Time& time) override;
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

        void handleInput();
        void jump();

    private:
        sf::Vector2f velocity;

    public:
        sf::CircleShape drawable;  // The bird's graphical representation
    };

    // Pipes class: Represents the obstacles the bird must avoid
    class Pipes : public Harmony::Core::SceneNode
    {
    public:
        Pipes();

        void updateCurrent(const sf::Time& time) override;
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

        bool checkCollision(const Bird& bird);

    public:
        sf::RectangleShape drawable;  // The graphical representation of the pipes
    };

    // GamePlayScene class: Holds the logic for the actual game scene
    class GamePlayScene : public Harmony::Core::Scene
    {
    public:
        GamePlayScene();

        void update(const sf::Time& time) override;
    };

    // GamePlayState class: Represents the gameplay state
    class GamePlayState : public Harmony::Core::State
    {
    public:
        GamePlayState();

        void update(const sf::Time& time) override;
        void onEnter() override;
        void onExit() override;
    };

    // Application class: Initializes and runs the game
    class Application : public Harmony::Core::Engine
    {
    public:
        Application();

    };
}
