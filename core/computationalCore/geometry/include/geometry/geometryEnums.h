#pragma once

#include <numeric>

enum class GeometryDim {
	D2 = 2,
	D3 = 3
};

constexpr std::size_t geometryDimSize(GeometryDim dim) {
	return static_cast<std::size_t>(dim);
}