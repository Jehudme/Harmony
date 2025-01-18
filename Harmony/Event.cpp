#include "pch.h"
#include "Event.h"




namespace Harmony::Core {

    Event_t::Event_t(const std::string& type, const uint64_t& uniqueId)
        : Core::Object(), type(type) {
    }

    void Event_t::execute() {
        if (function) {
            function(*this);
        }
    }

    void EventPool::addEvent(const std::shared_ptr<Event_t> event)
    {
        m_events.emplace(event);
    }

    void Harmony::Core::EventPool::handleEvent()
    {
        while (!m_events.empty())
        {
            m_events.front()->execute();
            m_events.pop();
        }

    }

}
