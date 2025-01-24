#include "pch.h"
#include "Window.h"
#include "Configuration.h"
#include <memory>
#include <string>
#include "Object.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace harmony {

    Window::Window(const Configuration& configuration)
        : Object(configuration)
    {
        const auto title = configuration.getData<std::string>({ "Title" }).value_or("Harmony Engine");
        const auto width = configuration.getData<unsigned int>({ "Width" }).value_or(600);
        const auto height = configuration.getData<unsigned int>({ "Height" }).value_or(600);
        const auto frameRateLimit = configuration.getData<unsigned int>({ "FrameRateLimit" }).value_or(60);
        const auto verticalSyncEnabled = configuration.getData<bool>({ "VerticalSyncEnabled" }).value_or(true);

        const sf::VideoMode videoMode(width, height);

        instance = std::make_shared<sf::RenderWindow>();
        instance->create(videoMode, title);
        instance->setFramerateLimit(frameRateLimit);
        instance->setVerticalSyncEnabled(verticalSyncEnabled);
    }

}
