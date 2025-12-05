#include "pch.h"
#include "Cylinder.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

HARMONY_REGISTER_COMPONENT_WITH_BASE(Harmony::Components::Renderable, Harmony::Components::Cylinder, cylinder);

namespace Harmony::Components {
	Cylinder::Cylinder(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
		Renderable(scene, entityId),
		radiusTop{25.0f},
		radiusBottom{25.0f},
		height{100.0f},
		slices{16},
		color{255, 0, 0, 255},
		frameColor{128, 0, 0, 255}
	{
		HARMONY_DEBUG("Initializing Cylinder component for entity {}", static_cast<uint32_t>(entityId));
		
		try {
			std::optional<float> radiusTopOpt = configuration.get<float>({ "radius_top" });
			std::optional<float> radiusBottomOpt = configuration.get<float>({ "radius_bottom" });
			std::optional<float> heightOpt = configuration.get<float>({ "height" });
			std::optional<int> slicesOpt = configuration.get<int>({ "slices" });

			std::optional<unsigned char> colorR = configuration.get<unsigned char>({ "color", "r" });
			std::optional<unsigned char> colorG = configuration.get<unsigned char>({ "color", "g" });
			std::optional<unsigned char> colorB = configuration.get<unsigned char>({ "color", "b" });
			std::optional<unsigned char> colorA = configuration.get<unsigned char>({ "color", "a" });

			std::optional<unsigned char> frameColorR = configuration.get<unsigned char>({ "frame_color", "r" });
			std::optional<unsigned char> frameColorG = configuration.get<unsigned char>({ "frame_color", "g" });
			std::optional<unsigned char> frameColorB = configuration.get<unsigned char>({ "frame_color", "b" });
			std::optional<unsigned char> frameColorA = configuration.get<unsigned char>({ "frame_color", "a" });

			float rt = radiusTopOpt.value_or(25.0f);
			float rb = radiusBottomOpt.value_or(25.0f);
			float h = heightOpt.value_or(100.0f);
			int slc = slicesOpt.value_or(16);
			
			HARMONY_ASSERT(rt > 0.0f, "Cylinder radius top must be positive");
			HARMONY_ASSERT(rb > 0.0f, "Cylinder radius bottom must be positive");
			HARMONY_ASSERT(h > 0.0f, "Cylinder height must be positive");
			HARMONY_ASSERT(slc > 0, "Cylinder slices must be positive");
			
			radiusTop = rt;
			radiusBottom = rb;
			height = h;
			slices = slc;

			color = Color{
				colorR.value_or(255),
				colorG.value_or(0),
				colorB.value_or(0),
				colorA.value_or(255)
			};

			frameColor = Color{
				frameColorR.value_or(128),
				frameColorG.value_or(0),
				frameColorB.value_or(0),
				frameColorA.value_or(255)
			};
			
			HARMONY_TRACE("Cylinder component initialized - RadiusTop: {}, RadiusBottom: {}, Height: {}, Slices: {}", 
				radiusTop, radiusBottom, height, slices);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Cylinder component: {}", e.what());
			throw Harmony::Exceptions::ComponentInitializationException("Cylinder", e.what());
		}
	}
	
	Cylinder::~Cylinder() 
	{
		HARMONY_TRACE("Cylinder component destroyed for entity {}", static_cast<uint32_t>(entityId_));
	}

	void Cylinder::onRender()
	{
		try {
			DrawCylinder(Vector3{ 0.0f, 0.0f, 0.0f }, radiusTop, radiusBottom, height, slices, color);
			DrawCylinderWires(Vector3{ 0.0f, 0.0f, 0.0f }, radiusTop, radiusBottom, height, slices, frameColor);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception during Cylinder render for entity {}: {}", 
				static_cast<uint32_t>(entityId_), e.what());
		}
	}
}
