#pragma once
#include "SceneNode.h"

namespace Harmony::Core
{
	class State;
	class Scene : public Core::Object, public sf::Drawable
	{
	public:
		Scene(const uint64_t& uniqueId = NULL);

		void update(const sf::Time& time);
		void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
		
		void reset();

		friend State;

	private:
		void onEnter();
		void onExit();

		virtual void onEnterCurrent();
		virtual void onExitCurrent();

	public:
		sf::View view;
		std::shared_ptr<SceneNode> sceneGraph;
	};
}

