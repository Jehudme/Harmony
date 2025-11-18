#pragma once

#include <memory>
#include <mutex>
#include <stack>
#include <vector>
#include "Configuration.h"
#include "State.h"

namespace Harmony::Internals {
	class Engine;
}

namespace Harmony::Internals {

	/// StateStack - Thread-safe stack for managing game states
	/// Allows pushing, popping, and managing the active game state
	class StateStack {
	public:
		/// Constructor
		/// @param engine Reference to the engine
		/// @param configuration Configuration for the state stack
		StateStack(Engine& engine, const Configuration& configuration);

		/// Destructor
		~StateStack();

		// Prevent copying
		StateStack(const StateStack&) = delete;
		StateStack& operator=(const StateStack&) = delete;

		// Allow moving
		StateStack(StateStack&&) noexcept = default;
		StateStack& operator=(StateStack&&) noexcept = default;

		/// Push a new state onto the stack
		/// The previous top state will be paused
		/// @param state The state to push
		void push(std::unique_ptr<State> state);

		/// Pop the current state from the stack
		/// The new top state will be resumed
		/// @return The popped state
		std::unique_ptr<State> pop();

		/// Get the current top state (without removing it)
		/// @return Pointer to the top state, or nullptr if stack is empty
		State* peek() const;

		/// Clear all states from the stack
		void clear();

		/// Get the number of states in the stack
		std::size_t size() const;

		/// Check if the stack is empty
		bool isEmpty() const;

		/// Update the current state
		/// @param deltaTime Time elapsed since last update
		void update(float deltaTime);

		/// Render the current state
		void render();

		/// Handle events for the current state
		void handleEvents();

	private:
		mutable std::mutex mutex_;
		Engine& engine_;
		Configuration configuration_;
		std::stack<std::unique_ptr<State>> states_;
	};

} // namespace Harmony::Internals
