#pragma once

#include <memory>
#include <mutex>
#include <string>
#include "Configuration.h"

namespace Harmony::Internals {
	class Engine;
	class StateStack;
}

namespace Harmony::Internals {

	/// State - Base class for all game states
	/// Represents a distinct state in the game (e.g., menu, gameplay, pause)
	/// Thread-safe design for concurrent access
	class State {
	public:
		/// Constructor
		/// @param engine Reference to the engine
		/// @param configuration Configuration for the state
		/// @param name Optional name for the state
		State(Engine& engine, const Configuration& configuration, const std::string& name = "UnnamedState");

		/// Virtual destructor
		virtual ~State();

		// Prevent copying
		State(const State&) = delete;
		State& operator=(const State&) = delete;

		// Allow moving
		State(State&&) noexcept = default;
		State& operator=(State&&) noexcept = default;

		/// Called when the state is entered (pushed onto the stack)
		virtual void onEnter();

		/// Called when the state is exited (popped from the stack)
		virtual void onExit();

		/// Called when the state is paused (another state is pushed on top)
		virtual void onPause();

		/// Called when the state is resumed (state on top is popped)
		virtual void onResume();

		/// Update the state
		/// @param deltaTime Time elapsed since last handleResources
		virtual void handleResources(float deltaTime);

		/// Render the state
		virtual void render();

		/// Handle events for the state
		virtual void handleEvents();

		/// Get the state name
		const std::string& getName() const;

		/// Check if the state is active
		bool isActive() const;

		/// Set the active status
		void setActive(bool active);

	protected:
		/// Get the engine reference
		Engine& getEngine();

		/// Get the configuration reference
		const Configuration& getConfiguration() const;

	private:
		mutable std::mutex mutex_;
		Engine& engine_;
		Configuration configuration_;
		std::string name_;
		bool active_;
	};

} // namespace Harmony::Internals
