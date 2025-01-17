#pragma once
#include <any>
#include <map>
#include <optional>
#include <functional>
#include <queue>

#include "Object.h"

namespace Harmony
{
	class EventQueue;

	class Event : public Core::Object
	{
	public:
		Event(const std::string& type, const uint64_t& uniqueId = NULL);

		const std::string getType() const;

		void setFunction(const std::function<void(const Event&)>& function);
		void executeFunction();

		template<typename Type>
		std::optional<Type> getData(const std::string& name) const;

		template<typename Type>
		void setData(const std::string& name, const Type& data);

		friend EventQueue;

	private:
		const std::string m_type;
		std::function<void(const Event&)> m_function;
		std::map<std::string, std::any> m_dataMap;
	};

	class EventQueue : public Core::Object
	{
	public:
		EventQueue(const uint64_t& uniqueId = NULL);

		void addEvent(const std::shared_ptr<Event>& event);
		void processEvents();
		void clearEvents();

		bool hasEvents() const;

	private:
		std::queue<std::shared_ptr<Event>> m_eventQueue;
	};
}

template<typename Type>
inline std::optional<Type> Harmony::Event::getData(const std::string& name) const
{
	if (!m_dataMap.contains(name))
		return std::nullopt;

	try {
		return std::make_optional<Type>(std::any_cast<Type>(m_dataMap[name]));
	} catch (const std::exception&) {
		return std::nullopt;
	}
}

template<typename Type>
inline void Harmony::Event::setData(const std::string& name, const Type& data)
{
	m_dataMap[name] = std::make_any<Type>(data);
}

