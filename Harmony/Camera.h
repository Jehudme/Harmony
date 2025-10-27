#pragma once

#include <memory>

namespace Harmony::Components
{
	/// @brief Camera component - view control and viewport management
	/// Uses composition to hide SFML implementation details
	class Camera
	{
	public:
		Camera(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~Camera();

		// Position
		void setCenter(float x, float y);
		void getCenter(float& x, float& y) const;
		void move(float offsetX, float offsetY);

		// Size
		void setSize(float width, float height);
		void getSize(float& width, float& height) const;

		// Rotation
		void setRotation(float angle);
		float getRotation() const;
		void rotate(float angle);

		// Zoom
		void zoom(float factor);
		void setZoom(float zoomLevel);
		float getZoom() const;

		// Viewport (portion of window to render to)
		void setViewport(float left, float top, float width, float height);
		void getViewport(float& left, float& top, float& width, float& height) const;

		// Reset to default
		void reset();

		// Active state
		void setActive(bool active);
		bool isActive() const;

		// Internal access for rendering
		void* getInternalView();
		const void* getInternalView() const;

	private:
		struct CameraImpl;
		std::unique_ptr<CameraImpl> impl_;
	};
}
