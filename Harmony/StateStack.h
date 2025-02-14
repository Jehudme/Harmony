#pragma once

#include "Object.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <stack>
#include <vector>

namespace Harmony {

    // Constants for configuration keys and error messages
    constexpr const char* CONFIG_STATES = "States";
    constexpr const char* CONFIG_INITIAL_STATE = "InitialState";
    constexpr const char* CONFIG_SCRIPT = "Script";

    constexpr const char* ERROR_NO_STATES_FOUND = "No States Found";
    constexpr const char* ERROR_STATE_NOT_FOUND = "State not found";

    class State;
    class TaskQueue;
    struct Script;

    class StateStack : public Object, public sf::Drawable {
    public:
        explicit StateStack(std::shared_ptr<Configuration> configuration);

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(const sf::Time& time, TaskQueue& taskQueue);

        void add(std::shared_ptr<State> state);

        void remove(uint64_t uniqueId);
        void remove(const std::string& name);

        void removeAll();
        void clear();

        void push(const std::string& stateName);
        void push(uint64_t uniqueId);

        void pop();

        std::shared_ptr<State> getCurrent() const;
        std::shared_ptr<State> get(uint64_t uniqueId) const;
        std::shared_ptr<State> get(const std::string& name) const;

    private:
        std::vector<std::shared_ptr<State>> states_;
        std::stack<std::shared_ptr<State>> buffer_;
        std::shared_ptr<Script> script_;
    };
}