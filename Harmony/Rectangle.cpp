#include "pch.h"
#include "Rectangle.h"

harmony::Rectangle::Rectangle(const Configuration& configuration)
	: SceneNode(configuration), rectangle(std::make_shared<sf::RectangleShape>())
{
	drawable = rectangle;
	if (const auto size = configuration.getData({ "Size" })) {
		rectangle->setSize({ size.value()[0], size.value()[1] });
	}

	if (const auto fillColor = configuration.getData({ "FillColor" })) {
		rectangle->setFillColor(sf::Color(fillColor.value()[0], fillColor.value()[1], fillColor.value()[2], fillColor.value()[3]));
	}

	if (const auto outlineColor = configuration.getData({ "OutlineColor" })) {
		rectangle->setOutlineColor(sf::Color(outlineColor.value()[0], outlineColor.value()[1], outlineColor.value()[2], outlineColor.value()[3]));
	}
	if (const auto outlineThickness = configuration.getData<float>({ "OutlineThickness" })) {
		rectangle->setOutlineThickness(outlineThickness.value());
	}
	//if (const auto texture = configuration.getData<std::string>({ "Texture" })) {
	//	rectangle->setTexture(&TextureManager::getTexture(texture.value()));
	//}
	if (const auto textureRect = configuration.getData({ "TextureRect" })) {
		rectangle->setTextureRect(sf::IntRect(textureRect.value()[0], textureRect.value()[1], textureRect.value()[2], textureRect.value()[3]));
	}
}

