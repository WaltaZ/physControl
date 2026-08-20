#pragma once

#include <geometry/geometryEnums.h>
#include <geometry/vector.h>

#include <optional>

template<GeometryDim dim>
struct VectorData {
	
	using V = Vector<dim>;

	V vector;
	V normal;
	double magnitude;
	
};

template<GeometryDim dim>
struct CellData {

	using V = VectorData<dim>;

	V centroidToFace;
	std::optional<V> Ef = std::nullopt;
	std::optional<V> Tf = std::nullopt;
};