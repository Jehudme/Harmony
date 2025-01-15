#pragma once

////////////////////////////////////////////////////////////////////////////////////
// Header
////////////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <SFML/System/NonCopyable.hpp>

#include "Random.h"

////////////////////////////////////////////////////////////////////////////////////
// Declaration 
////////////////////////////////////////////////////////////////////////////////////

namespace Harmony::Core {

	class Object : public std::enable_shared_from_this<Object>, public sf::NonCopyable
	{
	public:
		template<typename Type, typename... ARGS>
		static std::shared_ptr<Type> create(ARGS&&... args);

		template<typename Type>
		static std::shared_ptr<Type> find(const uint64_t& targetId);

		void retainSelf();
		void releaseSelf();

		virtual ~Object() = default;
		Object(const uint64_t& uniqueId = Utilities::Random::generateId());

	public:
		const uint64_t uniqueId;
		std::string type;

	private:
		static inline std::unordered_map<uint64_t, std::weak_ptr<Object>> m_objectRegistry;
		static inline std::unordered_map<Object*, std::shared_ptr<Object>> m_retainedObjects;
	};
}

////////////////////////////////////////////////////////////////////////////////////
// Definition
////////////////////////////////////////////////////////////////////////////////////

namespace Harmony::Core
{
	template<typename Type, typename ...ARGS>
	inline std::shared_ptr<Type> Object::create(ARGS && ...args)
	{
		static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Harmony::Core::Object");

		std::shared_ptr<Object> object		= std::make_shared<Type>(std::forward<ARGS>(args)...);		
		m_objectRegistry[object->uniqueId]	= object->weak_from_this();
		object->type						= typeid(Type).name();

		return object;
	}
	template<typename Type>
	inline std::shared_ptr<Type> Object::find(const uint64_t& targetId)
	{
		static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Harmony::Core::Object");

		if (!m_objectRegistry.contains(targetId))
			throw std::runtime_error("Object not find");

		std::shared_ptr<Object> object = m_objectRegistry[targetId].lock();
		return std::dynamic_pointer_cast<Type>(object);
	}
}