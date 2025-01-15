#pragma once
#include "SceneNode.h"

namespace Harmony::Core
{
	class Scene : public Object, public sf::Drawable
	{
	public:
		Scene(const uint64_t& uniqueId = Utilities::Random::generateId());

		virtual void update(const sf::Time time);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		std::shared_ptr<SceneNode> m_rootNode;
		sf::View m_view;
	};
}

