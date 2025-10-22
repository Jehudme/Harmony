#include "pch.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Rectangle.h"

HARMONY_REGISTER_COMPONENT_WITH_BASE(sf::Drawable, Harmony::Components::Rectangle, Rectangle)

namespace Harmony::Components
{
	Rectangle::Rectangle(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: Shape<sf::RectangleShape>(configuration, scene)
	{
		std::optional<float> width = configuration.get<float>({ "size", "width" });
		std::optional<float> height = configuration.get<float>({ "size", "height" });

		if (!width.has_value())  HARMONY_WARN("Rectangle component missing width configuration");
		if (!height.has_value()) HARMONY_WARN("Rectangle component missing height configuration");		

		this->setSize({ width.value_or(90), height.value_or(90)});
	}

	Rectangle::~Rectangle() = default;
}
