#include <Harmony/Engine.h>
#include <Harmony/Rectangle.h>

static uint64_t rectangle_config_id = 70;

class Engine : public Harmony::Engine
{
public:
    Engine(const std::shared_ptr<Harmony::Configuration> configuration) : Harmony::Engine(configuration)
    {
        // Initialize the rectangle
        rectangle_.setSize(sf::Vector2f(70, 70));
        rectangle_.setFillColor(sf::Color::Red); // Example color
        rectangle_.setPosition(100, 100);       // Example position
    }

private:
    void update() override {
        try {
            // Update logic can go here (if needed for animation, game state, etc.)
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Exception during update: {}", e.what());
            throw;
        }
    }

    void draw() override {
        try {
            renderWindow_.clear(sf::Color::Black);

            // Draw the persistent rectangle
            renderWindow_.draw(rectangle_);

            renderWindow_.display();
            HM_LOGGER_DEBUG("Frame rendered successfully.");
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Exception during drawing: {}", e.what());
            throw;
        }
    }

private:
    sf::RectangleShape rectangle_;
};

int main()
{
    {   // Rectangle
        auto configuration = Harmony::Object::create<Harmony::Configuration>(rectangle_config_id);
        configuration->retainSelf();
    }

    {   // Engine
        auto configuration = Harmony::Object::create<Harmony::Configuration>(NULL);
        Harmony::Object::create<Engine>(configuration)->run();
    }
}
