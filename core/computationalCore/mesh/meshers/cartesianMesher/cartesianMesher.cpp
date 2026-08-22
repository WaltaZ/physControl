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
		
		if (surface.vertices[0]->pos[0] == cuboid.points[0]->pos[0]) {
			faceType = Cuboid::FaceType::Back;
		}
		else if (surface.vertices[0]->pos[0] == cuboid.points[1]->pos[0]) {
			faceType = Cuboid::FaceType::Front;
		};
	}
	else if ((axis12 == 0 && axis23 == 2) || (axis12 == 2 && axis23 == 0)) {
		// XZ

		if (surface.vertices[0]->pos[1] == cuboid.points[0]->pos[1]) {
			faceType = Cuboid::FaceType::Left;
		}
		else if (surface.vertices[0]->pos[1] == cuboid.points[3]->pos[1]) {
			faceType = Cuboid::FaceType::Right;
		};
	}
	else if ((axis12 == 0 && axis23 == 1) || (axis12 == 1 && axis23 == 0)) {
		// XY

		if (surface.vertices[0]->pos[2] == cuboid.points[0]->pos[2]) {
			faceType = Cuboid::FaceType::Bottom;
		}
		else if (surface.vertices[0]->pos[2] == cuboid.points[4]->pos[2]) {
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

	range[indices[0]][0] = (surface.vertices[0]->pos[axis12] - cuboid.points[0]->pos[axis12]) / getCuboidDimension(axis12, cuboid);
	range[indices[0]][1] = (surface.vertices[1]->pos[axis12] - cuboid.points[0]->pos[axis12]) / getCuboidDimension(axis12, cuboid);
	range[indices[1]][0] = (surface.vertices[1]->pos[axis23] - cuboid.points[0]->pos[axis23]) / getCuboidDimension(axis23, cuboid);
	range[indices[1]][1] = (surface.vertices[2]->pos[axis23] - cuboid.points[0]->pos[axis23]) / getCuboidDimension(axis23, cuboid);

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
	cuboid(problemGeometry.getCuboid()),
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

	std::vector<MesherBC<BoundaryConditionD3<Field<V, C>>>> mesherBcVelocity;
	std::vector<MesherBC<BoundaryConditionD3<Field<double, C>>>> mesherBcTemperature;

	for (const auto& bc : problem.velocityBoundaries) {
		MesherBCData data = _getMesherBCDataFromSurface(bc.geometry);
		MesherBC<BoundaryConditionD3<Field<V, C>>> mesherBC{
			bc,
			data
		};
		mesherBcVelocity.push_back(mesherBC);
	}

	for (const auto& bc : problem.temperatureBoundaries) {
		MesherBCData data = _getMesherBCDataFromSurface(bc.geometry);
		MesherBC<BoundaryConditionD3<Field<double, C>>> mesherBC{
			bc,
			data
		};
		mesherBcTemperature.push_back(mesherBC);
	}

	// Patch the divisions for the boundaries to stick to the nodes:

	if (!mesherBcVelocity.empty() || !mesherBcTemperature.empty()) {
		std::array<std::vector<double>, 3> divisionPatches;

		for (int i = 0; i < 3; i++) {

			std::vector<int> indices = { 0, 1, 2 };
			indices.erase(indices.begin() + i);

			if (!mesherBcVelocity.empty()) {
				for (const auto& bc : mesherBcVelocity) {
					if (bc.data.face == Cuboid::faceOrder[2 * i] || bc.data.face == Cuboid::faceOrder[2 * i + 1]) {
						for (int i = 0; i < 2; i++) {
							for (int j = 0; j < 2; j++) {
								divisionPatches[indices[i]].push_back(bc.data.range[indices[i]][j]);
							}
						}
					}
				};
			}

			if (!mesherBcTemperature.empty()) {
				for (const auto& bc : mesherBcTemperature) {
					if (bc.data.face == Cuboid::faceOrder[2 * i] || bc.data.face == Cuboid::faceOrder[2 * i + 1]) {
						for (int i = 0; i < 2; i++) {
							for (int j = 0; j < 2; j++) {
								divisionPatches[indices[i]].push_back(bc.data.range[indices[i]][j]);
							}
						}
					}
				};
			}
		}

		for (int i = 0; i < 3; i++) {
			if (!divisionPatches[i].empty()) {
				std::sort(divisionPatches[i].begin(), divisionPatches[i].end());

				if (divisionPatches[i][divisionPatches[i].size() - 1] != 1.0) {
					divisionPatches[i].push_back(1.0);
				}
				if (divisionPatches[i][0] != 0) {
					divisionPatches[i].insert(divisionPatches[i].begin(), 0);
				}
				divisionPattern[i] = mathUtils::linearlyInterpolatePointsWithSpacing(divisionPatches[i], (1 / (double)refinments[i]));

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

				for (auto& t : divisionPattern[i]) {
					std::cout << "[ " << i << " ] " << cuboid.points[0]->pos[i] + (getCuboidDimension(i, cuboid) * t) << std::endl;
				}
			}
		}
	}

	for (int z = 0; z < divisionPattern[2].size(); z++) {
		for (int y = 0; y < divisionPattern[1].size(); y++) {
			for (int x = 0; x < divisionPattern[0].size(); x++) {
				Point<GeometryDim::D3> point({
					cuboid.points[0]->pos[0] + (cuboid.a * divisionPattern[0][x]),
					cuboid.points[0]->pos[1] + (cuboid.b * divisionPattern[1][y]),
					cuboid.points[0]->pos[2] + (cuboid.c * divisionPattern[2][z])
					});
				mesh.nodes.push_back(MesherNode<MeshDim::D3>(point));
			}
		}
	}
	

	return mesh.createMeshInHeap();
};
