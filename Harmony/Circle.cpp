#include "pch.h"
#include "Configuration.h"
#include "Circle.h"
#include "ComponentManagement.h"

HARMONY_REGISTER_COMPONENT_WITH_BASE(sf::Drawable, Harmony::Components::Circle, Circle)

namespace Harmony::Components
{
	Harmony::Components::Circle::Circle(const Utilities::Configuration& configuration)
		: Harmony::Components::Shape<sf::CircleShape>(configuration)
	{
		if (std::optional<float> radius = configuration.get<float>({ "radius" })) this->setRadius(radius.value());
		else HARMONY_WARN("Circle component missing radius configuration"); this->setRadius(45.0f);

		if (std::optional<std::size_t> pointCount = configuration.get<std::size_t>({ "point_count" })) this->setPointCount(pointCount.value());
		else HARMONY_WARN("Circle component missing point count configuration"); this->setPointCount(30);
	}

	Circle::~Circle() = default;
}