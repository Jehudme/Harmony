#pragma once
#include "Object.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>
#include <memory>
#include "Configuration.h"

namespace Harmony
{
    class Texture : public Object
    {
    public:
        // Constructor that takes a configuration
        explicit Texture(std::shared_ptr<Configuration> configuration);

        // Get the underlying SFML texture resource
        sf::Texture& getResource();

        // Check if the texture is valid (loaded successfully)
        bool isValid() const;

    private:
        // Helper methods to configure the texture
        void loadFromFile(const std::string& path);
        void loadFromMemory(const void* data, std::size_t size);
        void loadFromImage(const sf::Image& image);
        void createFromColor(const sf::Color& color, const sf::Vector2u& size);

        // Configure texture properties
        void setSmooth(bool smooth);
        void setRepeated(bool repeated);

        sf::Texture resource_; // The SFML texture resource
        bool isValid_;         // Flag to indicate if the texture is valid
    };
}