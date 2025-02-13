#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <stdexcept>

#include "Constants.h"

namespace Harmony {

    class Configuration;

    class Object : private sf::NonCopyable, public std::enable_shared_from_this<Object> {
    public:
        Object(uint64_t uniqueId = 0, const std::string& name = Default::NAME);
        explicit Object(std::shared_ptr<Configuration> configuration);
        virtual ~Object();

        void retain();
        void release();

        void setName(const std::string& name);
        const std::string& getName() const;

        uint64_t getUniqueId() const;

        template<typename Type, typename... Args>
        friend std::shared_ptr<Type> create(Args&&... args);

        template<typename Type, typename... Args>
        friend std::shared_ptr<Type> find(uint64_t uniqueId);

        template<typename Type, typename... Args>
        friend std::shared_ptr<Type> find(const std::string& name);

    private:
        std::string name_;
        const uint64_t uniqueId_;

        static inline std::unordered_map<uint64_t, std::weak_ptr<Object>> registeredById_;
        static inline std::unordered_map<std::string, std::weak_ptr<Object>> registeredByName_;
        static inline std::unordered_map<Object*, std::shared_ptr<Object>> retainedObjects_;
    };

    template<typename Type, typename... Args>
    std::shared_ptr<Type> create(Args&&... args) {
        static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Object");

        auto object = std::make_shared<Type>(std::forward<Args>(args)...);

        if (object->getName() != Default::NAME) {
            object->setName(object->getName());
        }

        Object::registeredById_[object->getUniqueId()] = object;
        return object;
    }

    template<typename Type, typename... Args>
    std::shared_ptr<Type> find(uint64_t uniqueId) {
        static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Object");

        if (Object::registeredById_.contains(uniqueId)) {
            if (auto object = std::dynamic_pointer_cast<Type>(Object::registeredById_[uniqueId].lock())) {
                return object;
            }
            throw std::runtime_error(Error::DYNAMIC_CAST_FAILED);
        }
        throw std::runtime_error(Error::OBJECT_NOT_FOUND);
    }

    template<typename Type, typename... Args>
    std::shared_ptr<Type> find(const std::string& name) {
        static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Object");

        if (Object::registeredByName_.contains(name)) {
            if (auto object = std::dynamic_pointer_cast<Type>(Object::registeredByName_[name].lock())) {
                return object;
            }
            throw std::runtime_error(Error::DYNAMIC_CAST_FAILED);
        }
        throw std::runtime_error(Error::OBJECT_NOT_FOUND);
    }
}