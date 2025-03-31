#include "pch.h"
#include "Texture.h"
#include "Configuration.h"
#include <SFML/Graphics/Image.hpp>

namespace Harmony
{
    namespace
    {
        // Configuration keys
        constexpr const char* CONFIG_PATH = "Path";
        constexpr const char* CONFIG_IMAGE = "Image";
        constexpr const char* CONFIG_COLOR = "Color";
        constexpr const char* CONFIG_SMOOTH = "Smooth";
        constexpr const char* CONFIG_REPEATED = "Repeated";
        constexpr const char* CONFIG_WIDTH = "Width";
        constexpr const char* CONFIG_HEIGHT = "Height";
        constexpr const char* CONFIG_COLOR_R = "R";
        constexpr const char* CONFIG_COLOR_G = "G";
        constexpr const char* CONFIG_COLOR_B = "B";
        constexpr const char* CONFIG_COLOR_A = "A";

        // Default values
        constexpr sf::Uint8 DEFAULT_COLOR_R = 255;
        constexpr sf::Uint8 DEFAULT_COLOR_G = 255;
        constexpr sf::Uint8 DEFAULT_COLOR_B = 255;
        constexpr sf::Uint8 DEFAULT_COLOR_A = 255;
        constexpr unsigned int DEFAULT_WIDTH = 128;
        constexpr unsigned int DEFAULT_HEIGHT = 128;
    }

    Texture::Texture(std::shared_ptr<Configuration> configuration)
        : Object(configuration), isValid_(false)
    {
        // Load the texture from a file if the path is provided
        if (const auto texturePathData = configuration->get({ CONFIG_PATH }))
        {
            std::string texturePath = texturePathData.value().get<std::string>();
            loadFromFile(texturePath);
        }
        // Load the texture from an image if image data is provided
        else if (const auto textureImageData = configuration->get({ CONFIG_IMAGE }))
        {
            const auto imageConfig = create<Configuration>(textureImageData.value());
            sf::Image image;

            // Load image data (e.g., from a file or raw pixel data)
            if (const auto imagePathData = imageConfig->get({ CONFIG_PATH }))
            {
                std::string imagePath = imagePathData.value().get<std::string>();
                if (image.loadFromFile(imagePath))
                {
                    loadFromImage(image);
                }
            }
        }
        // Create a texture from a solid color if color and size are provided
        else if (const auto textureColorData = configuration->get({ CONFIG_COLOR }))
        {
            const auto colorConfig = create<Configuration>(textureColorData.value());
            sf::Color color = {
                static_cast<sf::Uint8>(colorConfig->get<int>({ CONFIG_COLOR_R }).value_or(DEFAULT_COLOR_R)),
                static_cast<sf::Uint8>(colorConfig->get<int>({ CONFIG_COLOR_G }).value_or(DEFAULT_COLOR_G)),
                static_cast<sf::Uint8>(colorConfig->get<int>({ CONFIG_COLOR_B }).value_or(DEFAULT_COLOR_B)),
                static_cast<sf::Uint8>(colorConfig->get<int>({ CONFIG_COLOR_A }).value_or(DEFAULT_COLOR_A))
            };

            sf::Vector2u size = {
                static_cast<unsigned int>(colorConfig->get<int>({ CONFIG_WIDTH }).value_or(DEFAULT_WIDTH)),
                static_cast<unsigned int>(colorConfig->get<int>({ CONFIG_HEIGHT }).value_or(DEFAULT_HEIGHT))
            };

            createFromColor(color, size);
        }

        // Set texture properties
        if (const auto smoothData = configuration->get({ CONFIG_SMOOTH }))
        {
            setSmooth(smoothData.value().get<bool>());
        }

        if (const auto repeatedData = configuration->get({ CONFIG_REPEATED }))
        {
            setRepeated(repeatedData.value().get<bool>());
        }
    }

    void Texture::loadFromFile(const std::string& path)
    {
        isValid_ = resource_.loadFromFile(path);
    }

    void Texture::loadFromMemory(const void* data, std::size_t size)
    {
        isValid_ = resource_.loadFromMemory(data, size);
    }

    void Texture::loadFromImage(const sf::Image& image)
    {
        isValid_ = resource_.loadFromImage(image);
    }

    void Texture::createFromColor(const sf::Color& color, const sf::Vector2u& size)
    {
        sf::Image image;
        image.create(size.x, size.y, color);
        isValid_ = resource_.loadFromImage(image);
    }

    void Texture::setSmooth(bool smooth)
    {
        resource_.setSmooth(smooth);
    }

    void Texture::setRepeated(bool repeated)
    {
        resource_.setRepeated(repeated);
    }

    sf::Texture& Texture::getResource()
    {
        return resource_;
    }

    bool Texture::isValid() const
    {
        return isValid_;
    }
}