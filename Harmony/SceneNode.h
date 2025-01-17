#pragma once
#include <SFML/Graphics.hpp>

#include "Event.h"
#include "Object.h"

namespace Harmony::Core
{
	class Scene;

	class SceneNode : public sf::Transformable, public sf::Drawable, public Object
	{
	public:
		SceneNode(const uint64_t& uniqueId = NULL);
		~SceneNode();

		void attachChild(const std::shared_ptr<SceneNode> child);
		std::shared_ptr<SceneNode> detachChild(const SceneNode& child);

		void enableDraw(const bool option);
		void enableUpdate(const bool option);

		sf::Vector2f getGlobalPosition();
		std::shared_ptr<SceneNode> getRootNode();

	public:
		void draw(sf::RenderTarget& renderTarget, sf::RenderStates state) const override;
		void update(const sf::Time& time);

	private:
		virtual void drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates state) const;
		virtual void updateCurrent(const sf::Time& time);

	private:
		bool isDrawEnable;
		bool isUpdateEnable;

		SceneNode* m_parent;
		Scene* m_scene;

		std::vector<std::shared_ptr<SceneNode>> m_children;
	};
}

