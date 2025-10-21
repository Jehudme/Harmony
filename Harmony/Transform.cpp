#include "pch.h"
#include "Transform.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Logger.h"


HARMONY_REGISTER_COMPONENT(Harmony::Components::Transform, Transform)

namespace Harmony::Components
{
	Harmony::Components::Transform::Transform(const Utilities::Configuration& configuration)
	{
		if(std::optional<float> xposition = configuration.get<float>({ "position", "x" })) 
			setPosition(xposition.value(), getPosition().y);
		else HARMONY_WARN("Transform component missing position x configuration");

		if (std::optional<float> yposition = configuration.get<float>({ "position", "y" })) 
			setPosition(getPosition().x, yposition.value());
		else HARMONY_WARN("Transform component missing position y configuration");
		
		if (std::optional<float> rotation = configuration.get<float>({ "rotation" }))
			setRotation(rotation.value());
		else HARMONY_WARN("Transform component missing rotation configuration");
		
		if (std::optional<float> xscale = configuration.get<float>({ "scale", "x" }))
			setScale(xscale.value(), getScale().y);
		else HARMONY_WARN("Transform component missing scale x configuration");
		
		if (std::optional<float> yscale = configuration.get<float>({ "scale", "y" }))
			setScale(getScale().x, yscale.value());
		else HARMONY_WARN("Transform component missing scale y configuration");
		
		if(std::optional<float> originX = configuration.get<float>({ "origin", "x" }))
			setOrigin(originX.value(), getOrigin().y);
		else HARMONY_WARN("Transform component missing origin x configuration");
		
		if (std::optional<float> originY = configuration.get<float>({ "origin", "y" }))
			setOrigin(getOrigin().x, originY.value());
		else HARMONY_WARN("Transform component missing origin y configuration");
	}

	Transform::~Transform() = default;
}

