#include "pch.h"
#include "Event.h"

Harmony::Event::Event(const std::string& type, const uint64_t& uniqueId) : 
	Core::Object(uniqueId) {}

const std::string Harmony::Event::getType() const {
	return m_type;
}

void Harmony::Event::setFunction(const std::function<void(const Event&)>& function) {
	m_function = function;
}

void Harmony::Event::executeFunction() {
	m_function(*this);
}

Harmony::EventQueue::EventQueue(const uint64_t& uniqueId) :
	Core::Object(uniqueId) {}

void Harmony::EventQueue::addEvent(const std::shared_ptr<Event>& event)
{
	if(event)
		m_eventQueue.push(event);
}

void Harmony::EventQueue::processEvents()
{
	while (!m_eventQueue.empty()) {
		std::shared_ptr<Event> event = m_eventQueue.front();
		if (event && event->m_function) 
			event->executeFunction();

		m_eventQueue.pop();
	}
}

void Harmony::EventQueue::clearEvents()
{
	while (!m_eventQueue.empty()) {
		m_eventQueue.pop();
	}
}

bool Harmony::EventQueue::hasEvents() const {
	return m_eventQueue.empty();
}
