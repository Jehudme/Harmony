#pragma once
#include "Scene.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Texture.h"

namespace Harmony::Scenes {
	class Scene;
}

namespace Harmony::Components
{
	template<typename Type>
	class Shape : public Type
	{
	public:
		Shape(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~Shape() = default;
	};

	template<typename Type>
	inline Shape<Type>::Shape(const Utilities::Configuration& configuration, Scenes::Scene& scene)
	{
		configuration.debugPrint();
		if (std::optional<std::uint64_t> textureId = configuration.get<std::uint64_t>({ "texture" }))
		{
			Resources::Resource& resource = scene.engine.resourceManager->get("texture", textureId.value());
			this->setTexture(&convert<Resources::Texture>(resource));
		}

		if (std::optional<unsigned int> outlineThickness = configuration.get<unsigned int>({ "outline_thickness" }))
			this->setOutlineThickness(static_cast<float>(outlineThickness.value()));
		else HARMONY_WARN("Rectangle component missing outline thickness configuration");

		std::optional<unsigned int> fillR = configuration.get<unsigned int>({ "fill_color", "r" });
		std::optional<unsigned int> fillG = configuration.get<unsigned int>({ "fill_color", "g" });
		std::optional<unsigned int> fillB = configuration.get<unsigned int>({ "fill_color", "b" });
		std::optional<unsigned int> fillA = configuration.get<unsigned int>({ "fill_color", "a" });

		std::optional<unsigned int> outlineR = configuration.get<unsigned int>({ "outline_color", "r" });
		std::optional<unsigned int> outlineG = configuration.get<unsigned int>({ "outline_color", "g" });
		std::optional<unsigned int> outlineB = configuration.get<unsigned int>({ "outline_color", "b" });
		std::optional<unsigned int> outlineA = configuration.get<unsigned int>({ "outline_color", "a" });

		if (!fillR.has_value())     HARMONY_WARN("Shape component missing fill color red configuration");
		if (!fillG.has_value())     HARMONY_WARN("Shape component missing fill color green configuration");
		if (!fillB.has_value())     HARMONY_WARN("Shape component missing fill color blue configuration");
		if (!fillA.has_value())     HARMONY_WARN("Shape component missing fill color alpha configuration");
		
		if (!outlineR.has_value())  HARMONY_WARN("Shape component missing outline color red configuration");
		if (!outlineG.has_value())  HARMONY_WARN("Shape component missing outline color green configuration");
		if (!outlineB.has_value())  HARMONY_WARN("Shape component missing outline color blue configuration");
		if (!outlineA.has_value())  HARMONY_WARN("Shape component missing outline color alpha configuration");

		this->setFillColor(sf::Color(fillR.value_or(255), fillG.value_or(255), fillB.value_or(255), fillA.value_or(255)));
		this->setOutlineColor(sf::Color(outlineR.value_or(255), outlineG.value_or(255), outlineB.value_or(255), outlineA.value_or(255)));
	}
}