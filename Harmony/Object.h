#pragma once
#include "Logger.h"
#include <stdexcept>

namespace Harmony {

    constexpr uint64_t INVALID_UNIQUE_ID = 0;

    class Configuration;

    class Object : public std::enable_shared_from_this<Object>,
        private sf::NonCopyable
    {
    public:
        Object(const uint64_t uuid = INVALID_UNIQUE_ID);
        Object(const std::shared_ptr<Configuration>& configuration);
        virtual                         ~Object();

        void                            setID(uint64_t uuid);
        uint64_t                        getID() const;

        void                            retainSelf();
        void                            releaseSelf();
        bool                            isSelfRetained() const;

        static uint64_t                 generateRandomUniqueID();

        template                        <typename Type, typename... Args>
        static std::shared_ptr<Type>    create(Args&&... args);

        template                        <typename Type>
        static std::shared_ptr<Type>    find(uint64_t uuid);

    private:
        void                            initialize();

    private:
        static inline std::unordered_map<uint64_t, std::weak_ptr<Object>>
            objectRegistry;

        static inline std::unordered_map<const Object*, std::shared_ptr<Object>>
            retainedObjects;

    protected:
        std::string                     typeId_;
        uint64_t                        uniqueID_ = INVALID_UNIQUE_ID;
        std::shared_ptr<Object>         selfPointer_;
    };

    template <typename Type, typename... Args>
    std::shared_ptr<Type> Object::create(Args&&... args) {
        static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Object");
        try {
            auto object = std::make_shared<Type>(std::forward<Args>(args)...);
            object->typeId_ = typeid(Type).name();

            object->Object::initialize();
            return object;
        }
        catch (const std::exception& e) {
            HM_LOGGER_ERROR("Error creating object of type '{}': {}", typeid(Type).name(), e.what());
            throw;
        }
    }

    template <typename Type>
    std::shared_ptr<Type> Object::find(uint64_t uuid) {
        static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Object");
        auto it = objectRegistry.find(uuid);
        if (it != objectRegistry.end()) {
            auto object = std::dynamic_pointer_cast<Type>(it->second.lock());
            if (object) {
                return object;
            }
            else {
                HM_LOGGER_ERROR("Failed to cast object with ID {} to type '{}'.", uuid, typeid(Type).name());
                throw std::runtime_error("Object casting failed.");
            }
        }

        HM_LOGGER_ERROR("Object with ID {} not found in the registry.", uuid);
        throw std::runtime_error("Object not found.");
    }
}
