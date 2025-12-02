#include "pch.h"
#include "StateStack.h"
#include "Engine.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

namespace Harmony::Internals {

    StateStack::StateStack(Engine& engine, const Configuration& configuration)
        : engine_(engine), configuration_(configuration) {
        HARMONY_INFO("Initializing StateStack");

        try {
            HARMONY_ASSERT_NOT_NULL(&engine, "Engine reference cannot be null");
            HARMONY_ASSERT_NOT_NULL(&configuration, "Configuration reference cannot be null");

            HARMONY_DEBUG("StateStack initialized successfully");
        }
        catch (const Exceptions::HarmonyException& e) {
            std::string errorMessage = e.what();
            HARMONY_CRITICAL("StateStack initialization failed with HarmonyException: {}", errorMessage);
            throw Exceptions::StateStackOperationException("initialization", errorMessage);
        }
        catch (const std::exception& e) {
            std::string errorMessage = e.what();
            HARMONY_CRITICAL("StateStack initialization failed with exception: {}", errorMessage);
            throw Exceptions::StateStackOperationException("initialization", errorMessage);
        }

        HARMONY_INFO("StateStack initialized successfully");
    }

    StateStack::~StateStack() {
        HARMONY_INFO("Destroying StateStack");
        std::lock_guard lock(mutex_);
        
        std::size_t stateCount = states_.size();
        HARMONY_DEBUG("Clearing {} states from stack", stateCount);
        
        while (!states_.empty()) {
            std::unique_ptr<State>& state = states_.top();
            if (state) {
                try {
                    state->onExit();
                }
                catch (const std::exception& e) {
                    std::string errorMessage = e.what();
                    std::string stateName = state->getName();
                    HARMONY_ERROR("Error exiting state '{}': {}", stateName, errorMessage);
                }
            }
            states_.pop();
        }
    }

    void StateStack::push(std::unique_ptr<State> state) {
        std::lock_guard lock(mutex_);

        State* statePtr = state.get();
        HARMONY_ASSERT_NOT_NULL(statePtr, "Cannot push null state onto stack");
        
        if (!state) {
            HARMONY_ERROR("Attempted to push null state onto StateStack");
            throw Exceptions::NullStateException("push");
        }

        try {
            bool hasStates = !states_.empty();
            if (hasStates) {
                State* topState = states_.top().get();
                if (topState) {
                    std::string topStateName = topState->getName();
                    HARMONY_DEBUG("Pausing current state: '{}'", topStateName);
                    topState->onPause();
                }
            }

            std::string stateName = state->getName();
            states_.push(std::move(state));
            
            std::size_t stackSize = states_.size();
            HARMONY_INFO("Pushing state '{}' onto stack (stack size: {})", stateName, stackSize);
            
            State* newTopState = states_.top().get();
            if (newTopState) {
                newTopState->onEnter();
            }
        }
        catch (const Exceptions::StateException& e) {
            std::string errorMessage = e.what();
            HARMONY_ERROR("Failed to push state onto stack: {}", errorMessage);
            throw Exceptions::StateStackOperationException("push", errorMessage);
        }
    }

    std::unique_ptr<State> StateStack::pop() {
        std::lock_guard lock(mutex_);

        bool stackEmpty = states_.empty();
        if (stackEmpty) {
            HARMONY_WARN("Attempted to pop from empty StateStack");
            throw Exceptions::EmptyStateStackException();
        }

        try {
            std::unique_ptr<State> state = std::move(states_.top());
            states_.pop();

            std::string stateName = state ? state->getName() : "null";
            std::size_t stackSize = states_.size();
            HARMONY_INFO("Popping state '{}' from stack (stack size: {})", stateName, stackSize);

            if (state) {
                state->onExit();
            }

            bool hasRemainingStates = !states_.empty();
            if (hasRemainingStates) {
                State* topState = states_.top().get();
                if (topState) {
                    std::string topStateName = topState->getName();
                    HARMONY_DEBUG("Resuming state: '{}'", topStateName);
                    topState->onResume();
                }
            }

            return state;
        }
        catch (const Exceptions::StateException& e) {
            std::string errorMessage = e.what();
            HARMONY_ERROR("Failed to pop state from stack: {}", errorMessage);
            throw Exceptions::StateStackOperationException("pop", errorMessage);
        }
    }

    State* StateStack::peek() const {
        std::lock_guard lock(mutex_);

        bool stackEmpty = states_.empty();
        if (stackEmpty) {
            HARMONY_TRACE("StateStack is empty, peek returns nullptr");
            return nullptr;
        }

        State* topState = states_.top().get();
        if (topState) {
            std::string topStateName = topState->getName();
            HARMONY_TRACE("Peeking at top state: '{}'", topStateName);
        }
        return topState;
    }

    void StateStack::clear() {
        std::lock_guard lock(mutex_);

        std::size_t stateCount = states_.size();
        HARMONY_INFO("Clearing StateStack ({} states)", stateCount);

        try {
            while (!states_.empty()) {
                std::unique_ptr<State>& state = states_.top();
                if (state) {
                    std::string stateName = state->getName();
                    HARMONY_DEBUG("Exiting and removing state: '{}'", stateName);
                    state->onExit();
                }
                states_.pop();
            }

            HARMONY_INFO("StateStack cleared successfully");
        }
        catch (const Exceptions::StateException& e) {
            std::string errorMessage = e.what();
            HARMONY_ERROR("Error during StateStack clear: {}", errorMessage);
            throw Exceptions::StateStackOperationException("clear", errorMessage);
        }
    }

    std::size_t StateStack::size() const {
        std::lock_guard lock(mutex_);
        return states_.size();
    }

    bool StateStack::isEmpty() const {
        std::lock_guard lock(mutex_);
        return states_.empty();
    }

    void StateStack::update(float deltaTime) {
        std::lock_guard lock(mutex_);

        bool hasStates = !states_.empty();
        if (hasStates) {
            State* topState = states_.top().get();
            if (topState) {
                std::string stateName = topState->getName();
                HARMONY_TRACE("Updating current state: '{}'", stateName);
                topState->update(deltaTime);
            }
        }
    }

    void StateStack::render() {
        std::lock_guard lock(mutex_);

        bool hasStates = !states_.empty();
        if (hasStates) {
            State* topState = states_.top().get();
            if (topState) {
                std::string stateName = topState->getName();
                HARMONY_TRACE("Rendering current state: '{}'", stateName);
                topState->render();
            }
        }
    }

    void StateStack::handleEvents() {
        std::lock_guard lock(mutex_);

        bool hasStates = !states_.empty();
        if (hasStates) {
            State* topState = states_.top().get();
            if (topState) {
                std::string stateName = topState->getName();
                HARMONY_TRACE("Handling events for current state: '{}'", stateName);
                topState->handleEvents();
            }
        }
    }

} // namespace Harmony::Internals
