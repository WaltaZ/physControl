#pragma once

#include <cmath>

namespace calc {

	constexpr double eps = 10e-12;

	template<typename T>
	bool approxEq(T a, T b) {
		return (std::abs(a - b) < eps);
	};

	template<typename T>
	T max(T* num, size_t length) {
		T max = 0;
		for (size_t i = 0; i < length; i++)
		{
			if (num[i] > max) { max = num[i]; }
		}
		return max;
	}
}