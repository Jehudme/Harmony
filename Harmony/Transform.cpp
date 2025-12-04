#include "pch.h"
#include "Transform.h"
#include "raymath.h"

namespace Harmony::Components {
	Transform::Transform(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) : 
		Utilities::Transformable()
	{
		std::optional<float> posX = configuration.get<float>({ "position", "x" });
		std::optional<float> posY = configuration.get<float>({ "position", "y" });
		std::optional<float> posZ = configuration.get<float>({ "position", "z" });

		std::optional<float> rotX = configuration.get<float>({ "rotation", "x" });
		std::optional<float> rotY = configuration.get<float>({ "rotation", "y" });
		std::optional<float> rotZ = configuration.get<float>({ "rotation", "z" });

		std::optional<float> scaleX = configuration.get<float>({ "scale", "x" });
		std::optional<float> scaleY = configuration.get<float>({ "scale", "y" });
		std::optional<float> scaleZ = configuration.get<float>({ "scale", "z" });

		setPosition(Vector3(
			posX.value_or(0.0f),
			posY.value_or(0.0f),
			posZ.value_or(0.0f)
		));

		setRotation(QuaternionFromEuler(
			rotX.value_or(0.0f) * DEG2RAD,
			rotY.value_or(0.0f) * DEG2RAD,
			rotZ.value_or(0.0f) * DEG2RAD
		));

		setScale(Vector3(
			scaleX.value_or(1.0f),
			scaleY.value_or(1.0f),
			scaleZ.value_or(1.0f)
		));
	}
}
