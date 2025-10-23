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
	class View : public sf::View
	{
	public:
		View(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~View();

		// Overload setCenter to provide array interface
		void setCenter(const float center[2]);
		void getCenter(float& x, float& y) const;

		// Overload setSize to provide array interface
		void setSize(const float size[2]);
		void getSize(float& width, float& height) const;

		// Provide getRotation for consistency
		float getRotation() const;

		// Overload setViewport to provide individual parameters
		void setViewport(float left, float top, float width, float height);
		void getViewport(float& left, float& top, float& width, float& height) const;

		// Reset to default
		void reset(float centerX, float centerY, float width, float height);

		// Internal access for rendering - returns pointer to sf::View (this)
		void* getInternalView();
		const void* getInternalView() const;
	};
}
