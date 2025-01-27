#pragma once
#include <vector>
#include <cmath>
#include <random>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/View.hpp>

namespace Harmony::Utilities {
    sf::Color getInterpolatedColor(float elapsedTime, float duration, const std::vector<sf::Color>& colorStages);

    float sinus(float period, float amplitude, float elapsedTime, float phase = 0.0f);

    sf::FloatRect getViewBounds(const sf::View& view);

    float degreesToRadians(float degrees);

    template<typename Type>
    inline Type generateRandomNumber() {
        static std::random_device rd;
        static std::mt19937_64 engine(rd());
        static std::uniform_int_distribution<Type> dist;
        return dist(engine);
    }

    template<typename Type>
    inline Type generateRandomNumber(Type min, Type max) {
        static std::random_device rd;
        static std::mt19937 generator(rd());
        std::uniform_int_distribution<Type> distribution(min, max);
        return distribution(generator);
    }

    template<typename Type>
    Type generateRandomNumber(const std::vector<std::pair<Type, Type>>& ranges) {
        static std::random_device rd;
        static std::mt19937 generator(rd());

        // Choose a random range
        std::uniform_int_distribution<size_t> rangeSelector(0, ranges.size() - 1);
        size_t chosenRangeIndex = rangeSelector(generator);

        // Generate a random number within the chosen range
        Type min = ranges[chosenRangeIndex].first;
        Type max = ranges[chosenRangeIndex].second;
        std::uniform_int_distribution<Type> numberSelector(min, max);

        return numberSelector(generator);
    }
}
