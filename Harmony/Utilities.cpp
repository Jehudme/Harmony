#include "pch.h"
#include "Utilities.h"
#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>

namespace Harmony::Utilities
{
    sf::Color getInterpolatedColor(float elapsedTime, float duration, const std::vector<sf::Color>& colorStages) {
        // Calculate progress and get the interpolated color
        float progress = elapsedTime / duration;

        // Ensure progress is clamped between 0 and 1
        progress = std::clamp(progress, 0.0f, 1.0f);

        // Determine the current and next stage based on progress
        size_t currentStage = static_cast<size_t>(progress * (colorStages.size() - 1));
        size_t nextStage = std::min(currentStage + 1, colorStages.size() - 1);

        // Calculate the relative progress within the current stage
        float stageProgress = (progress * (colorStages.size() - 1)) - currentStage;

        // Retrieve the start and end colors
        const sf::Color& startColor = colorStages[currentStage];
        const sf::Color& endColor = colorStages[nextStage];

        // Interpolate the color components
        sf::Uint8 red = static_cast<sf::Uint8>(startColor.r + stageProgress * (endColor.r - startColor.r));
        sf::Uint8 green = static_cast<sf::Uint8>(startColor.g + stageProgress * (endColor.g - startColor.g));
        sf::Uint8 blue = static_cast<sf::Uint8>(startColor.b + stageProgress * (endColor.b - startColor.b));
        sf::Uint8 alpha = static_cast<sf::Uint8>(startColor.a + stageProgress * (endColor.a - startColor.a));

        return sf::Color(red, green, blue, alpha);
    }

    float sinus(float period, float amplitude, float elapsedTime, float phase) {
        float frequency = 1.0f / period;
        return amplitude * std::sin(2.0f * 3.1415926535f * frequency * elapsedTime + phase);
    }

    sf::FloatRect getViewBounds(const sf::View& view) {
        const float left = view.getCenter().x - view.getSize().x / 2;
        const float top = view.getCenter().y - view.getSize().y / 2;
        const float width = view.getSize().x;
        const float height = view.getSize().y;

        return sf::FloatRect(left, top, width, height);
    }

    float degreesToRadians(float degrees) {
        return degrees * 3.1415926535f / 180.0f;
    }
}