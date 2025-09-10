#include "pch.h"
#include "Font.h"
#include "Configuration.h"

namespace {
    // Configuration keys
    const std::string CONFIG_PATH = "Path";
    const std::string CONFIG_SMOOTH = "Smooth";
}

namespace Harmony
{
    Font::Font(std::shared_ptr<Configuration> configuration)
        : Resource(configuration) {
		reload();
    }

    void Font::loadFromFile(const std::string& path)
    {
        isValid_ = resource_.loadFromFile(path);
    }

    void Font::loadFromMemory(const void* data, std::size_t size)
    {
        isValid_ = resource_.loadFromMemory(data, size);
    }

    void Font::setSmooth(bool smooth)
    {
        resource_.setSmooth(smooth);
    }

    void Harmony::Font::reload()
    {
        // Load the font from a file if the path is provided
        if (const auto fontPathData = configuration_->get({ CONFIG_PATH }))
        {
            std::string fontPath = fontPathData.value().get<std::string>();
            loadFromFile(fontPath);
        }
        // Set font properties
        if (const auto smoothData = configuration_->get({ CONFIG_SMOOTH }))
        {
            setSmooth(smoothData.value().get<bool>());
        }
    }

    sf::Font& Font::getResource()
    {
        return resource_;
    }
}