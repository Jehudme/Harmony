#include "pch.h"
#include "Capsule.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

HARMONY_REGISTER_COMPONENT_WITH_BASE(Harmony::Components::Renderable, Harmony::Components::Capsule, capsule);

namespace Harmony::Components {
	Capsule::Capsule(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
		Renderable(scene, entityId),
		radius{25.0f},
		height{100.0f},
		slices{16},
		rings{8},
		color{255, 0, 0, 255},
		frameColor{128, 0, 0, 255}
	{
		HARMONY_DEBUG("Initializing Capsule component for entity {}", static_cast<uint32_t>(entityId));
		
		try {
			std::optional<float> radiusOpt = configuration.get<float>({ "radius" });
			std::optional<float> heightOpt = configuration.get<float>({ "height" });
			std::optional<int> slicesOpt = configuration.get<int>({ "slices" });
			std::optional<int> ringsOpt = configuration.get<int>({ "rings" });

			std::optional<unsigned char> colorR = configuration.get<unsigned char>({ "color", "r" });
			std::optional<unsigned char> colorG = configuration.get<unsigned char>({ "color", "g" });
			std::optional<unsigned char> colorB = configuration.get<unsigned char>({ "color", "b" });
			std::optional<unsigned char> colorA = configuration.get<unsigned char>({ "color", "a" });

			std::optional<unsigned char> frameColorR = configuration.get<unsigned char>({ "frame_color", "r" });
			std::optional<unsigned char> frameColorG = configuration.get<unsigned char>({ "frame_color", "g" });
			std::optional<unsigned char> frameColorB = configuration.get<unsigned char>({ "frame_color", "b" });
			std::optional<unsigned char> frameColorA = configuration.get<unsigned char>({ "frame_color", "a" });

			float r = radiusOpt.value_or(25.0f);
			float h = heightOpt.value_or(100.0f);
			int slc = slicesOpt.value_or(16);
			int rng = ringsOpt.value_or(8);
			
			HARMONY_ASSERT(r > 0.0f, "Capsule radius must be positive");
			HARMONY_ASSERT(h > 0.0f, "Capsule height must be positive");
			HARMONY_ASSERT(slc > 0, "Capsule slices must be positive");
			HARMONY_ASSERT(rng > 0, "Capsule rings must be positive");
			
			radius = r;
			height = h;
			slices = slc;
			rings = rng;

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
			
			HARMONY_TRACE("Capsule component initialized - Radius: {}, Height: {}, Slices: {}, Rings: {}", 
				radius, height, slices, rings);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Capsule component: {}", e.what());
			throw Harmony::Exceptions::ComponentInitializationException("Capsule", e.what());
		}
	}
	
	Capsule::~Capsule() 
	{
		HARMONY_TRACE("Capsule component destroyed for entity {}", static_cast<uint32_t>(entityId_));
	}

	void Capsule::onRender()
	{
		try {
			// Capsule = cylinder with hemisphere caps
			Vector3 startPos = Vector3{ 0.0f, -height / 2.0f, 0.0f };
			Vector3 endPos = Vector3{ 0.0f, height / 2.0f, 0.0f };
			DrawCapsule(startPos, endPos, radius, slices, rings, color);
			DrawCapsuleWires(startPos, endPos, radius, slices, rings, frameColor);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception during Capsule render for entity {}: {}", 
				static_cast<uint32_t>(entityId_), e.what());
		}
	}
}
