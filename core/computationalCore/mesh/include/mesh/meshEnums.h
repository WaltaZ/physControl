#pragma once

#include<geometry/geometryEnums.h>
#include <stdexcept>

enum class MeshDim {
	D2 = 2,
	D3 = 3
};

constexpr std::size_t meshDimSize(MeshDim dim) {
	return static_cast<std::size_t>(dim);
}

constexpr GeometryDim mesh2geom(MeshDim meshDim) {
	switch (meshDim) {
	case MeshDim::D2:
		return GeometryDim::D2;
	case MeshDim::D3:
		return GeometryDim::D3;
	default:
		throw std::invalid_argument("Invalid MeshDim value");
	}
}

constexpr MeshDim geom2mesh(GeometryDim geometryDim) {
	switch (geometryDim) {
	case GeometryDim::D2:
		return MeshDim::D2;
	case GeometryDim::D3:
		return MeshDim::D3;
	default:
		throw std::invalid_argument("Invalid GeometryDim value");
	}
}