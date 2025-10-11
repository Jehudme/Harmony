#include "pch.h"

#include "StateManagement.h"

namespace Harmony::Internals {
	
	StateManagement::StateManagement(Configuration& configuration) :
		configuration_(configuration) {

	}

	void StateManagement::push(std::uint64_t stateId) {
		if (std::shared_ptr<State> state = std::make_shared<State>(configuration_)) {
			states_.emplace(std::move(state));
		}
	}

	void StateManagement::swap(std::uint64_t stateId) {
		std::shared_ptr<State> state = std::make_shared<State>(configuration_);
		states_.emplace(std::move(state));
		pop();
	}

	void StateManagement::pop() {
		if (!states_.empty())
			states_.pop();
	}

	std::optional<std::uint64_t> StateManagement::getId(const std::string& name) const
	{
		return std::nullopt;
	}
}