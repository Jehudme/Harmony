#pragma once
#include <any>
#include <map>
#include <optional>
#include <functional>
#include <queue>
#include <string>

#include "Object.h"

namespace harmony::core {

    class Event : public core::Object {
    public:
        Event(const std::string& type = "Unknown", const uint64_t& uniqueId = 0);

        virtual void execute();

        template<typename Type>
        void setOption(const std::string& name, const Type& option);

        template<typename Type>
        std::optional<Type> getOption(const std::string& name) const;

    public:
        const std::string type;
        std::function<void(const Event&)> function;

    private:
        std::map<std::string, std::optional<std::any>> m_options;
    };

    class EventQueue : public core::Object {
    public:
        EventQueue() = default;

        void addEvent(const std::shared_ptr<Event> event);
        void handleEvents();

    private:
        std::queue<std::shared_ptr<Event>> m_events;
    };

    template<typename Type>
    void Event::setOption(const std::string& name, const Type& option) {
        m_options[name] = std::make_any<Type>(option);
    }

    template<typename Type>
    std::optional<Type> Event::getOption(const std::string& name) const {
        auto it = m_options.find(name);
        if (it == m_options.end() || !it->second.has_value())
            return std::nullopt;

        try {
            return std::any_cast<Type>(it->second.value());
        }
        catch (const std::bad_any_cast&) {
            return std::nullopt;
        }
    }

}


