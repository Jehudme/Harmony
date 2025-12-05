#include "pch.h"
#include "Hemisphere.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

HARMONY_REGISTER_COMPONENT_WITH_BASE(Harmony::Components::Renderable, Harmony::Components::Hemisphere, hemisphere);

namespace Harmony::Components {
	Hemisphere::Hemisphere(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
		Renderable(scene, entityId),
		radius{50.0f},
		rings{16},
		slices{16},
		color{255, 0, 0, 255},
		frameColor{128, 0, 0, 255}
	{
		HARMONY_DEBUG("Initializing Hemisphere component for entity {}", static_cast<uint32_t>(entityId));
		
		try {
			std::optional<float> radiusOpt = configuration.get<float>({ "radius" });
			std::optional<int> ringsOpt = configuration.get<int>({ "rings" });
			std::optional<int> slicesOpt = configuration.get<int>({ "slices" });

			std::optional<unsigned char> colorR = configuration.get<unsigned char>({ "color", "r" });
			std::optional<unsigned char> colorG = configuration.get<unsigned char>({ "color", "g" });
			std::optional<unsigned char> colorB = configuration.get<unsigned char>({ "color", "b" });
			std::optional<unsigned char> colorA = configuration.get<unsigned char>({ "color", "a" });

			std::optional<unsigned char> frameColorR = configuration.get<unsigned char>({ "frame_color", "r" });
			std::optional<unsigned char> frameColorG = configuration.get<unsigned char>({ "frame_color", "g" });
			std::optional<unsigned char> frameColorB = configuration.get<unsigned char>({ "frame_color", "b" });
			std::optional<unsigned char> frameColorA = configuration.get<unsigned char>({ "frame_color", "a" });

			float r = radiusOpt.value_or(50.0f);
			int rng = ringsOpt.value_or(16);
			int slc = slicesOpt.value_or(16);
			
			HARMONY_ASSERT(r > 0.0f, "Hemisphere radius must be positive");
			HARMONY_ASSERT(rng > 0, "Hemisphere rings must be positive");
			HARMONY_ASSERT(slc > 0, "Hemisphere slices must be positive");
			
			radius = r;
			rings = rng;
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
			
			HARMONY_TRACE("Hemisphere component initialized - Radius: {}, Rings: {}, Slices: {}", radius, rings, slices);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Hemisphere component: {}", e.what());
			throw Harmony::Exceptions::ComponentInitializationException("Hemisphere", e.what());
		}
	}
	
	Hemisphere::~Hemisphere() 
	{
		HARMONY_TRACE("Hemisphere component destroyed for entity {}", static_cast<uint32_t>(entityId_));
	}

	void Hemisphere::onRender()
	{
		try {
			// Draw hemisphere using sphere with clipped geometry
			// For simplicity, using cylinder + sphere cap or custom mesh approach
			DrawSphere(Vector3{ 0.0f, 0.0f, 0.0f }, radius, color);
			DrawSphereWires(Vector3{ 0.0f, 0.0f, 0.0f }, radius, rings, slices, frameColor);
			// Note: This draws a full sphere. A true hemisphere would require custom mesh generation
			// or using DrawMesh with a hemisphere model. For now, keeping it simple.
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception during Hemisphere render for entity {}: {}", 
				static_cast<uint32_t>(entityId_), e.what());
		}
	}
}
