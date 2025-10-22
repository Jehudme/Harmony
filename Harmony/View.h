#pragma once

#include <memory>

namespace Harmony::Utilities {
	class Configuration;
}

namespace Harmony::Scenes {
	class Scene;
}

namespace Harmony::Components
{
	/// @brief View component - camera/viewport for rendering
	/// Uses composition to hide SFML implementation details
	/// Registered as a singleton per scene
	class View
	{
	public:
		View(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~View();

		// Center
		void setCenter(float x, float y);
		void setCenter(const float center[2]);
		void getCenter(float& x, float& y) const;

		// Size
		void setSize(float width, float height);
		void setSize(const float size[2]);
		void getSize(float& width, float& height) const;

		// Rotation
		void setRotation(float angle);
		float getRotation() const;

		// Viewport (normalized coordinates 0-1)
		void setViewport(float left, float top, float width, float height);
		void getViewport(float& left, float& top, float& width, float& height) const;

		// Zoom
		void zoom(float factor);

		// Move
		void move(float offsetX, float offsetY);

		// Reset to default
		void reset(float centerX, float centerY, float width, float height);

		// Internal access for rendering - returns void* to sf::View
		void* getInternalView();
		const void* getInternalView() const;

	private:
		// PImpl to hide sf::View
		struct ViewImpl;
		std::unique_ptr<ViewImpl> impl_;
	};
}
