
#include "Asteroid.h"
#include "Player.h"
#include "Bullet.h"
#include "PlayerTrail.h"
#include "Harmony/Scene.h"


namespace Asteroid
{
    // Constants for player movement and configuration
    constexpr float PlayerRadius = 10.0f;
    constexpr float PlayerOutlineThickness = 3.0f;
    constexpr int PlayerPointCount = 3;
    constexpr float RotationSpeed = 580.0f;   // Faster rotation for quicker turning
    constexpr float MaxVelocity = 300.0f;     // Increased max velocity for faster movement
    constexpr float MinVelocity = -300.0f;    // Increased min velocity for faster reverse
    constexpr float AccelerationSpeed = 300.0f; // Higher acceleration for more dynamic movement
    constexpr float BulletCooldown = 0.1;    // Faster bullet cooldown for quicker firing
    constexpr float TrailCooldown = 0.001;     // Faster trail cooldown for more frequent trail effects
    constexpr float AccelerationDeadzone = .5f; // Smaller deadzone for more fluid movement

    const uint64_t Asteroid::PlayerUniqueId = Harmony::Utilities::generateRandomNumber<uint64_t>();

    Player::Player() : Harmony::Core::SceneNode(PlayerUniqueId)
    {
    }

    void Asteroid::Player::onEnterCurrent(Harmony::Core::Scene& scene)
    {
        this->drawable = std::make_shared<sf::CircleShape>();
        auto circle = std::static_pointer_cast<sf::CircleShape>(drawable);

        circle->setOutlineColor(sf::Color::White);
        circle->setFillColor(sf::Color::Black);
        circle->setOutlineThickness(PlayerOutlineThickness);
        circle->setPointCount(PlayerPointCount);
        circle->setRadius(PlayerRadius);

        setPosition(scene.view.getCenter());
        setOrigin(PlayerRadius, PlayerRadius); // Set the origin at the right side of the circle
    }

    void Asteroid::Player::onExitCurrent(Harmony::Core::Scene& scene)
    {
        // Handle any necessary cleanup here
    }

    static float degreesToRadians(float degrees)
    {
        constexpr float Pi = 3.1415926535f;
        return degrees * Pi / 180.0f;
    }

    class AttachChildEvent : public Harmony::Core::Event_t
    {
    public:
        AttachChildEvent(
            const std::shared_ptr<Harmony::Core::SceneNode> parent,
            const std::shared_ptr<Harmony::Core::SceneNode> child) :
            m_parent(parent), m_child(child) {
        }

        void execute() override {
            m_parent->attachChild(m_child);
        }

    private:
        std::shared_ptr<Harmony::Core::SceneNode> m_parent;
        std::shared_ptr<Harmony::Core::SceneNode> m_child;
    };

    void Asteroid::Player::updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool)
    {
        const float deltaTime = time.asSeconds();

        const sf::FloatRect field = Harmony::Utilities::getViewBounds(currentScene->view);

        const sf::Vector2f curentPosition = getPosition();
        const sf::Vector2f currentViewSize = currentScene->view.getSize();

        const std::shared_ptr<sf::CircleShape> circle = std::static_pointer_cast<sf::CircleShape>(drawable);

        // Wrap the asteroid's position around the screen boundaries
        float newX = curentPosition.x;
        float newY = curentPosition.y;

        if (curentPosition.x < field.left)
            newX += field.width;
        else if (curentPosition.x > field.left + field.width)
            newX -= field.width;

        if (curentPosition.y < field.top)
            newY += field.height;
        else if (curentPosition.y > field.top + field.height)
            newY -= field.height;

        setPosition(newX, newY);


        // Handle shooting logic with cooldown
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && bulletClock.getElapsedTime().asSeconds() > BulletCooldown) {
            eventPool.addEvent(Harmony::Core::Object::create<AttachChildEvent>(
                std::static_pointer_cast<SceneNode>(parentNode->shared_from_this()),
                Harmony::Core::Object::create<Bullet>(getRotation(), getGlobalPosition()))
            );

            bulletClock.restart();
        }

        // Handle rotation input (turning left/right)
        rotationVelocity = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            rotationVelocity += -RotationSpeed;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            rotationVelocity += RotationSpeed;

        // Handle acceleration (Up for thrust, Down for reverse)
        positionAcceleration = { 0, 0 };
        float acceleration = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            acceleration += -AccelerationSpeed;
            if (trailClock.getElapsedTime().asSeconds() > TrailCooldown) {
                eventPool.addEvent(Harmony::Core::Object::create<AttachChildEvent>(
                    std::static_pointer_cast<SceneNode>(parentNode->shared_from_this()),
                    Harmony::Core::Object::create<PlayerTrail>(getRotation(), getGlobalPosition()))
                );
                trailClock.restart();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            acceleration = AccelerationSpeed;  // Apply positive acceleration for downward thrust
        }

        const float angle = getRotation();
        positionAcceleration.x = acceleration * sin(-Harmony::Utilities::degreesToRadians(getRotation()));
        positionAcceleration.y = acceleration * cos(-Harmony::Utilities::degreesToRadians(getRotation()));

        std::clamp(positionVelocity.x, MinVelocity, MaxVelocity);
        std::clamp(positionVelocity.y, MinVelocity, MaxVelocity);
    }
}
