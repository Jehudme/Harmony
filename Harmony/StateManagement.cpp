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
		const auto keys = configuration_.extractKeys({});
		
		for (const std::string key : keys) {
			const std::optional<std::string> name = configuration_.get<std::string>({ key, "name" });
			if(name.has_value() && name.value() == name)
				return  std::optional<std::uint64_t>(std::stoull(key));
		}

		return std::nullopt;

	}
}