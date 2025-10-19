#include "pch.h"
#include "Transform.h"
#include "Configuration.h"
#include "ComponentManagement.h"


HARMONY_REGISTER_COMPONENT(Harmony::Components::Transform, Transform)

namespace Harmony::Components
{
	Harmony::Components::Transform::Transform(const Utilities::Configuration& configuration)
	{
		if(std::optional<float> xposition = configuration.get<float>({ "position", "x" }))
			setPosition(xposition.value(), getPosition().y);

		if (std::optional<float> yposition = configuration.get<float>({ "position", "y" }))
			setPosition(getPosition().x, yposition.value());

		if (std::optional<float> rotation = configuration.get<float>({ "rotation" }))
			setRotation(rotation.value());

		if (std::optional<float> xscale = configuration.get<float>({ "scale", "x" }))
			setScale(xscale.value(), getScale().y);

		if (std::optional<float> yscale = configuration.get<float>({ "scale", "y" }))
			setScale(getScale().x, yscale.value());

		if(std::optional<float> originX = configuration.get<float>({ "origin", "x" }))
			setOrigin(originX.value(), getOrigin().y);

		if (std::optional<float> originY = configuration.get<float>({ "origin", "y" }))
			setOrigin(getOrigin().x, originY.value());
	}

	Transform::~Transform() = default;
}

