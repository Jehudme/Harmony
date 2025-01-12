#include "pch.h"
#include "Resource.h"
#include "Logger.h"

namespace Harmony
{
    template<typename Type>
    Resource<Type>::Resource()
        : Object()
    {
        HM_LOGGER_INFO("Resource default constructor called.");
    }

    template<typename Type>
    Resource<Type>::Resource(const uint64_t& configuration_id)
        : Object(configuration_id)
    {
        HM_LOGGER_INFO("Resource initialized with configuration ID: {}", configuration_id);
    }

    template<typename Type>
    Resource<Type>::Resource(const std::shared_ptr<Configuration> configuration)
        : Object(configuration->getID())
    {
        HM_LOGGER_INFO("Resource initialized with configuration ID from shared Configuration.");
    }

    template<typename Type>
    Type* Resource<Type>::getResource()
    {
        return &resourceInstance_;
    }

    template<typename Type>
    void Resource<Type>::setResource(const std::string& filePath)
    {
        HM_LOGGER_INFO("Attempting to load resource from file: {}", filePath);

        if (!loadFromFile(filePath)) {
            HM_LOGGER_ERROR("Failed to load resource from file: {}", filePath);
            throw std::runtime_error("Unable to load resource: " + filePath);
        }

        HM_LOGGER_INFO("Resource successfully loaded from file: {}", filePath);
    }

    template<typename Type>
    bool Resource<Type>::loadFromFile(const std::string& filePath)
    {
        if constexpr (std::is_same<Type, sf::Texture>::value) {
            return resourceInstance_.loadFromFile(filePath);
        }
        else if constexpr (std::is_same<Type, sf::Font>::value) {
            return resourceInstance_.loadFromFile(filePath);
        }
        else if constexpr (std::is_same<Type, sf::SoundBuffer>::value) {
            return resourceInstance_.loadFromFile(filePath);
        }
        else if constexpr (std::is_same<Type, sf::Shader>::value) {
            return resourceInstance_.loadFromFile(filePath, sf::Shader::Fragment); // Example for fragment shaders
        }
        else {
            HM_LOGGER_ERROR("Unsupported resource type for file: {}", filePath);
            return false;
        }
    }

    // Explicit template instantiations for supported SFML types
    template class Resource<sf::Texture>;
    template class Resource<sf::Font>;
    template class Resource<sf::SoundBuffer>;
    template class Resource<sf::Shader>;
}

