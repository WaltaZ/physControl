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

	// The array containing normalized (relative to the each cuboid's length) points, marking start and the end of the boundary condition 
	// along the proper axis. The pair is either XY, XZ or YZ (the lower [in terms of indexing] axis is always first)
	std::array<std::array<double, 2>, 2> range;
	std::array<int, 2> indices = { (axis12 > axis23), (axis23 > axis12) };

	range[indices[0]][0] = (surface.vertices[0]->pos[axis12] - cuboid.points[0]->pos[axis12]) / _getCuboidDimension(axis12);
	range[indices[0]][1] = (surface.vertices[1]->pos[axis12] - cuboid.points[0]->pos[axis12]) / _getCuboidDimension(axis12);
	range[indices[1]][0] = (surface.vertices[1]->pos[axis23] - cuboid.points[0]->pos[axis23]) / _getCuboidDimension(axis23);
	range[indices[1]][1] = (surface.vertices[2]->pos[axis23] - cuboid.points[0]->pos[axis23]) / _getCuboidDimension(axis23);

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

double CartesianMesher<MeshDim::D3>::_getCuboidDimension(int axis) {
	switch (axis) {
	case 0:
		return this->cuboid.a;
	case 1:
		return this->cuboid.b;
	case 2:
		return this->cuboid.c;
	}
	throw("Axis exeeding 3 dimensions");
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

		for (int dim = 0; dim < 3; dim++) {

			std::vector<int> indices = { 0, 1, 2 };
			indices.erase(indices.begin() + dim);

			if (!mesherBcVelocity.empty()) {
				for (const auto& bc : mesherBcVelocity) {
					if (bc.data.face == Cuboid::faceOrder[2 * dim] || bc.data.face == Cuboid::faceOrder[2 * dim + 1]) {
						for (int i = 0; i < 2; i++) {
							for (int j = 0; j < 2; j++) {
								divisionPatches[indices[i]].push_back(bc.data.range[i][j]);
							}
						}
					}
				};
			}

			if (!mesherBcTemperature.empty()) {
				for (const auto& bc : mesherBcTemperature) {
					if (bc.data.face == Cuboid::faceOrder[2 * dim] || bc.data.face == Cuboid::faceOrder[2 * dim + 1]) {
						for (int i = 0; i < 2; i++) {
							for (int j = 0; j < 2; j++) {
								divisionPatches[indices[i]].push_back(bc.data.range[i][j]);
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

				for (auto& t : divisionPattern[i]) {
					std::cout << "[ " << i << " ] " << cuboid.points[0]->pos[i] + (_getCuboidDimension(i) * t) << std::endl;
				}
			}
		}
	}

	const std::array<int, 3> sizes = {
		divisionPattern[0].size(),
		divisionPattern[1].size(),
		divisionPattern[2].size()
	};

	// Nodes --------------------------------------------------

	for (int z = 0; z < sizes[2]; z++) {
		for (int y = 0; y < sizes[1]; y++) {
			for (int x = 0; x < sizes[0]; x++) {
				Point<GeometryDim::D3> point({
					cuboid.points[0]->pos[0] + (cuboid.a * divisionPattern[0][x]),
					cuboid.points[0]->pos[1] + (cuboid.b * divisionPattern[1][y]),
					cuboid.points[0]->pos[2] + (cuboid.c * divisionPattern[2][z])
					});
				mesh.nodes.push_back(MesherNode<MeshDim::D3>(point));
			}
		}
	}

	// Faces and cells ----------------------------------------

	std::array<bool, 6> currentNeighbours{false};
	std::array<bool, 6> currentBoundaries{false};

	auto setBoundariesAndNeighbours =
		[
			&currentBoundaries,
			&currentNeighbours,
			sizes
		]
		(
			int coord,
			int iteration, 
			int index1, 
			int index2
		) 
		{
		if (iteration == 0) {
			currentBoundaries[index1] = true;
			currentBoundaries[index2] = false;
			currentNeighbours[index1] = false;
		}
		else if (iteration == sizes[coord] - 2) {
			currentBoundaries[index1] = false;
			currentBoundaries[index2] = true;
			currentNeighbours[index1] = true;
		}
		else {
			currentBoundaries[index1] = false;
			currentBoundaries[index2] = false;
			currentNeighbours[index1] = true;
		};
	};

	auto globalIndex2Node = [sizes](std::array<int, 3> i) { return i[0] + (i[1] * sizes[0]) + (i[2] * sizes[0] * sizes[1]);};
	auto globalIndex2Cell = [sizes](std::array<int, 3> i) { return i[0] + (i[1] * (sizes[0] - 1)) + (i[2] * (sizes[0] - 1) * (sizes[1] - 1));};
	auto nodeIDsFromFace = [globalIndex2Node](int faceType, std::array<int, 3> i) {
		switch (faceType) {
		case 0:
			return std::array<int, 4>{
				globalIndex2Node({ i[0] + 1, i[1], i[2] }),
				globalIndex2Node({ i[0] + 1, i[1]+1, i[2] }),
				globalIndex2Node({ i[0] + 1, i[1]+1, i[2]+1 }),
				globalIndex2Node({ i[0] + 1, i[1], i[2]+1 })
			};
		case 1:
			return std::array<int, 4>{
				globalIndex2Node({ i[0], i[1], i[2] }),
				globalIndex2Node({ i[0], i[1] + 1, i[2] }),
				globalIndex2Node({ i[0], i[1] + 1, i[2] + 1 }),
				globalIndex2Node({ i[0], i[1], i[2] + 1 })
			};
		case 2:
			return std::array<int, 4>{
				globalIndex2Node({ i[0], i[1] + 1, i[2] }),
				globalIndex2Node({ i[0] + 1, i[1] + 1, i[2] }),
				globalIndex2Node({ i[0] + 1, i[1] + 1, i[2] + 1 }),
				globalIndex2Node({ i[0], i[1] + 1, i[2] + 1 })
			};
		case 3:
			return std::array<int, 4>{
				globalIndex2Node({ i[0], i[1], i[2] }),
				globalIndex2Node({ i[0] + 1, i[1], i[2] }),
				globalIndex2Node({ i[0] + 1, i[1], i[2] + 1 }),
				globalIndex2Node({ i[0], i[1], i[2] + 1 })
			};
		case 4:
			return std::array<int, 4>{
				globalIndex2Node({ i[0], i[1], i[2] + 1}),
					globalIndex2Node({ i[0] + 1, i[1], i[2] + 1}),
					globalIndex2Node({ i[0] + 1, i[1]+1, i[2] + 1 }),
					globalIndex2Node({ i[0], i[1]+1, i[2] + 1 })
			};
		case 5:
			return std::array<int, 4>{
				globalIndex2Node({ i[0], i[1], i[2]}),
					globalIndex2Node({ i[0] + 1, i[1], i[2]}),
					globalIndex2Node({ i[0] + 1, i[1] + 1, i[2]}),
					globalIndex2Node({ i[0], i[1] + 1, i[2]})
			};
		}
	};

	auto neighbourCellIndices = [](int faceType, std::array<int, 3> i) {
		switch (faceType) {
		case 0:
			return std::array<int, 3>({ i[0] + 1, i[1], i[2]});
		case 1:
			return std::array<int, 3>({ i[0] - 1, i[1], i[2]});
		case 2:
			return std::array<int, 3>({ i[0], i[1] + 1, i[2]});
		case 3:
			return std::array<int, 3>({ i[0] , i[1] - 1, i[2]});
		case 4:
			return std::array<int, 3>({ i[0], i[1], i[2] + 1});
		case 5:
			return std::array<int, 3>({ i[0], i[1], i[2] - 1});
		}
	};

	auto switchOrderOfTheFaces = [](int faceType) {
		int axis = faceType / 2;
		int baseType = axis * 2;
		int result = faceType;

		if (faceType == baseType) { result++; }
		else { result--; };

		return result;
	};
	
	for (int z = 0; z < sizes[2]-1; z++) {

		setBoundariesAndNeighbours(2, z, 5, 4);

		for (int y = 0; y < sizes[1]-1; y++) {

			setBoundariesAndNeighbours(1, y, 3, 2);

			for (int x = 0; x < sizes[0]-1; x++) {
				
				setBoundariesAndNeighbours(0, x, 1, 0);

				MesherCell<MeshDim::D3> cell{};
				std::vector<MesherFace<MeshDim::D3>> faces{};

				for (int i = 0; i < 6; i++) {

					std::array<int, 4> pointIDs = nodeIDsFromFace(i, { x, y, z });

					if (currentNeighbours[i] == true) {
						int neighbourCellId = globalIndex2Cell(neighbourCellIndices(i, {x, y, z}));

						int neighbourFaceId = mesh.cells[neighbourCellId].faceIDs[switchOrderOfTheFaces(i)];
						mesh.faces[neighbourFaceId].neighbourCellID = mesh.cells.size();

						// Cell
						cell.faceIDs.push_back(neighbourFaceId);
						cell.neighbourCellsIDs.push_back(neighbourCellId);

					}
					else {
						std::array<int, 4> facePointIDs = nodeIDsFromFace(i, { x, y, z });
						RectangleD3 surface(
							new Point<GeometryDim::D3>(mesh.nodes[facePointIDs[0]].pos),
							new Point<GeometryDim::D3>(mesh.nodes[facePointIDs[1]].pos),
							new Point<GeometryDim::D3>(mesh.nodes[facePointIDs[2]].pos),
							new Point<GeometryDim::D3>(mesh.nodes[facePointIDs[3]].pos)
						);

						// FACE
						MesherFace<MeshDim::D3> face{};
						// TODO: Check if it's pointing outwards
						// TODO: Optimize that \/
						face.area = VectorData<GeometryDim::D3>{
							surface.getAreaVector(),
							surface.getAreaVector().getNormal(),
							surface.getAreaVector().getMagnitude()
						};

						face.centroid = surface.getCentroid();

						face.nodeIDs = std::vector<int>(facePointIDs.begin(), facePointIDs.end());
						face.ownerCellID = mesh.cells.size();

						mesh.faces.push_back(face);

						cell.faceIDs.push_back(mesh.faces.size() - 1);
						
					}
					cell.pointIDs.insert(cell.pointIDs.end(), std::begin(pointIDs), std::end(pointIDs));

				}
				// Sort the vector
				std::sort(cell.pointIDs.begin(), cell.pointIDs.end());

				// Move all duplicates to last of vector
				auto it = std::unique(cell.pointIDs.begin(), cell.pointIDs.end());

				// Remove all duplicates
				cell.pointIDs.erase(it, cell.pointIDs.end());

				Cuboid cellCuboid(
					mesh.nodes[cell.pointIDs[1]].pos[0] - mesh.nodes[cell.pointIDs[0]].pos[0],
					mesh.nodes[cell.pointIDs[2]].pos[1] - mesh.nodes[cell.pointIDs[0]].pos[1],
					mesh.nodes[cell.pointIDs[4]].pos[2] - mesh.nodes[cell.pointIDs[0]].pos[2]
				);

				cell.centroid = cellCuboid.getCentroid();
				cell.volume = cellCuboid.getVolume();

				mesh.cells.push_back(cell);


			}
		}
	}

	// Setting up rest of the data inside faces

	for (auto& face : mesh.faces) {
		MesherCell<MeshDim::D3>& ownerCell = mesh.cells[face.ownerCellID];
		Vector<GeometryDim::D3> ownerToFace(ownerCell.centroid, face.centroid);
		face.ownerData = CellData<GeometryDim::D3>{
			VectorData<GeometryDim::D3>{
				ownerToFace,
				ownerToFace.getNormal(),
				ownerToFace.getMagnitude()
			}
		};
		
		if (face.neighbourCellID.has_value()) {
			MesherCell<MeshDim::D3>& neighbourCell = mesh.cells[face.neighbourCellID.value()];
			Vector<GeometryDim::D3> neighbourToFace(ownerCell.centroid, face.centroid);
			face.neighbourData = CellData<GeometryDim::D3>{
			VectorData<GeometryDim::D3>{
					neighbourToFace,
					neighbourToFace.getNormal(),
					neighbourToFace.getMagnitude()
				}
			};
			Vector<GeometryDim::D3> ownerToNeighbour(ownerCell.centroid, neighbourCell.centroid);
			face.ownerToNeighbourCell = VectorData<GeometryDim::D3>{
				ownerToNeighbour,
				ownerToNeighbour.getNormal(),
				ownerToNeighbour.getMagnitude()
			};
		}
	}
	

	return mesh.createMeshInHeap();
};
