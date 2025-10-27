#include "pch.h"
#include "Camera.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"
#include <SFML/Graphics.hpp>

HARMONY_REGISTER_COMPONENT(Harmony::Components::Camera, Camera)

namespace Harmony::Components
{
	// PImpl to hide sf::View
	struct Camera::CameraImpl {
		sf::View view;
		sf::Vector2f defaultCenter;
		sf::Vector2f defaultSize;
		float zoomLevel = 1.0f;
		bool active = false;
	};

	Camera::Camera(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: impl_(std::make_unique<CameraImpl>())
	{
		try {
			// Center
			float centerX = configuration.get<float>({ "center", "x" }).value_or(0.0f);
			float centerY = configuration.get<float>({ "center", "y" }).value_or(0.0f);
			impl_->view.setCenter(centerX, centerY);
			impl_->defaultCenter = sf::Vector2f(centerX, centerY);

			// Size
			float width = configuration.get<float>({ "size", "width" }).value_or(800.0f);
			float height = configuration.get<float>({ "size", "height" }).value_or(600.0f);
			impl_->view.setSize(width, height);
			impl_->defaultSize = sf::Vector2f(width, height);

			// Rotation
			float rotation = configuration.get<float>({ "rotation" }).value_or(0.0f);
			impl_->view.setRotation(rotation);

			// Viewport
			float vpLeft = configuration.get<float>({ "viewport", "left" }).value_or(0.0f);
			float vpTop = configuration.get<float>({ "viewport", "top" }).value_or(0.0f);
			float vpWidth = configuration.get<float>({ "viewport", "width" }).value_or(1.0f);
			float vpHeight = configuration.get<float>({ "viewport", "height" }).value_or(1.0f);
			impl_->view.setViewport(sf::FloatRect(vpLeft, vpTop, vpWidth, vpHeight));

			// Zoom
			impl_->zoomLevel = configuration.get<float>({ "zoom" }).value_or(1.0f);
			if (impl_->zoomLevel != 1.0f) {
				impl_->view.zoom(impl_->zoomLevel);
			}

			// Active state
			impl_->active = configuration.get<bool>({ "active" }).value_or(false);

			HARMONY_DEBUG("Camera component initialized at ({}, {})", centerX, centerY);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Camera component: {}", e.what());
			throw;
		}
	}

	Camera::~Camera() = default;

	void Camera::setCenter(float x, float y) {
		impl_->view.setCenter(x, y);
	}

	void Camera::getCenter(float& x, float& y) const {
		const auto& center = impl_->view.getCenter();
		x = center.x;
		y = center.y;
	}

	void Camera::move(float offsetX, float offsetY) {
		impl_->view.move(offsetX, offsetY);
	}

	void Camera::setSize(float width, float height) {
		impl_->view.setSize(width, height);
	}

	void Camera::getSize(float& width, float& height) const {
		const auto& size = impl_->view.getSize();
		width = size.x;
		height = size.y;
	}

	void Camera::setRotation(float angle) {
		impl_->view.setRotation(angle);
	}

	float Camera::getRotation() const {
		return impl_->view.getRotation();
	}

	void Camera::rotate(float angle) {
		impl_->view.rotate(angle);
	}

	void Camera::zoom(float factor) {
		impl_->view.zoom(factor);
		impl_->zoomLevel *= factor;
	}

	void Camera::setZoom(float zoomLevel) {
		// Reset to default size then apply zoom
		impl_->view.setSize(impl_->defaultSize);
		impl_->view.zoom(zoomLevel);
		impl_->zoomLevel = zoomLevel;
	}

	float Camera::getZoom() const {
		return impl_->zoomLevel;
	}

	void Camera::setViewport(float left, float top, float width, float height) {
		impl_->view.setViewport(sf::FloatRect(left, top, width, height));
	}

	void Camera::getViewport(float& left, float& top, float& width, float& height) const {
		const auto& viewport = impl_->view.getViewport();
		left = viewport.left;
		top = viewport.top;
		width = viewport.width;
		height = viewport.height;
	}

	void Camera::reset() {
		impl_->view.setCenter(impl_->defaultCenter);
		impl_->view.setSize(impl_->defaultSize);
		impl_->view.setRotation(0.0f);
		impl_->zoomLevel = 1.0f;
	}

	void Camera::setActive(bool active) {
		impl_->active = active;
		HARMONY_DEBUG("Camera active state set to {}", active);
	}

	bool Camera::isActive() const {
		return impl_->active;
	}

	void* Camera::getInternalView() {
		return &impl_->view;
	}

	const void* Camera::getInternalView() const {
		return &impl_->view;
	}
}
