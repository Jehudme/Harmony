#include "pch.h"
#include "Font.h"
#include "Configuration.h"

namespace Harmony
{
    Font::Font(std::shared_ptr<Configuration> configuration)
        : Object(configuration), isValid_(false)
    {
        // Load the font from a file if the path is provided
        if (const auto fontPathData = configuration->get({ "Path" }))
        {
            std::string fontPath = fontPathData.value().get<std::string>();
            loadFromFile(fontPath);
        }
        // Set font properties
        if (const auto smoothData = configuration->get({ "Smooth" }))
        {
            setSmooth(smoothData.value().get<bool>());
        }
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

    sf::Font& Font::getResource()
    {
        return resource_;
    }

    bool Font::isValid() const
    {
        return isValid_;
    }
}