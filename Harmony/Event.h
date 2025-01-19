#pragma once
#include <any>
#include <map>
#include <optional>
#include <functional>
#include <queue>
#include <string>

#include "Object.h"

namespace harmony::core {

    class Event_t : public core::Object {
    public:
        Event_t(const std::string& type = "Unknown", const uint64_t& uniqueId = 0);

        virtual void execute();

        template<typename Type>
        void setOption(const std::string& name, const Type& option);

        template<typename Type>
        std::optional<Type> getOption(const std::string& name) const;

    public:
        const std::string type;
        std::function<void(const Event_t&)> function;


    private:
        std::map<std::string, std::optional<std::any>> m_options;
    };

    class EventPool : public core::Object
    {
    public:
        EventPool() = default;

        void addEvent(const std::shared_ptr<Event_t> event);
        void handleEvent();

    private:
        std::queue<std::shared_ptr<Event_t>> m_events;
    };

    template<typename Type>
    void harmony::core::Event_t::setOption(const std::string& name, const Type& option) {
        m_options[name] = std::make_any<Type>(option);
    }

    template<typename Type>
    std::optional<Type> harmony::core::Event_t::getOption(const std::string& name) const {
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

