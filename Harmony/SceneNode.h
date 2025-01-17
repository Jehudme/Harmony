#pragma once
#include <SFML/Graphics.hpp>

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

		void draw(sf::RenderTarget& renderTarget, sf::RenderStates state) const override;
		void update(const sf::Time& time);

		void onEnter(Scene& scene);
		void onExit(Scene& scene);

		friend Scene;

	private:
		virtual void drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates state) const;
		virtual void updateCurrent(const sf::Time& time);

		virtual void onEnterCurrent(Scene& scene);
		virtual void onExitCurrent(Scene& scene);

		void updateTransform(const sf::Time& time);

	public:
		bool isDrawEnable;
		bool isUpdateEnable;

		SceneNode* parentNode;
		Scene* currentScene;

		float rotationVelocity;
		float rotationAcceleration;

		sf::Vector2f positionVelocity;
		sf::Vector2f positionAcceleration;

		std::shared_ptr<sf::Drawable> drawable;

	private:
		std::vector<std::shared_ptr<SceneNode>> m_children;
	};
}

namespace Harmony::Utilities
{
	sf::FloatRect getRect(const std::shared_ptr<Core::SceneNode> node);
	bool intersect(const std::shared_ptr<Core::SceneNode> node1, const std::shared_ptr<Core::SceneNode> node2);

}

