#include "pch.h"
#include "Rectangle.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Logger.h"

HARMONY_REGISTER_COMPONENT_FULL(Harmony::Components::Drawable, Harmony::Components::Rectangle, Rectangle)

namespace Harmony::Components
{
	Harmony::Components::Rectangle::Rectangle(const Utilities::Configuration& configuration)
	{
		std::optional<float> width;
		if (width = configuration.get<float>({ "size", "width" }); !width.has_value())
			HARMONY_WARN("Rectangle component missing width configuration");

		std::optional<float> height;
		if (height = configuration.get<float>({ "size", "height" }); !height.has_value())
			HARMONY_WARN("Rectangle component missing height configuration");

		std::optional<unsigned int> fillR;
		if (fillR = configuration.get<unsigned int>({ "fill_color", "r" }); !fillR.has_value())
			HARMONY_WARN("Rectangle component missing fill color R configuration");
		std::optional<unsigned int> fillG;
		if (fillG = configuration.get<unsigned int>({ "fill_color", "g" }); !fillG.has_value())
			HARMONY_WARN("Rectangle component missing fill color G configuration");
		std::optional<unsigned int> fillB;
		if (fillB = configuration.get<unsigned int>({ "fill_color", "b" }); !fillB.has_value())
			HARMONY_WARN("Rectangle component missing fill color B configuration");
		std::optional<unsigned int> fillA;
		if (fillA = configuration.get<unsigned int>({ "fill_color", "a" }); !fillA.has_value())
			HARMONY_WARN("Rectangle component missing fill color A configuration");
		

		if (std::optional<unsigned int> outlineThickness = configuration.get<unsigned int>({ "outline_thickness" }))
			shape_.setOutlineThickness(static_cast<float>(outlineThickness.value()));
		else HARMONY_WARN("Rectangle component missing outline thickness configuration");

		std::optional<unsigned int> outlineR;
		if (outlineR = configuration.get<unsigned int>({ "outline_color", "r" }); !outlineR.has_value())
			HARMONY_WARN("Rectangle component missing outline color R configuration");
		std::optional<unsigned int> outlineG;
		if (outlineG = configuration.get<unsigned int>({ "outline_color", "g" }); !outlineG.has_value())
			HARMONY_WARN("Rectangle component missing outline color G configuration");
		std::optional<unsigned int> outlineB;
		if (outlineB = configuration.get<unsigned int>({ "outline_color", "b" }); !outlineB.has_value())
			HARMONY_WARN("Rectangle component missing outline color B configuration");
		std::optional<unsigned int> outlineA;
		if (outlineA = configuration.get<unsigned int>({ "outline_color", "a" }); !outlineA.has_value())
			HARMONY_WARN("Rectangle component missing outline color A configuration");
		
		shape_.setFillColor(sf::Color(
			fillA.value_or(255),
			fillR.value_or(255),
			fillG.value_or(255),
			fillB.value_or(255)));

		shape_.setOutlineColor(sf::Color(
			outlineA.value_or(255),
			outlineR.value_or(255),
			outlineG.value_or(255),
			outlineB.value_or(255)));

		shape_.setSize({ width.value_or(90), height.value_or(90)});

	}

	Harmony::Components::Rectangle::~Rectangle()
	{
	}

	void Rectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(shape_, states);
	}
}
