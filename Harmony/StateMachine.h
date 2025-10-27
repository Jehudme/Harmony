#pragma once

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

namespace Harmony::Components
{
	/// @brief StateMachine component - entity state management
	class StateMachine
	{
	public:
		StateMachine(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~StateMachine();

		// State management
		void addState(const std::string& stateName);
		void removeState(const std::string& stateName);
		bool hasState(const std::string& stateName) const;

		// Transitions
		void changeState(const std::string& stateName);
		std::string getCurrentState() const;
		std::string getPreviousState() const;

		// State callbacks
		using StateCallback = std::function<void()>;
		void setOnStateEnter(const std::string& stateName, StateCallback callback);
		void setOnStateExit(const std::string& stateName, StateCallback callback);
		void setOnStateUpdate(const std::string& stateName, StateCallback callback);

		// Update current state
		void update(float deltaTime);

		// State data (optional custom data per state)
		void setStateData(const std::string& stateName, const std::string& key, const std::string& value);
		std::string getStateData(const std::string& stateName, const std::string& key) const;

	private:
		struct StateMachineImpl;
		std::unique_ptr<StateMachineImpl> impl_;
	};
}
