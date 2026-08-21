#pragma once

#include "../../meshEnums.h"
#include "../mesherElements.h"
#include "../../meshElements/mesh.h"

#include <problem/problem.h>
#include <utility.h>
#include <geometry/shapes/cuboid.h>

template<MeshDim>
class _CartesianMesher {};

template<MeshDim dim>
class CartesianMesher : _CartesianMesher<dim> {};

template<>
class CartesianMesher<MeshDim::D3> : _CartesianMesher<MeshDim::D3> {
private:

	using V = Vector<GeometryDim::D3>;
	using C = Cell<geometryDimToMeshDim(GeometryDim::D3)>;

	const static GeometryDim Gdim = GeometryDim::D3;

	// Problem

	const Cuboid& problemGeometry;
	const RoomHeatTransferD3& problem;

	// Mesh

	struct MesherBCData {
		Cuboid::FaceType face;
		std::array<std::array<double, 2>, 2> range;
	};

	template<typename T>
	struct MesherBC {
		const T& boundaryCondition;
		MesherBCData data;
	};

	std::array<int, geometryDimSize(Gdim)> refinments;
	std::vector<std::vector<double>> divisionPattern{};

	// Functions

	std::vector<double> _linspace(const int& index) const;
	MesherBCData _getMesherBCDataFromSurface(const Surface<GeometryDim::D3>& surface);

public:

	CartesianMesher(
		const RoomHeatTransferD3& problem,
		const ProblemGeometryCuboid& problemGeometry,
		const std::array<int, geometryDimSize(Gdim)>& refinments = { 10, 10, 10 });

	void setDivisionPattern(const std::vector<double>& divisionPattern, int index);

	const Mesh<MeshDim::D3> createMesh();
};