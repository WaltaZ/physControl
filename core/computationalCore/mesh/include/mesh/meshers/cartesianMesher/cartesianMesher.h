#pragma once

#include "../../meshEnums.h"
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
	const ProblemGeometry<Gdim, Cuboid>& problemGeometry;
	std::array<int, geometryDimSize(Gdim)> refinments;
	std::vector<std::vector<std::vector<double>>> divisionPatterns{};

	std::vector<double> _linspace(const int& index) const;

public:

	CartesianMesher(
		const ProblemGeometry<Gdim, Cuboid>& problemGeometry,
		const std::array<int, geometryDimSize(Gdim)>& refinments = { 10, 10, 10 });

	void setDivisionPattern(const std::vector<std::vector<double>>& divisionPattern, int index);

	Mesh<MeshDim::D3> createMesh();
};