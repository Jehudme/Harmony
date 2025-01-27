#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <stdexcept>

namespace Harmony
{
	template<typename Type, typename... ARGS>
	std::shared_ptr<Type> create(ARGS&& ...args);

	template<typename Type, typename... ARGS>
	std::shared_ptr<Type> find(const uint64_t& uniqueId);

	template<typename Type, typename... ARGS>
	std::shared_ptr<Type> find(const std::string& name);

	class Object : private sf::NonCopyable, public std::enable_shared_from_this<Object> 
	{
	public:
		Object(const uint64_t& uniqueId);
		virtual ~Object();

		void retainSelf();
		void releaseSelf();

		void setName(const std::string& name);
		const std::string& getName() const;

		const uint64_t& getUniqueId() const;

		template<typename Type, typename... ARGS>
		friend std::shared_ptr<Type> Harmony::create(ARGS&& ...args);

		template<typename Type, typename... ARGS>
		friend std::shared_ptr<Type> Harmony::find(const uint64_t& uniqueId);

		template<typename Type, typename... ARGS>
		friend std::shared_ptr<Type> Harmony::find(const std::string& uniqueId);

	private:
		std::string m_name;
		const uint64_t m_uniqueId;

		bool m_isFoundByNameEnable;

	private:
		static inline std::unordered_map<uint64_t, std::weak_ptr<Object>> m_registersById;
		static inline std::unordered_map<std::string, std::weak_ptr<Object>> m_registersByName;
		static inline std::unordered_map<Object*, std::shared_ptr<Object>> m_retained;
	};



	template<typename Type, typename ...ARGS>
	std::shared_ptr<Type> create(ARGS && ...args) {
		static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Object");

		std::shared_ptr<Type> object = std::make_shared<Type>(args...);
		Object::m_registersById[object->m_uniqueId] = object;
		return object;
	}

	template<typename Type, typename ...ARGS>
	std::shared_ptr<Type> find(const uint64_t& uniqueId) {
		static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Object");

		if (Object::m_registersById.contains(uniqueId)) {
			if (std::shared_ptr<Type> object = std::dynamic_pointer_cast<Type>(Object::m_registersById[uniqueId])) {
				return object;
			}
			throw std::runtime_error("Dynamic Pointer Cast Failed");
		}
		throw std::runtime_error("Object Not Find");
	}

	template<typename Type, typename ...ARGS>
	std::shared_ptr<Type> find(const std::string& name) {
		static_assert(std::is_base_of<Object, Type>::value, "Type must inherit from Object");

		if (Object::m_registersByName.contains(name)) {
			if (std::shared_ptr<Type> object = std::dynamic_pointer_cast<Type>(Object::m_registersByName[name].lock())) {
				return object;
			}
			throw std::runtime_error("Dynamic Pointer Cast Failed");
		}
		throw std::runtime_error("Object Not Find");
	}
}

