#include "pch.h"
#include "Pyramid.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

HARMONY_REGISTER_COMPONENT_WITH_BASE(Harmony::Components::Renderable, Harmony::Components::Pyramid, pyramid);

namespace Harmony::Components {
	Pyramid::Pyramid(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
		Renderable(scene, entityId),
		size{100.0f, 100.0f, 100.0f},
		color{255, 0, 0, 255},
		frameColor{128, 0, 0, 255}
	{
		HARMONY_DEBUG("Initializing Pyramid component for entity {}", static_cast<uint32_t>(entityId));
		
		try {
			std::optional<float> sizeX = configuration.get<float>({ "size", "x" });
			std::optional<float> sizeY = configuration.get<float>({ "size", "y" });
			std::optional<float> sizeZ = configuration.get<float>({ "size", "z" });

			std::optional<unsigned char> colorR = configuration.get<unsigned char>({ "color", "r" });
			std::optional<unsigned char> colorG = configuration.get<unsigned char>({ "color", "g" });
			std::optional<unsigned char> colorB = configuration.get<unsigned char>({ "color", "b" });
			std::optional<unsigned char> colorA = configuration.get<unsigned char>({ "color", "a" });

			std::optional<unsigned char> frameColorR = configuration.get<unsigned char>({ "frame_color", "r" });
			std::optional<unsigned char> frameColorG = configuration.get<unsigned char>({ "frame_color", "g" });
			std::optional<unsigned char> frameColorB = configuration.get<unsigned char>({ "frame_color", "b" });
			std::optional<unsigned char> frameColorA = configuration.get<unsigned char>({ "frame_color", "a" });

			float sx = sizeX.value_or(100.0f);
			float sy = sizeY.value_or(100.0f);
			float sz = sizeZ.value_or(100.0f);
			
			HARMONY_ASSERT(sx > 0.0f, "Pyramid size X must be positive");
			HARMONY_ASSERT(sy > 0.0f, "Pyramid size Y must be positive");
			HARMONY_ASSERT(sz > 0.0f, "Pyramid size Z must be positive");
			
			size = Vector3{sx, sy, sz};

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
			
			HARMONY_TRACE("Pyramid component initialized - Size: ({}, {}, {})", size.x, size.y, size.z);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Pyramid component: {}", e.what());
			throw Harmony::Exceptions::ComponentInitializationException("Pyramid", e.what());
		}
	}
	
	Pyramid::~Pyramid() 
	{
		HARMONY_TRACE("Pyramid component destroyed for entity {}", static_cast<uint32_t>(entityId_));
	}

	void Pyramid::onRender()
	{
		try {
			// Draw pyramid using raylib's DrawMesh or manual triangle drawing
			// For simplicity, using a cone-like shape with 4 sides
			DrawCylinder(Vector3{ 0.0f, 0.0f, 0.0f }, 0.0f, size.x / 2.0f, size.y, 4, color);
			DrawCylinderWires(Vector3{ 0.0f, 0.0f, 0.0f }, 0.0f, size.x / 2.0f, size.y, 4, frameColor);
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception during Pyramid render for entity {}: {}", 
				static_cast<uint32_t>(entityId_), e.what());
		}
	}
}
