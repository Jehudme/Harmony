#pragma once
#include "SceneNode.h"
#include <memory>
#include "Configuration.h"
#include "Event.h"
#include "Object.h"
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

		void update(const sf::Time& time, EventQueue& eventQueue);
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

