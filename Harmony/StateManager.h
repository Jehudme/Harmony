#pragma once

#include "State.h"
#include <memory>
#include <stack>

namespace Harmony::Core {

    class StateManager {
    public:
        // Constructor and Destructor
        StateManager();
        ~StateManager();

        // Push a new state onto the stack
        void pushState(std::shared_ptr<State> state);

        // Pop the top state from the stack
        void popState();

        // Get the current active state
        std::shared_ptr<State> getCurrentState() const;

        // Update the current active state
        void update(const sf::Time& time);

        // Draw the current active state
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        // Check if there are any states in the stack
        bool hasStates() const;

    private:
        // Stack to store the states
        std::stack<std::shared_ptr<State>> m_stateStack;
    };

}
