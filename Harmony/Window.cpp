#include "pch.h"
#include "Window.h"
#include "Configuration.h"
#include "Logger.h"

harmony::Window::Window(const uint64_t& uniqueId) : Object(uniqueId)
{
    const std::string uniqueIdKey = std::to_string(uniqueId);

    LOG_TRACE(Logger::core, "[Window] Creating Window object with unique ID: {}", uniqueId);

    const auto titleOpt = configuration->getData<std::string>({ "Objects", uniqueIdKey, "title" });
    const auto widthOpt = configuration->getData<unsigned int>({ "Objects", uniqueIdKey, "width" });
    const auto heightOpt = configuration->getData<unsigned int>({ "Objects", uniqueIdKey, "height" });
    const auto frameRateLimitOpt = configuration->getData<unsigned int>({ "Objects", uniqueIdKey, "frameRateLimit" });
    const auto verticalSyncEnabledOpt = configuration->getData<bool>({ "Objects", uniqueIdKey, "verticalSyncEnabled" });

    const std::string title = titleOpt.value_or("Harmony Engine");
    if (!titleOpt) {
        LOG_WARN(Logger::core, "[Window] Title not found in configuration for unique ID: {}, using default: {}", this->uniqueId, title);
    }

    const unsigned int width = widthOpt.value_or(600);
    if (!widthOpt) {
        LOG_WARN(Logger::core, "[Window] Width not found in configuration for unique ID: {}, using default: {}", this->uniqueId, width);
    }

    const unsigned int height = heightOpt.value_or(600);
    if (!heightOpt) {
        LOG_WARN(Logger::core, "[Window] Height not found in configuration for unique ID: {}, using default: {}", this->uniqueId, height);
    }

    const unsigned int frameRateLimit = frameRateLimitOpt.value_or(60);
    if (!frameRateLimitOpt) {
        LOG_WARN(Logger::core, "[Window] Frame rate limit not found in configuration for unique ID: {}, using default: {}", this->uniqueId, frameRateLimit);
    }

    const bool verticalSyncEnabled = verticalSyncEnabledOpt.value_or(true);
    if (!verticalSyncEnabledOpt) {
        LOG_WARN(Logger::core, "[Window] Vertical sync enabled not found in configuration for unique ID: {}, using default: {}", this->uniqueId, verticalSyncEnabled);
    }

    LOG_INFO(Logger::core, "[Window] Window properties for unique ID: {}: title='{}', width={}, height={}, frameRateLimit={}, verticalSyncEnabled={}", this->uniqueId, title, width, height, frameRateLimit, verticalSyncEnabled);

    const sf::VideoMode videoMode(width, height);

    // Set window properties based on provided arguments
    instance = std::make_shared<sf::RenderWindow>();

    instance->onEnter(videoMode, title);
    instance->setFramerateLimit(frameRateLimit);
    instance->setVerticalSyncEnabled(verticalSyncEnabled);

    LOG_TRACE(Logger::core, "[Window] Window object created successfully with unique ID: {}", uniqueId);
}

