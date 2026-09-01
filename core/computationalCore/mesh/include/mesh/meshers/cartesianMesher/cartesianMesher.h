#pragma once

#include "../../meshEnums.h"
#include "../mesherElements.h"
#include "../../meshElements/mesh.h"

#include <problem/problem.h>
#include <utility/mathUtils.h>
#include <utility/utility.h>
#include <geometry/shapes/cuboid.h>
#include <geometry/shapes/rectangle.h>
#include <algorithm>

template<MeshDim>
class _CartesianMesher {};

template<MeshDim dim>
class CartesianMesher : _CartesianMesher<dim> {};

template<>
class CartesianMesher<MeshDim::D3> : _CartesianMesher<MeshDim::D3> {
private:

	using V = Vector<GeometryDim::D3>;

	using N = MesherNode<MeshDim::D3>;
	using F = MesherFace<MeshDim::D3>;
	using C = MesherCell<MeshDim::D3>;

	const static GeometryDim Gdim = GeometryDim::D3;

	// Problem

	const Cuboid& cuboid;
	ProblemD3& problem;

	// Mesh

	std::array<int, geometryDimSize(Gdim)> refinments;
	std::vector<std::vector<double>> divisionPattern{};

	// Functions

	std::vector<double> _linspace(const int& index) const;
	MesherBoundaryCondition _getMesherBCFromSurface(const Surface<GeometryDim::D3>& surface);
	double _getCuboidDimension(int axis);

public:

	CartesianMesher(
		ProblemD3& problem,
		const ProblemGeometryCuboid& problemGeometry,
		const std::array<int, geometryDimSize(Gdim)>& refinments = { 10, 10, 10 });

	void setDivisionPattern(const std::vector<double>& divisionPattern, int index);

	const Mesh<MeshDim::D3> generateMesh();
};