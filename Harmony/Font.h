#pragma once
#include "Object.h"
#include <SFML/Graphics/Font.hpp>
#include <memory>
#include "Configuration.h"

namespace Harmony
{
    class Font : public Object
    {
    public:
        // Constructor that takes a configuration
        explicit Font(std::shared_ptr<Configuration> configuration);

        // Get the underlying SFML font resource
        sf::Font& getResource();

        // Check if the font is valid (loaded successfully)
        bool isValid() const;

    private:
        // Helper methods to configure the font
        void loadFromFile(const std::string& path);
        void loadFromMemory(const void* data, std::size_t size);

        // Configure font properties
        void setSmooth(bool smooth);

        sf::Font resource_; // The SFML font resource
        bool isValid_;      // Flag to indicate if the font is valid
    };
}