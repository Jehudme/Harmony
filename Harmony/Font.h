#pragma once
#include <SFML/Graphics/Font.hpp>
#include <memory>

#include "Resource.h"


namespace Harmony
{
	class Configuration;

    class Font : public Resource
    {
    public:
        // Constructor that takes a configuration
        explicit Font(std::shared_ptr<Configuration> configuration);
		void reload() override;

        // Get the underlying SFML font resource
        sf::Font& getResource();

    private:
        // Helper methods to configure the font
        void loadFromFile(const std::string& path);
        void loadFromMemory(const void* data, std::size_t size);

        // Configure font properties
        void setSmooth(bool smooth);

        sf::Font resource_; // The SFML font resource
    };
}