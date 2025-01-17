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
		
		friend State;

	private:
		virtual void onEnter();
		virtual void onExit();

	public:
		sf::View view;

	private:
		std::shared_ptr<SceneNode> m_sceneGraph;
	};
}

