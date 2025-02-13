#pragma once
#include "Object.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace Harmony
{
	class TaskQueue;
	class Scene;
	class Script;

	class SceneNode : public Object, public sf::Transformable, public sf::Drawable
	{
	public:
		SceneNode(std::shared_ptr<Configuration> configuration, const bool enableCreateChildren = false);
		~SceneNode();

		void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
		void update(const sf::Time& time, TaskQueue& taskQueue);

		void attachChild(std::shared_ptr<SceneNode> sceneNode);
		void detachChild(std::shared_ptr<SceneNode> sceneNode);

		sf::Vector2f getGlobalPosition() const;

		std::vector<std::shared_ptr<SceneNode>> findChildrenByName(std::initializer_list<std::string> names);
		std::vector<std::shared_ptr<SceneNode>> findChildrenByName(std::initializer_list<uint64_t> uniqueIds);

		virtual void initialize(std::shared_ptr<Configuration> configuration);

	private:
		virtual void drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const;
		virtual void updateCurrrent(const sf::Time& time, TaskQueue& taskQueue);

		void updateTransform(const sf::Time& time, TaskQueue& taskQueue);

	public:
		std::vector<std::shared_ptr<SceneNode>> children;

		sf::Vector2f position_velocity;
		sf::Vector2f position_acceleration;

		float rotation_velocity = 0;
		float rotation_acceleration = 0;
		
	public:
		SceneNode* parent;
		Scene* scene;

		std::shared_ptr<Script> script;
		std::shared_ptr<Configuration> configuration;
	};

	template<>
	std::shared_ptr<SceneNode> create<SceneNode, std::shared_ptr<Configuration>>(std::shared_ptr<Configuration>&& configuration);
}

