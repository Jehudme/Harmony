#pragma once

#include <SFML/Graphics/View.hpp>

namespace Harmony::Utilities {
	class Configuration;
}

namespace Harmony::Scenes {
	class Scene;
}

namespace Harmony::Components
{
	// View now inherits from sf::View instead of composing it
	class View : public sf::View
	{
	public:
		View(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		View(const sf::View& other);
		View();
		~View();

		// Additional helper methods that wrap or extend sf::View functionality
		
		// Center helpers
		void setCenter(const float center[2]);
		void getCenter(float& x, float& y) const;

		// Size helpers
		void setSize(const float size[2]);
		void getSize(float& width, float& height) const;

		// Viewport helpers (normalized coordinates 0-1)
		void getViewport(float& left, float& top, float& width, float& height) const;

		// Note: setCenter(x, y), setSize(width, height), setRotation(), getRotation(),
		// setViewport(), zoom(), move(), and reset() are inherited from sf::View
	};
}
