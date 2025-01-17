#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <unordered_map>
#include <memory>

namespace Harmony::Core 
{
	class Object : private sf::NonCopyable, public std::enable_shared_from_this<Object> 
	{
	public:
		Object(const uint64_t uniqueId = NULL);
		virtual ~Object();

		void retainSelf();
		void releaseSelf();

	public:
		template<typename Type, typename... ARGS>
		static std::shared_ptr<Type> create(ARGS&&... args);

		template<typename Type>
		static std::shared_ptr<Type> find(const uint64_t& uniqueId);

	public:
		const uint64_t uniqueId;

	private:
		static inline std::unordered_map<uint64_t, std::weak_ptr<Object>> m_registers;
		static inline std::unordered_map<Object*, std::shared_ptr<Object>> m_retained;
	};
}

namespace Harmony::Utilities
{
	uint64_t generateRandomId();
}

template<typename Type, typename ...ARGS>
inline std::shared_ptr<Type> Harmony::Core::Object::create(ARGS && ...args)
{
	static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Object");

	std::shared_ptr<Object> object = std::make_shared<Type>(std::forward<ARGS>(args)...);
	m_registers[object->uniqueId] = object;

	return std::static_pointer_cast<Type>(object);
}

template<typename Type>
inline std::shared_ptr<Type> Harmony::Core::Object::find(const uint64_t& uniqueId)
{
	if (!m_registers.contains(uniqueId))
		throw std::runtime_error("Object Not Find");

	std::shared_ptr<Object> object = m_registers[uniqueId].lock();

	try {
		return std::dynamic_pointer_cast<Type>(object);
	} catch (const std::exception&) {
		return nullptr;
	}
}
