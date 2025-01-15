#include "FlappyBird.h"

namespace FlappyBird
{
    // Bird class implementation
    Bird::Bird()
    {
        drawable.setRadius(20.f);
        drawable.setFillColor(sf::Color::Yellow);
        drawable.setOrigin(drawable.getRadius(), drawable.getRadius());
        drawable.setPosition(100.f, 300.f);  // Initial position
    }

    void Bird::handleInput()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            jump();
        }
    }

    void Bird::jump()
    {
        velocity.y = -200.f;  // Set the upward velocity for the jump
    }

    void Bird::updateCurrent(const sf::Time& time)
    {
        handleInput();

        // Gravity effect
        velocity.y += 500.f * time.asSeconds();  // Apply gravity

        // Update the bird's position based on velocity
        sf::Vector2f newPos = drawable.getPosition() + velocity * time.asSeconds();
        drawable.setPosition(newPos);
    }

    void Bird::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(drawable, states);
    }

    // Pipes class implementation
    Pipes::Pipes()
    {
        drawable.setSize(sf::Vector2f(60.f, 500.f));
        drawable.setFillColor(sf::Color::Green);
        drawable.setPosition(800.f, 0.f);  // Initial position of pipe
    }

    void Pipes::updateCurrent(const sf::Time& time)
    {
        // Move the pipes leftward at a constant speed
        sf::Vector2f newPos = drawable.getPosition() - sf::Vector2f(20 * time.asSeconds(), 0.f);
        drawable.setPosition(newPos);

        // Reset position once pipe is off-screen
        if (drawable.getPosition().x < -drawable.getSize().x) {
            drawable.setPosition(800.f, 0.f);  // Reset pipe position
        }
    }

    bool Pipes::checkCollision(const Bird& bird)
    {
        // Simple collision check (you may need to expand it for better accuracy)
        return drawable.getGlobalBounds().intersects(bird.drawable.getGlobalBounds());
    }

    void Pipes::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(drawable, states);
    }

    // GamePlayScene class implementation
    GamePlayScene::GamePlayScene()
    {
        // Create bird and pipes in the scene
        m_rootNode->attachChild(std::make_shared<Bird>());
        m_rootNode->attachChild(std::make_shared<Pipes>());
    }

    void GamePlayScene::update(const sf::Time& time)
    {
        // Update both the bird and pipes each frame
        if (auto bird = std::dynamic_pointer_cast<Bird>(m_rootNode->m_children[0])) {
            bird->update(time);
        }
        if (auto pipes = std::dynamic_pointer_cast<Pipes>(m_rootNode->m_children[1])) {
            pipes->update(time);
        }
    }

    // GamePlayState class implementation
    GamePlayState::GamePlayState()
    {
        // Initialize the game scene
        auto gameScene = std::make_shared<GamePlayScene>();
        addScene("GamePlay", gameScene);
        switchToScene("GamePlay");
    }

    void GamePlayState::update(const sf::Time& time)
    {
        if (auto currentScene = getScene("GamePlay")) {
            currentScene->update(time);
        }
    }

    void GamePlayState::onEnter()
    {
        // Additional setup when entering the game state
    }

    void GamePlayState::onExit()
    {
        // Cleanup when exiting the game state
    }

    // Application class implementation
    Application::Application()
    {
        renderWindow.create(sf::VideoMode(800, 600), "Flappy Bird");
        stateManager->pushState(std::make_shared<GamePlayState>());
    }
}

