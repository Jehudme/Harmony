#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>
#include <memory>

#include "Resource.h"

namespace Harmony
{
    class Configuration;

    class Texture : public Resource
    {
    public:
        // Constructor that takes a configuration
        explicit Texture(std::shared_ptr<Configuration> configuration);
		void reload() override;

        // Get the underlying SFML texture resource
        sf::Texture& getResource();

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
    };
}