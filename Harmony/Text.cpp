#include "pch.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Text.h"
#include "Scene.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Font.h"

HARMONY_REGISTER_COMPONENT_WITH_BASE(sf::Drawable, Harmony::Components::Text, Text)

namespace Harmony::Components
{
	Text::Text(const Utilities::Configuration& configuration, Scenes::Scene& scene)
	{
		configuration.debugPrint();

		// Load font resource
		if (std::optional<std::uint64_t> fontId = configuration.get<std::uint64_t>({ "font" }))
		{
			Resources::Resource& resource = scene.engine.resourceManager->get("font", fontId.value());
			this->setFont(convert<Resources::Font>(resource));
		}
		else {
			HARMONY_WARN("Text component missing font configuration");
		}

		// Set text string
		if (std::optional<std::string> textString = configuration.get<std::string>({ "string" }))
			this->setString(textString.value());
		else {
			HARMONY_WARN("Text component missing string configuration");
			this->setString("");
		}

		// Set character size
		if (std::optional<unsigned int> characterSize = configuration.get<unsigned int>({ "character_size" }))
			this->setCharacterSize(characterSize.value());
		else {
			HARMONY_WARN("Text component missing character size configuration");
			this->setCharacterSize(30);
		}

		// Set text style
		if (std::optional<unsigned int> style = configuration.get<unsigned int>({ "style" }))
			this->setStyle(style.value());
		else {
			HARMONY_WARN("Text component missing style configuration");
			this->setStyle(sf::Text::Regular);
		}

		// Set outline thickness
		if (std::optional<unsigned int> outlineThickness = configuration.get<unsigned int>({ "outline_thickness" }))
			this->setOutlineThickness(static_cast<float>(outlineThickness.value()));
		else {
			HARMONY_WARN("Text component missing outline thickness configuration");
			this->setOutlineThickness(0.0f);
		}

		// Set fill color
		std::optional<unsigned int> fillR = configuration.get<unsigned int>({ "fill_color", "r" });
		std::optional<unsigned int> fillG = configuration.get<unsigned int>({ "fill_color", "g" });
		std::optional<unsigned int> fillB = configuration.get<unsigned int>({ "fill_color", "b" });
		std::optional<unsigned int> fillA = configuration.get<unsigned int>({ "fill_color", "a" });

		if (!fillR.has_value())     HARMONY_WARN("Text component missing fill color red configuration");
		if (!fillG.has_value())     HARMONY_WARN("Text component missing fill color green configuration");
		if (!fillB.has_value())     HARMONY_WARN("Text component missing fill color blue configuration");
		if (!fillA.has_value())     HARMONY_WARN("Text component missing fill color alpha configuration");

		this->setFillColor(sf::Color(fillR.value_or(255), fillG.value_or(255), fillB.value_or(255), fillA.value_or(255)));

		// Set outline color
		std::optional<unsigned int> outlineR = configuration.get<unsigned int>({ "outline_color", "r" });
		std::optional<unsigned int> outlineG = configuration.get<unsigned int>({ "outline_color", "g" });
		std::optional<unsigned int> outlineB = configuration.get<unsigned int>({ "outline_color", "b" });
		std::optional<unsigned int> outlineA = configuration.get<unsigned int>({ "outline_color", "a" });

		if (!outlineR.has_value())  HARMONY_WARN("Text component missing outline color red configuration");
		if (!outlineG.has_value())  HARMONY_WARN("Text component missing outline color green configuration");
		if (!outlineB.has_value())  HARMONY_WARN("Text component missing outline color blue configuration");
		if (!outlineA.has_value())  HARMONY_WARN("Text component missing outline color alpha configuration");

		this->setOutlineColor(sf::Color(outlineR.value_or(0), outlineG.value_or(0), outlineB.value_or(0), outlineA.value_or(255)));
	}

	Text::~Text() = default;
}
