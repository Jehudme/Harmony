#include "pch.h"
#include "Transform.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"
#include <SFML/Graphics.hpp>

HARMONY_REGISTER_COMPONENT(Harmony::Components::Transform, Transform)

namespace Harmony::Components
{
	Transform::Transform(const Utilities::Configuration& configuration, Scenes::Scene& scene)
	{
		float x = 0.0f, y = 0.0f;

		// Position
		if (std::optional<float> xposition = configuration.get<float>({ "position", "x" }))
			x = xposition.value();
		else HARMONY_WARN("Transform component missing position x configuration");

		if (std::optional<float> yposition = configuration.get<float>({ "position", "y" }))
			y = yposition.value();
		else HARMONY_WARN("Transform component missing position y configuration");

		setPosition(x, y);

		// Rotation
		if (std::optional<float> rotation = configuration.get<float>({ "rotation" }))
			setRotation(rotation.value());
		else HARMONY_WARN("Transform component missing rotation configuration");

		// Scale
		x = 1.0f; y = 1.0f;
		if (std::optional<float> xscale = configuration.get<float>({ "scale", "x" }))
			x = xscale.value();
		else HARMONY_WARN("Transform component missing scale x configuration");

		if (std::optional<float> yscale = configuration.get<float>({ "scale", "y" }))
			y = yscale.value();
		else HARMONY_WARN("Transform component missing scale y configuration");

		setScale(x, y);

		// Origin
		x = 0.0f; y = 0.0f;
		if (std::optional<float> originX = configuration.get<float>({ "origin", "x" }))
			x = originX.value();
		else HARMONY_WARN("Transform component missing origin x configuration");

		if (std::optional<float> originY = configuration.get<float>({ "origin", "y" }))
			y = originY.value();
		else HARMONY_WARN("Transform component missing origin y configuration");

		setOrigin(x, y);
	}

	Transform::~Transform() = default;
}