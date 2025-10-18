#pragma once

namespace Harmony::Errors 
{
	class StateManagerError : public std::runtime_error 
	{
	public:
		explicit StateManagerError(const std::string& msg);
	};
}

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
		mutable std::mutex mutex_;
		std::queue<std::shared_ptr<Scenes::State>> states_;
	};
}

