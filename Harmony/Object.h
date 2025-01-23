#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <unordered_map>
#include <memory>
#include "Logger.h"


namespace harmony::utilities 
{
	template<typename Type, typename ...ARGS>
	inline std::shared_ptr<Type> create(ARGS&& ...args);

	template<typename Type>
	inline std::shared_ptr<Type> find(const uint64_t& uniqueId);
}

namespace harmony::core {
    class Object : private sf::NonCopyable, public std::enable_shared_from_this<Object> {
    public:
        Object(const uint64_t uniqueId = NULL);
        virtual ~Object();

        void retainSelf();
        void releaseSelf();

        template<typename Type, typename... ARGS>
        friend std::shared_ptr<Type> utilities::create(ARGS&&... args);

        template<typename Type>
        friend std::shared_ptr<Type> utilities::find(const uint64_t& uniqueId);

    public:
        const uint64_t uniqueId;
        std::string objectType;

    private:
        static inline std::unordered_map<uint64_t, std::weak_ptr<Object>> m_registers;
        static inline std::unordered_map<Object*, std::shared_ptr<Object>> m_retained;
    };
}

namespace harmony::utilities
{
	template<typename Type, typename ...ARGS>
	inline std::shared_ptr<Type> create(ARGS&& ...args) {
		static_assert(std::is_base_of<core::Object, Type>::value, "Type must inherit from Object");

		auto object = std::make_shared<Type>(std::forward<ARGS>(args)...);
		core::Object::m_registers[object->uniqueId] = object;
		object->objectType = typeid(Type).name();

		LOG_TRACE(Logger::core, "[Object] Created object of type '{}' with ID: {}", object->objectType, object->uniqueId);
		return std::static_pointer_cast<Type>(object);
	}

	template<typename Type>
	inline std::shared_ptr<Type> find(const uint64_t& uniqueId) {
		if (!core::Object::m_registers.contains(uniqueId)) {
			LOG_ERROR(Logger::core, "[Object] Object with ID '{}' not found", uniqueId);
			throw std::runtime_error("Object Not Found");
		}

		auto object = core::Object::m_registers[uniqueId].lock();
		if (!object) {
			LOG_ERROR(Logger::core, "[Object] Object with ID '{}' has expired", uniqueId);
			return nullptr;
		}

		try {
			auto castedObject = std::dynamic_pointer_cast<Type>(object);
			if (!castedObject) {
				LOG_ERROR(Logger::core, "[Object] Failed to cast object with ID '{}' to type '{}'", uniqueId, typeid(Type).name());
			}
			return castedObject;
		}
		catch (const std::exception& e) {
			LOG_CRITICAL(Logger::core, "[Object] Exception while casting object with ID '{}': {}", uniqueId, e.what());
			return nullptr;
		}
	}
}