#pragma once

#include "Object.h"
#include "Logger.h"
#include <map>
#include <any>
#include <optional>
#include <typeinfo>
#include <stdexcept>

namespace Harmony {

    class Configuration : public Object {
    public:
        Configuration(uint64_t uuid = INVALID_UNIQUE_ID) : Object(uuid) {}

        template<typename Type>
        void setCreateType();

        template<typename Type>
        void setParameter(const std::string& parameterName, const Type& parameter);

        template<typename Type>
        std::optional<Type> getParameter(const std::string& parameterName) const;

        template<typename Type>
        Type getParameterOrDefault(const std::string& parameterName, const Type& defaultParameter) const;

        template<typename Type>
        std::shared_ptr<Type> create() const;

    private:
        std::map<std::string, std::any> parameters;
        std::function<std::shared_ptr<Object>()> factoryFunction;
    };

    static inline auto DefaultConfiguration = Object::create<Configuration>(INVALID_UNIQUE_ID);

} // namespace Harmony


#include "Configuration.h"

namespace Harmony {

    template<typename Type>
    inline void Configuration::setCreateType() {
        factoryFunction = [self = std::dynamic_pointer_cast<Configuration>(shared_from_this())]() -> std::shared_ptr<Object> {
            return Object::create<Type>(self);
            };
    }

    template<typename Type>
    void Configuration::setParameter(const std::string& parameterName, const Type& parameter) {
        try {
            parameters[parameterName] = parameter;
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Failed to set parameter '{}' for Configuration ID [{}] [{}]: {}", parameterName, typeId_, uniqueID_, e.what());
            throw;
        }
    }

    template<typename Type>
    std::optional<Type> Configuration::getParameter(const std::string& parameterName) const {
        try {
            auto it = parameters.find(parameterName);
            if (it != parameters.end()) {
                if (auto* value = std::any_cast<Type>(&it->second)) {
                    return *value;
                }
                else {
                    HM_LOGGER_ERROR("Type mismatch for parameter '{}' in Configuration ID [{}] [{}].", parameterName, typeId_, uniqueID_);
                    return std::nullopt;
                }
            }
            return std::nullopt;
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error retrieving parameter '{}' for Configuration ID [{}] [{}]: {}", parameterName, typeId_, uniqueID_, e.what());
            throw;
        }
    }

    template<typename Type>
    Type Configuration::getParameterOrDefault(const std::string& parameterName, const Type& defaultParameter) const {
        try {
            auto parameter = getParameter<Type>(parameterName);
            if (parameter) {
                return *parameter;
            }
            return defaultParameter;
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error retrieving parameter '{}' with default for Configuration ID [{}] [{}]: {}", parameterName, typeId_, uniqueID_, e.what());
            throw;
        }
    }

    template<typename Type>
    inline std::shared_ptr<Type> Configuration::create() const {
        try {
            return std::dynamic_pointer_cast<Type>(factoryFunction());
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error creating object from factory function for Configuration ID [{}] [{}]: {}", typeId_, uniqueID_, e.what());
            throw;
        }
    }

} // namespace Harmony

