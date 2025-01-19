#include "pch.h"
#include "Event.h"




namespace harmony::core {

    Event_t::Event_t(const std::string& type, const uint64_t& uniqueId)
        : core::Object(), type(type) {
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

    void harmony::core::EventPool::handleEvent()
    {
        while (!m_events.empty())
        {
            m_events.front()->execute();
            m_events.pop();
        }

    }

}
