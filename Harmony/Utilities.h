#pragma once
#include <cstdint>
#include <random>
#include <algorithm>

namespace Harmony::Utilities
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
	inline Type generateRandomNumber(int min, int max) {
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<int> distribution(min, max);
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
}