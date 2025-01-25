#pragma once
#include "SceneNode.h"
#include <memory>
#include "Configuration.h"
#include "Event.h"
#include "Object.h"
#include "Script.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>

namespace harmony::core
{
	class State;
	class Scene : public core::Object, public sf::Drawable
	{
	public:
		Scene(const Configuration& configuration);
		~Scene();

		void update(const sf::Time& time, EventQueue& eventQueue);
		void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
		
		void reset();

		friend State;

	private:
		void onEnter();
		void onExit();

	public:
		sf::View view;
		std::shared_ptr<SceneNode> sceneGraph;

	private:
		std::shared_ptr<DrawScript> m_drawScript;
		std::shared_ptr<UpdateScript> m_updateScript;
		std::shared_ptr<CreateScript> m_createScript;
		std::shared_ptr<DestroyScript> m_destroyScript;
		std::shared_ptr<EnterScript> m_enterScript;
		std::shared_ptr<ExitScript> m_exitScript;
	};
}

