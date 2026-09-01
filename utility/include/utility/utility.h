#pragma once

#include <cmath>

namespace calc {

	constexpr double eps = 10e-12;

	template<typename T>
	bool approxEq(T a, T b) {
		return (std::abs(a - b) < eps);
	};
}