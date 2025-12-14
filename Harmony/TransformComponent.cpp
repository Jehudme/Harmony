#include "pch.h"
#include "TransformComponent.h"
#include "raymath.h"
#include "ComponentsHandler.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

HARMONY_REGISTER_COMPONENT(Harmony::Components::TransformComponent, transform);

namespace Harmony::Components {
	TransformComponent::TransformComponent(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
		Utilities::Transformable()
	{
		HARMONY_DEBUG("Initializing TransformComponent component for entity {}", static_cast<uint32_t>(entityId));
		
		try {
			std::optional<float> posX = configuration.get<float>({ "position", "x" });
			std::optional<float> posY = configuration.get<float>({ "position", "y" });
			std::optional<float> posZ = configuration.get<float>({ "position", "z" });

			std::optional<float> rotX = configuration.get<float>({ "rotation", "x" });
			std::optional<float> rotY = configuration.get<float>({ "rotation", "y" });
			std::optional<float> rotZ = configuration.get<float>({ "rotation", "z" });

			std::optional<float> scaleX = configuration.get<float>({ "scale", "x" });
			std::optional<float> scaleY = configuration.get<float>({ "scale", "y" });
			std::optional<float> scaleZ = configuration.get<float>({ "scale", "z" });

			Vector3 position(
				posX.value_or(0.0f),
				posY.value_or(0.0f),
				posZ.value_or(0.0f)
			);
			setPosition(position);

			Quaternion rotation = QuaternionFromEuler(
				rotX.value_or(0.0f) * DEG2RAD,
				rotY.value_or(0.0f) * DEG2RAD,
				rotZ.value_or(0.0f) * DEG2RAD
			);
			setRotation(rotation);

			float sx = scaleX.value_or(1.0f);
			float sy = scaleY.value_or(1.0f);
			float sz = scaleZ.value_or(1.0f);
			
			HARMONY_ASSERT(sx >= 0.0f, "TransformComponent scale X must be positive");
			HARMONY_ASSERT(sy >= 0.0f, "TransformComponent scale Y must be positive");
			HARMONY_ASSERT(sz >= 0.0f, "TransformComponent scale Z must be positive");
			
			Vector3 scale(sx, sy, sz);
			setScale(scale);
			
			HARMONY_TRACE("TransformComponent component initialized - Position: ({}, {}, {}), Scale: ({}, {}, {})", 
				position.x, position.y, position.z, sx, sy, sz);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize TransformComponent component: {}", e.what());
			throw Harmony::Exceptions::ComponentInitializationException("TransformComponent", e.what());
		}
	}

	TransformComponent::~TransformComponent() 
	{
		HARMONY_TRACE("TransformComponent component destroyed");
	}
}
