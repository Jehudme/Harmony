#include "pch.h"
#include "Event.h"
#include <cstdint>
#include <memory>
#include <queue>
#include <string>
#include "Object.h"

namespace harmony::core {

    Event::Event(const std::string& type, const uint64_t& uniqueId)
        : core::Object(uniqueId), type(type) {
    }

    void Event::execute() {
        if (function) {
            function(*this);
        }
    }

    void EventQueue::addEvent(const std::shared_ptr<Event> event) {
        m_events.emplace(event);
    }

    void EventQueue::handleEvents()
    {
        while (!m_events.empty()) {
            m_events.front()->execute();
            m_events.pop();
        }
    }
}
