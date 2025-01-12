#pragma once

#pragma once

#include "Configuration.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <optional>

namespace Harmony
{
    template<typename Type>
    class Resource : public Object
    {
    public:
        Resource();
        Resource(const uint64_t& configuration_id);
        Resource(const std::shared_ptr<Configuration> configuration);

        Type* getResource();
        void setResource(const std::string& filePath);

    private:
        Type resourceInstance_;
        bool loadFromFile(const std::string& filePath); // Helper to load resources
    };
}
