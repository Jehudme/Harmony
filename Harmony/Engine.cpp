#include "pch.h"
#include "Engine.h"

namespace Harmony {

    Engine::Engine() {
        initialize(*Harmony::DefaultConfiguration);
    }

    Engine::Engine(const uint64_t& configuration_id) {
        initialize(*Object::find<Configuration>(configuration_id));
    }

    Engine::Engine(const std::shared_ptr<Configuration>& configuration)
        : Object(configuration->getParameterOrDefault<uint64_t>("engine_id", INVALID_UNIQUE_ID)) {
        initialize(*configuration);
    }

    void Engine::run() {
        HM_LOGGER_INFO("Starting Engine main loop...");
        try {
            while (renderWindow_.isOpen()) {
                event();
                update();
                draw();
            }
            HM_LOGGER_INFO("Engine main loop terminated gracefully.");
        }
        catch (const std::exception& e) {
            HM_LOGGER_CRITICAL("Unhandled exception during Engine run: {}", e.what());
            throw;
        }
    }

    void Engine::initialize(const Configuration& configuration) {
        HM_LOGGER_INFO("Initializing Engine with ID: {}", getID());

        try {
            // Load parameters from Configuration
            const std::string windowTitle   = configuration.getParameterOrDefault<std::string>("window_title", "Harmony Engine");
            const unsigned int width        = configuration.getParameterOrDefault<unsigned int>("window_width", 800);
            const unsigned int height       = configuration.getParameterOrDefault<unsigned int>("window_height", 600);
            const bool fullscreen           = configuration.getParameterOrDefault<bool>("fullscreen", false);
            const int framerateLimit        = configuration.getParameterOrDefault<unsigned int>("framerate_limit", 60);
            const bool verticalSync         = configuration.getParameterOrDefault<bool>("vertical_sync", true);

            const sf::Uint32 style          = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
            sf::VideoMode videoMode         = sf::VideoMode(width, height);

            // Initialize RenderWindow
            renderWindow_.create(videoMode, windowTitle, style);
            renderWindow_.setFramerateLimit(framerateLimit);
            renderWindow_.setVerticalSyncEnabled(verticalSync);

            HM_LOGGER_INFO("Engine initialized with parameters: [Title: {}, Width: {}, Height: {}, Fullscreen: {}, Framerate: {}, VSync: {}]",
                windowTitle, width, height, fullscreen, framerateLimit, verticalSync);
        }
        catch (const std::exception& e) {
            HM_LOGGER_CRITICAL("Failed to initialize Engine: {}", e.what());
            throw;
        }
    }

    void Engine::event() {
        try {
            sf::Event event;
            while (renderWindow_.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    HM_LOGGER_INFO("Close event received. Closing RenderWindow.");
                    renderWindow_.close();
                }
            }
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Exception during event handling: {}", e.what());
            throw;
        }
    }

    void Engine::update() {
        try {
            sf::Time deltaTime = clock_.restart();

            // Update game state logic here...
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Exception during update: {}", e.what());
            throw;
        }
    }

    void Engine::draw() {
        try {
            renderWindow_.clear(sf::Color::Black);

            // Draw game objects here...

            renderWindow_.display();
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Exception during drawing: {}", e.what());
            throw;
        }
    }

} // namespace Harmony

