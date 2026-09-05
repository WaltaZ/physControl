#pragma once

#include <cmath>
#include <cuda_runtime.h>

namespace calc {

	constexpr double eps = 10e-12;

	template<typename T>
	__host__ __device__
	bool approxEq(const T& a, const T& b) {
		return (std::abs(a - b) < eps);
	};

	template<typename T>
	__host__ __device__
	T max(T* num, size_t length) {
		T max = 0;
		for (size_t i = 0; i < length; i++)
		{
			if (num[i] > max) { max = num[i]; }
		}
		return max;
	}
}