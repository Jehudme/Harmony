#include "pch.h"
#include "Torus.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

HARMONY_REGISTER_COMPONENT_WITH_BASE(Harmony::Components::Renderable, Harmony::Components::Torus, torus);

namespace Harmony::Components {
	Torus::Torus(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
		Renderable(scene, entityId),
		radius{50.0f},
		size{10.0f},
		radSeg{16},
		sides{16},
		color{255, 0, 0, 255},
		frameColor{128, 0, 0, 255}
	{
		HARMONY_DEBUG("Initializing Torus component for entity {}", static_cast<uint32_t>(entityId));
		
		try {
			std::optional<float> radiusOpt = configuration.get<float>({ "radius" });
			std::optional<float> sizeOpt = configuration.get<float>({ "size" });
			std::optional<int> radSegOpt = configuration.get<int>({ "rad_seg" });
			std::optional<int> sidesOpt = configuration.get<int>({ "sides" });

			std::optional<unsigned char> colorR = configuration.get<unsigned char>({ "color", "r" });
			std::optional<unsigned char> colorG = configuration.get<unsigned char>({ "color", "g" });
			std::optional<unsigned char> colorB = configuration.get<unsigned char>({ "color", "b" });
			std::optional<unsigned char> colorA = configuration.get<unsigned char>({ "color", "a" });

			std::optional<unsigned char> frameColorR = configuration.get<unsigned char>({ "frame_color", "r" });
			std::optional<unsigned char> frameColorG = configuration.get<unsigned char>({ "frame_color", "g" });
			std::optional<unsigned char> frameColorB = configuration.get<unsigned char>({ "frame_color", "b" });
			std::optional<unsigned char> frameColorA = configuration.get<unsigned char>({ "frame_color", "a" });

			float r = radiusOpt.value_or(50.0f);
			float s = sizeOpt.value_or(10.0f);
			int rs = radSegOpt.value_or(16);
			int sd = sidesOpt.value_or(16);
			
			HARMONY_ASSERT(r > 0.0f, "Torus radius must be positive");
			HARMONY_ASSERT(s > 0.0f, "Torus size must be positive");
			HARMONY_ASSERT(rs > 0, "Torus rad_seg must be positive");
			HARMONY_ASSERT(sd > 0, "Torus sides must be positive");
			
			radius = r;
			size = s;
			radSeg = rs;
			sides = sd;

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
			
			HARMONY_TRACE("Torus component initialized - Radius: {}, Size: {}, RadSeg: {}, Sides: {}", 
				radius, size, radSeg, sides);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Torus component: {}", e.what());
			throw Harmony::Exceptions::ComponentInitializationException("Torus", e.what());
		}
	}
	
	Torus::~Torus() 
	{
		HARMONY_TRACE("Torus component destroyed for entity {}", static_cast<uint32_t>(entityId_));
	}

	void Torus::onRender()
	{
		try {
			DrawTorus(Vector3{ 0.0f, 0.0f, 0.0f }, radius, size, radSeg, sides, color);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception during Torus render for entity {}: {}", 
				static_cast<uint32_t>(entityId_), e.what());
		}
	}
}
