#include "pch.h"
#include "View.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"
#include <SFML/Graphics.hpp>

HARMONY_REGISTER_COMPONENT(Harmony::Components::View, View)

namespace Harmony::Components
{
	// PImpl to hide sf::View
	struct View::ViewImpl {
		sf::View view;
	};

	View::View(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: impl_(std::make_unique<ViewImpl>())
	{
		// Center
		float centerX = 0.0f, centerY = 0.0f;
		if (std::optional<float> cx = configuration.get<float>({ "center", "x" }))
			centerX = cx.value();
		else HARMONY_WARN("View component missing center x configuration");

		if (std::optional<float> cy = configuration.get<float>({ "center", "y" }))
			centerY = cy.value();
		else HARMONY_WARN("View component missing center y configuration");

		impl_->view.setCenter(centerX, centerY);

		// Size
		float width = 800.0f, height = 600.0f;
		if (std::optional<float> w = configuration.get<float>({ "size", "width" }))
			width = w.value();
		else HARMONY_WARN("View component missing size width configuration");

		if (std::optional<float> h = configuration.get<float>({ "size", "height" }))
			height = h.value();
		else HARMONY_WARN("View component missing size height configuration");

		impl_->view.setSize(width, height);

		// Rotation (optional)
		if (std::optional<float> rotation = configuration.get<float>({ "rotation" }))
			impl_->view.setRotation(rotation.value());

		// Viewport (optional, normalized coordinates 0-1)
		if (std::optional<float> left = configuration.get<float>({ "viewport", "left" })) {
			float top = configuration.get<float>({ "viewport", "top" }).value_or(0.0f);
			float vpWidth = configuration.get<float>({ "viewport", "width" }).value_or(1.0f);
			float vpHeight = configuration.get<float>({ "viewport", "height" }).value_or(1.0f);
			impl_->view.setViewport(sf::FloatRect(left.value(), top, vpWidth, vpHeight));
		}

		HARMONY_DEBUG("View component created with center ({}, {}), size ({}, {})", 
			centerX, centerY, width, height);
	}

	View::~View() = default;

	void View::setCenter(float x, float y) {
		impl_->view.setCenter(x, y);
	}

	void View::setCenter(const float center[2]) {
		impl_->view.setCenter(center[0], center[1]);
	}

	void View::getCenter(float& x, float& y) const {
		const auto& center = impl_->view.getCenter();
		x = center.x;
		y = center.y;
	}

	void View::setSize(float width, float height) {
		impl_->view.setSize(width, height);
	}

	void View::setSize(const float size[2]) {
		impl_->view.setSize(size[0], size[1]);
	}

	void View::getSize(float& width, float& height) const {
		const auto& size = impl_->view.getSize();
		width = size.x;
		height = size.y;
	}

	void View::setRotation(float angle) {
		impl_->view.setRotation(angle);
	}

	float View::getRotation() const {
		return impl_->view.getRotation();
	}

	void View::setViewport(float left, float top, float width, float height) {
		impl_->view.setViewport(sf::FloatRect(left, top, width, height));
	}

	void View::getViewport(float& left, float& top, float& width, float& height) const {
		const auto& viewport = impl_->view.getViewport();
		left = viewport.left;
		top = viewport.top;
		width = viewport.width;
		height = viewport.height;
	}

	void View::zoom(float factor) {
		impl_->view.zoom(factor);
	}

	void View::move(float offsetX, float offsetY) {
		impl_->view.move(offsetX, offsetY);
	}

	void View::reset(float centerX, float centerY, float width, float height) {
		impl_->view.reset(sf::FloatRect(centerX - width / 2, centerY - height / 2, width, height));
	}

	void* View::getInternalView() {
		return &impl_->view;
	}

	const void* View::getInternalView() const {
		return &impl_->view;
	}
}
