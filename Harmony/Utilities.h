#pragma once
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Logger.h"

namespace harmony::utilities
{
	inline float degreesToRadians(float degrees) {
		return degrees * 3.1415926535f / 180.0f;
	}

	template<typename Type>
	inline Type generateRandomNumber() {
		static std::random_device rd;
		static std::mt19937_64 engine(rd());
		static std::uniform_int_distribution<Type> dist;
		return dist(engine);
	}

	template<typename Type>
	inline Type generateRandomNumber(Type min, Type max) {
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<Type> distribution(min, max);
		return distribution(generator);
	}

	template<typename Type>
	int generateRandomNumber(const std::vector<std::pair<Type, Type>>& ranges) {
		std::random_device rd;
		std::mt19937 generator(rd());

		// Choose a random range
		std::uniform_int_distribution<size_t> rangeSelector(0, ranges.size() - 1);
		size_t chosenRangeIndex = rangeSelector(generator);

		// Generate a random number within the chosen range
		int min = ranges[chosenRangeIndex].first;
		int max = ranges[chosenRangeIndex].second;
		std::uniform_int_distribution<Type> numberSelector(min, max);

		return numberSelector(generator);
	}

	inline sf::Color getInterpolatedColor(float elapsedTime, float Duration, const std::vector<sf::Color>& colorStages) {
		// Calculate progress and get the interpolated color
		float progress = elapsedTime / Duration;
		
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

	inline float sinus(float period, float amplitude, float elapsedTime, float phase = 0.0f) {
		float frequency = 1.0f / period;
		return amplitude * std::sin(2.0f * 3.1415926535f * frequency * elapsedTime + phase);
	}

	inline sf::FloatRect getViewBounds(const sf::View& view) {
		const float left = view.getCenter().x - view.getSize().x / 2;
		const float top = view.getCenter().y - view.getSize().y / 2;
		const float width = view.getSize().x;
		const float height = view.getSize().y;

		return sf::FloatRect(left, top, width, height);
	}
}