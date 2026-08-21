#include "../../include/mesh/meshers/cartesianMesher/cartesianMesher.h"


std::vector<double> CartesianMesher<MeshDim::D3>::_linspace(const int& index) const {

	const int& refinment = refinments[index];

	double interval = 1.0 / (double)refinment;
	std::vector<double> result(refinment);
	for (int i = 0; i < refinment; i++) {
		result[i] = (i + 1) * interval;
	}
	return result;
}

CartesianMesher<MeshDim::D3>::MesherBCData CartesianMesher<MeshDim::D3>::_getMesherBCDataFromSurface(const Surface<GeometryDim::D3>& surface)
{
	using P = Point<GeometryDim::D3>;
	using V = Vector<GeometryDim::D3>;

	V v12(*surface.vertices[0], *surface.vertices[1]);
	V v23(*surface.vertices[1], *surface.vertices[2]);

	// The type of field
	Cuboid::FaceType faceType;

	auto checkAxis = [](const V& v) {
		constexpr double eps = 1e-11;

		int zeroCount = 0;
		int axis = 0;

		for (int i = 0; i < 3; i++) {
			if (std::abs(v.comp[i]) < eps) {
				zeroCount++;
			}
			else {
				axis = i;
			}
		}

		assert(zeroCount == 2, "The rectangle doesn't lie on one of the axes");
		return axis;
	};

	int axis12 = checkAxis(v12);
	int axis23 = checkAxis(v23);

	if ((axis12 == 1 && axis23 == 2) || (axis12 == 2 && axis23 == 1)) {
		// YZ
		
		if (surface.vertices[0]->pos[0] == problemGeometry.points[0]->pos[0]) {
			faceType = Cuboid::FaceType::Back;
		}
		else if (surface.vertices[0]->pos[0] == problemGeometry.points[1]->pos[0]) {
			faceType = Cuboid::FaceType::Front;
		};
	}
	else if ((axis12 == 0 && axis23 == 2) || (axis12 == 2 && axis23 == 0)) {
		// XZ

		if (surface.vertices[0]->pos[1] == problemGeometry.points[0]->pos[1]) {
			faceType = Cuboid::FaceType::Left;
		}
		else if (surface.vertices[0]->pos[1] == problemGeometry.points[3]->pos[1]) {
			faceType = Cuboid::FaceType::Right;
		};
	}
	else if ((axis12 == 0 && axis23 == 1) || (axis12 == 1 && axis23 == 0)) {
		// XY

		if (surface.vertices[0]->pos[2] == problemGeometry.points[0]->pos[2]) {
			faceType = Cuboid::FaceType::Bottom;
		}
		else if (surface.vertices[0]->pos[2] == problemGeometry.points[4]->pos[2]) {
			faceType = Cuboid::FaceType::Top;
		};
	}
	else {
		throw("The shape is not a face");
	}

	auto getCuboidDimension = [](int axis, const Cuboid& cuboid) {
		switch (axis) {
		case 0:
			return cuboid.a;
		case 1:
			return cuboid.b;
		case 2:
			return cuboid.c;
		}

	};

	// The array containing normalized (relative to the each cuboid's length) points, marking start and the end of the boundary condition 
	// along the proper axis. The pair is either XY, XZ or YZ (the lower [in terms of indexing] axis is always first)
	std::array<std::array<double, 2>, 2> range;
	std::array<int, 2> indices = { (axis12 > axis23), (axis23 > axis12) };

	range[indices[0]][0] = (surface.vertices[0]->pos[axis12] - problemGeometry.points[0]->pos[axis12]) / getCuboidDimension(axis12, problemGeometry);
	range[indices[0]][1] = (surface.vertices[1]->pos[axis12] - problemGeometry.points[0]->pos[axis12]) / getCuboidDimension(axis12, problemGeometry);
	range[indices[1]][0] = (surface.vertices[1]->pos[axis23] - problemGeometry.points[0]->pos[axis23]) / getCuboidDimension(axis23, problemGeometry);
	range[indices[1]][1] = (surface.vertices[2]->pos[axis23] - problemGeometry.points[0]->pos[axis23]) / getCuboidDimension(axis23, problemGeometry);

	for (int i = 0; i < 2; i++) {
		if (range[i][0] > range[i][1]) {
			double buffer = range[i][0];
			range[i][0] = range[i][1];
			range[i][1] = buffer;
		}
	}

	return MesherBCData{
		faceType,
		range
	};
};

CartesianMesher<MeshDim::D3>::CartesianMesher(
	const RoomHeatTransferD3& problem,
	const ProblemGeometryCuboid& problemGeometry,
	const std::array<int, geometryDimSize(Gdim)>& refinments)
	:
	problem(problem),
	problemGeometry(problemGeometry.getCuboid()),
	refinments(refinments)
{
	// TODO: Fix that \/
	//constexpr bool isCuboid = std::is_same_v<decltype(problemGeometry.operationalSpace), Cuboid>;
	//assert(isCuboid);

	for (int j = 0; j < geometryDimSize(Gdim); j++) {
		divisionPattern.push_back(_linspace(j));
	}
}

void CartesianMesher<MeshDim::D3>::setDivisionPattern(
	const std::vector<double>& divisionPattern,
	int index)
{
	this->divisionPattern[index] = divisionPattern;
}

const Mesh<MeshDim::D3> CartesianMesher<MeshDim::D3>::createMesh()
{
	MesherMesh mesh = MesherMesh<MeshDim::D3>();

	// Set up boundary conditions for the mesher
	std::vector<MesherBC<BoundaryConditionD3<Field<V, C>>>> bcDataVelocity;
	std::vector<MesherBC<BoundaryConditionD3<Field<double, C>>>> bcDataTemperature;

	for (const auto& bc : problem.velocityBoundaries) {
		MesherBCData data = _getMesherBCDataFromSurface(bc.geometry);
		MesherBC<BoundaryConditionD3<Field<V, C>>> mesherBC{
			bc,
			data
		};
		bcDataVelocity.push_back(mesherBC);
	}

	for (const auto& bc : problem.temperatureBoundaries) {
		MesherBCData data = _getMesherBCDataFromSurface(bc.geometry);
		MesherBC<BoundaryConditionD3<Field<double, C>>> mesherBC{
			bc,
			data
		};
		bcDataTemperature.push_back(mesherBC);
	}

	

	return mesh.createMeshInHeap();
};
