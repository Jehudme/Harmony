#pragma once


namespace Harmony::Management
{
	class StateManager : public sf::Drawable 
	{
	public:
		friend class Engine;
		StateManager(Engine& engine);
		~StateManager();

		void  push(std::uint64_t stateId);
		void  pop();
	
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime);

	private:
		Engine& engine;
		mutable std::shared_mutex mutex_;
		std::queue<std::shared_ptr<Scenes::State>> states_;
	};
}

namespace Harmony::Exceptions 
{
	struct StateStackEmptyError : std::runtime_error {
		explicit StateStackEmptyError();
	};

	struct StartupStatesNotDefined : std::runtime_error {
		explicit StartupStatesNotDefined();
	};

	struct StateConfigurationNotFound : std::runtime_error {
		explicit StateConfigurationNotFound(const std::string& stateKey);
	};

	struct StateStackPushFailed : std::runtime_error { 
		explicit StateStackPushFailed(const std::string& reason);
	};
}
