#include "pch.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Rectangle.h"

HARMONY_REGISTER_COMPONENT_FULL(sf::Drawable, Harmony::Components::Rectangle, Rectangle)

namespace Harmony::Components
{
	Harmony::Components::Rectangle::Rectangle(const Utilities::Configuration& configuration)
		: Harmony::Components::Shape<sf::RectangleShape>(configuration)
	{
		std::optional<float> width;
		if (width = configuration.get<float>({ "size", "width" }); !width.has_value())
			HARMONY_WARN("Rectangle component missing width configuration");

		std::optional<float> height;
		if (height = configuration.get<float>({ "size", "height" }); !height.has_value())
			HARMONY_WARN("Rectangle component missing height configuration");

		this->setSize({ width.value_or(90), height.value_or(90)});
	}

	Harmony::Components::Rectangle::~Rectangle() = default;
}
