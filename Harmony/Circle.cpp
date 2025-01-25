#include "pch.h"
#include "Circle.h"

harmony::Circle::Circle(const Configuration& configuration)
    : SceneNode(configuration), circle(std::make_shared<sf::CircleShape>()) {
	
    drawable = circle;
    if (const auto radius = configuration.getData<float>({ "Radius" })) {
        circle->setRadius(radius.value());
    }

    if (const auto fillColor = configuration.getData({ "FillColor" })) {
        circle->setFillColor(sf::Color(fillColor.value()[0], fillColor.value()[1], fillColor.value()[2], fillColor.value()[3]));
    }

    if (const auto outlineColor = configuration.getData({ "OutlineColor" })) {
        circle->setOutlineColor(sf::Color(outlineColor.value()[0], outlineColor.value()[1], outlineColor.value()[2], outlineColor.value()[3]));
    }

    if (const auto outlineThickness = configuration.getData<float>({ "OutlineThickness" })) {
        circle->setOutlineThickness(outlineThickness.value());
    }

    if (const auto textureRect = configuration.getData({ "TextureRect" })) {
        circle->setTextureRect(sf::IntRect(textureRect.value()[0], textureRect.value()[1], textureRect.value()[2], textureRect.value()[3]));
    }
}
