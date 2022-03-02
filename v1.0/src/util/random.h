// random numbers utility

#pragma once

#include <random>
#include <cassert>

class Random {
	
public:
	
	Random() = delete;
	Random(const Random&) = delete;
	Random& operator = (const Random&) = delete;
	
	inline static double real() {
		static std::random_device device;
		static std::mt19937 generator(device());
		static std::uniform_real_distribution <double> distribution(0.0, 1.0);
		return distribution(generator);
	}
	
	inline static double range(double min, double max) {
		assert(min <= max);
		return min + (max - min) * real();
	}
	
};
