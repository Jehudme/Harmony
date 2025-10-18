#pragma once

namespace Harmony::Scenes
{
	class State : public sf::Drawable, public std::enable_shared_from_this<State>  
	{
	public:
		State(const Utilities::Configuration& configuration, Engine& engine);
		~State();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime);
	
	public:
		Engine& engine;

	private:
		std::map<Utilities::UUID, std::shared_ptr<Scene>> scenes_;
	};
}
