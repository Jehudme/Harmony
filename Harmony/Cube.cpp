#include "pch.h"
#include "Cube.h"

HARMONY_REGISTER_COMPONENT_WITH_BASE(Harmony::Components::Renderable, Harmony::Components::Cube, cube);

namespace Harmony::Components {
	Cube::Cube(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
		Renderable(scene, entityId) 
	{
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

		size = Vector3{
			sizeX.value_or(100.0f),
			sizeY.value_or(100.0f),
			sizeZ.value_or(100.0f)
		};

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
	}
	
	Cube::~Cube() = default;

	void Cube::onRender()
	{
		DrawCube(Vector3{ 0.0f, 0.0f, 0.0f }, size.x, size.y, size.z, color);
		DrawCubeWires(Vector3{ 0.0f, 0.0f, 0.0f }, size.x, size.y, size.z, frameColor);
	}
}
