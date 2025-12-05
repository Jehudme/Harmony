#include "pch.h"
#include "View3D.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

HARMONY_REGISTER_COMPONENT(Harmony::Components::View3D, view3d)

namespace Harmony::Components {
	Harmony::Components::View3D::View3D(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
		Camera3D()
	{
		HARMONY_DEBUG("Initializing View3D component for entity {}", static_cast<uint32_t>(entityId));
		
		try {
			std::optional<float> posX = configuration.get<float>({ "position", "x" });
			std::optional<float> posY = configuration.get<float>({ "position", "y" });
			std::optional<float> posZ = configuration.get<float>({ "position", "z" });

			std::optional<float> targetX = configuration.get<float>({ "target", "x" });
			std::optional<float> targetY = configuration.get<float>({ "target", "y" });
			std::optional<float> targetZ = configuration.get<float>({ "target", "z" });

			std::optional<float> upX = configuration.get<float>({ "up", "x" });
			std::optional<float> upY = configuration.get<float>({ "up", "y" });
			std::optional<float> upZ = configuration.get<float>({ "up", "z" });

			std::optional<float> fovyOpt = configuration.get<float>({ "fovy" });
			std::optional<std::string> projectionOpt = configuration.get<std::string>({ "projection" });

			Camera3D::position = {
				posX.value_or(0.0f),
				posY.value_or(0.0f),
				posZ.value_or(10.0f)
			};

			Camera3D::target = {
				targetX.value_or(0.0f),
				targetY.value_or(0.0f),
				targetZ.value_or(0.0f)
			};

			Camera3D::up = {
				upX.value_or(0.0f),
				upY.value_or(1.0f),
				upZ.value_or(0.0f)
			};

			float fovy = fovyOpt.value_or(45.0f);
			HARMONY_ASSERT_RANGE(fovy, 1.0f, 179.0f, "FOV must be between 1 and 179 degrees");
			Camera3D::fovy = fovy;

			std::string projection = projectionOpt.value_or("perspective");
			if (projection == "perspective") {
				Camera3D::projection = CAMERA_PERSPECTIVE;
			}
			else if (projection == "orthographic" || projection == "orthogonal") {
				Camera3D::projection = CAMERA_ORTHOGRAPHIC;
			}
			else {
				HARMONY_WARN("Unknown projection type '{}', defaulting to perspective", projection);
				Camera3D::projection = CAMERA_PERSPECTIVE;
			}
			
			HARMONY_TRACE("View3D component initialized - Position: ({}, {}, {}), Target: ({}, {}, {}), FOV: {}", 
				Camera3D::position.x, Camera3D::position.y, Camera3D::position.z,
				Camera3D::target.x, Camera3D::target.y, Camera3D::target.z,
				Camera3D::fovy);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize View3D component: {}", e.what());
			throw Harmony::Exceptions::ComponentInitializationException("View3D", e.what());
		}
	}
	
	View3D::~View3D()
	{
		HARMONY_TRACE("View3D component destroyed");
	}
}
