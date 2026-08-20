#include "../../include/mesh/meshers/cartesianMesher/cartesianMesher.h"

std::vector<double> CartesianMesher<MeshDim::D3>::_linspace(const int& index) const {

	const int& refinment = refinments[index];

	double interval = 1.0 / (double)refinment;
	std::vector<double> result(refinment);
	for (int i = 0; i < refinment; i++) {
		result[i] = (i + 1) * interval;
	}
	return result;
};

CartesianMesher<MeshDim::D3>::CartesianMesher(
	const ProblemGeometryCuboid& problemGeometry,
	const std::array<int, geometryDimSize(Gdim)>& refinments)
	:
	problemGeometry(problemGeometry),
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
	const Cuboid& cuboid = problemGeometry.getCuboid();

	MesherMesh mesh = MesherMesh<MeshDim::D3>();

	// Nodes ------------------------------------------------------------------------

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

	// Faces ------------------------------------------------------------------------
	// Set up 
	return mesh.createMeshInHeap();
};
