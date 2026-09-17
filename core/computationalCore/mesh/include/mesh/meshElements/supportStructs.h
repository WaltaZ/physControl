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
	
	__host__ __device__
	VectorData();

	__host__ __device__
	VectorData(
		const V& vector,
		const V& normal,
		const double& magnitude
	);

	__host__ __device__
	VectorData(Vector<dim> vector);
};

template<GeometryDim dim>
void to_json(nlohmann::json& j, const VectorData<dim>& vectorData);

template<GeometryDim dim>
void from_json(const nlohmann::json& j, VectorData<dim>& vectorData);

template<GeometryDim dim>
struct CellData {

	using V = VectorData<dim>;

	V centroidToFace;
	std::optional<V> Ef = std::nullopt;
	std::optional<V> Tf = std::nullopt;
};

template<GeometryDim dim>
void to_json(nlohmann::json& j, const CellData<dim>& cellData);

template<GeometryDim dim>
void from_json(const nlohmann::json& j, CellData<dim>& cellData);