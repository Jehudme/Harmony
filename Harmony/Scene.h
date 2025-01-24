#pragma once
#include "SceneNode.h"

namespace harmony::core
{
	class State;
	class Scene : public core::Object, public sf::Drawable
	{
	public:
		Scene(const uint64_t& uniqueId = NULL);

		void update(const sf::Time& time, EventPool& eventPool);
		void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
		
		void reset();

		friend State;

	private:
		void onEnter();
		void onExit();

		virtual void onCreate();
		virtual void onDestroy();

	public:
		sf::View view;
		std::shared_ptr<SceneNode> sceneGraph;
	};
}

