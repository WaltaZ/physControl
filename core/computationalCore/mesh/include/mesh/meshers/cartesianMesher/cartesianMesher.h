#pragma once

#include "../../meshEnums.h"
#include "../mesherElements.h"
#include "../../meshElements/mesh.h"

#include <problem/problemGeometry.h>

template<MeshDim>
class _CartesianMesher {};

template<MeshDim dim>
class CartesianMesher : _CartesianMesher<dim> {};

template<>
class CartesianMesher<MeshDim::D3> : _CartesianMesher<MeshDim::D3> {
private:
	const static GeometryDim Gdim = GeometryDim::D3;
	const ProblemGeometryCuboid& problemGeometry;
	std::array<int, geometryDimSize(Gdim)> refinments;
	std::vector<std::vector<double>> divisionPattern{};

	std::vector<double> _linspace(const int& index) const;

public:

	CartesianMesher(
		const ProblemGeometryCuboid& problemGeometry,
		const std::array<int, geometryDimSize(Gdim)>& refinments = { 10, 10, 10 });

	void setDivisionPattern(const std::vector<double>& divisionPattern, int index);

	const Mesh<MeshDim::D3> createMesh();
};