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
	View::View(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: sf::View()  // Initialize base class
	{
		// Center
		float centerX = 0.0f, centerY = 0.0f;
		if (std::optional<float> cx = configuration.get<float>({ "center", "x" }))
			centerX = cx.value();
		else HARMONY_WARN("View component missing center x configuration");

		if (std::optional<float> cy = configuration.get<float>({ "center", "y" }))
			centerY = cy.value();
		else HARMONY_WARN("View component missing center y configuration");

		sf::View::setCenter(centerX, centerY);

		// Size
		float width = 800.0f, height = 600.0f;
		if (std::optional<float> w = configuration.get<float>({ "size", "width" }))
			width = w.value();
		else HARMONY_WARN("View component missing size width configuration");

		if (std::optional<float> h = configuration.get<float>({ "size", "height" }))
			height = h.value();
		else HARMONY_WARN("View component missing size height configuration");

		sf::View::setSize(width, height);

		// Rotation (optional)
		if (std::optional<float> rotation = configuration.get<float>({ "rotation" }))
			sf::View::setRotation(rotation.value());

		// Viewport (optional, normalized coordinates 0-1)
		if (std::optional<float> left = configuration.get<float>({ "viewport", "left" })) {
			float top = configuration.get<float>({ "viewport", "top" }).value_or(0.0f);
			float vpWidth = configuration.get<float>({ "viewport", "width" }).value_or(1.0f);
			float vpHeight = configuration.get<float>({ "viewport", "height" }).value_or(1.0f);
			sf::View::setViewport(sf::FloatRect(left.value(), top, vpWidth, vpHeight));
		}

		HARMONY_DEBUG("View component created with center ({}, {}), size ({}, {})", 
			centerX, centerY, width, height);
	}

	View::View(const sf::View& other)
		: sf::View(other) {}



	View::View() = default;

	View::~View() = default;

	void View::setCenter(const float center[2]) {
		sf::View::setCenter(center[0], center[1]);
	}

	void View::getCenter(float& x, float& y) const {
		const auto& center = sf::View::getCenter();
		x = center.x;
		y = center.y;
	}

	void View::setSize(const float size[2]) {
		sf::View::setSize(size[0], size[1]);
	}

	void View::getSize(float& width, float& height) const {
		const auto& size = sf::View::getSize();
		width = size.x;
		height = size.y;
	}

	void View::getViewport(float& left, float& top, float& width, float& height) const {
		const auto& viewport = sf::View::getViewport();
		left = viewport.left;
		top = viewport.top;
		width = viewport.width;
		height = viewport.height;
	}
}
