#include "../../include/mesh/meshers/cartesianMesher/cartesianMesher.h"

std::vector<double> CartesianMesher<MeshDim::D3>::_linspace(const int& index) const {

	const int& refinment = refinments[index];

	double interval = 1.0 / (double)refinment;
	std::vector<double> result(refinment+1);
	for (int i = 0; i <= refinment; i++) {
		result[i] = i * interval;
	}
	return result;
}

MesherBoundaryCondition CartesianMesher<MeshDim::D3>::_getMesherBCFromSurface(const Surface<GeometryDim::D3>& surface)
{
	using P = Point<GeometryDim::D3>;
	using V = Vector<GeometryDim::D3>;

	V v12(*surface.vertices[0], *surface.vertices[1]);
	V v23(*surface.vertices[1], *surface.vertices[2]);

	// The type of field
	Cuboid::FaceType faceType{};

	auto checkAxis = [](const V& v) {

		int zeroCount = 0;
		int axis = 0;

		for (int i = 0; i < 3; i++) {
			if (calc::approxEq<double>(v.comp[i], 0)){
				zeroCount++;
			}
			else {
				axis = i;
			}
		}

		assert(zeroCount == 2 && "The rectangle doesn't lie on one of the axes");
		return axis;
	};

	int axis12 = checkAxis(v12);
	int axis23 = checkAxis(v23);

	bool isFaceTypeSet = false;

	if ((axis12 == 1 && axis23 == 2) || (axis12 == 2 && axis23 == 1)) {
		// YZ
		
		if (calc::approxEq(surface.vertices[0]->pos[0], cuboid.points[0]->pos[0])) {
			faceType = Cuboid::FaceType::Back;
			isFaceTypeSet = true;
		}
		else if (calc::approxEq(surface.vertices[0]->pos[0], cuboid.points[1]->pos[0])) {
			faceType = Cuboid::FaceType::Front;
			isFaceTypeSet = true;
		};
	}
	else if ((axis12 == 0 && axis23 == 2) || (axis12 == 2 && axis23 == 0)) {
		// XZ

		if (calc::approxEq(surface.vertices[0]->pos[1], cuboid.points[0]->pos[1])) {
			faceType = Cuboid::FaceType::Left;
			isFaceTypeSet = true;
		}
		else if (calc::approxEq(surface.vertices[0]->pos[1], cuboid.points[3]->pos[1])) {
			faceType = Cuboid::FaceType::Right;
			isFaceTypeSet = true;
		};
	}
	else if ((axis12 == 0 && axis23 == 1) || (axis12 == 1 && axis23 == 0)) {
		// XY

		if (calc::approxEq(surface.vertices[0]->pos[2], cuboid.points[0]->pos[2])) {
			faceType = Cuboid::FaceType::Bottom;
			isFaceTypeSet = true;
		}
		else if (calc::approxEq(surface.vertices[0]->pos[2], cuboid.points[4]->pos[2])) {
			faceType = Cuboid::FaceType::Top;
			isFaceTypeSet = true;
		};
	}
	else {
		throw("The shape is not a face");
	}

	if (!isFaceTypeSet) { throw("Could not classify boundary surface to a cuboid face"); }

	// The array containing normalized (relative to the each cuboid's length) points, marking start and the end of the boundary condition 
	// along the proper axis. The pair is either XY, XZ or YZ (the lower [in terms of indexing] axis is always first)
	std::array<std::array<double, 2>, 2> range;
	std::array<int, 2> indices = { (axis12 > axis23), (axis23 > axis12) };

	range[indices[0]][0] = (surface.vertices[0]->pos[axis12] - cuboid.points[0]->pos[axis12]); // _getCuboidDimension(axis12);
	range[indices[0]][1] = (surface.vertices[1]->pos[axis12] - cuboid.points[0]->pos[axis12]); // _getCuboidDimension(axis12);
	range[indices[1]][0] = (surface.vertices[1]->pos[axis23] - cuboid.points[0]->pos[axis23]); // _getCuboidDimension(axis23);
	range[indices[1]][1] = (surface.vertices[2]->pos[axis23] - cuboid.points[0]->pos[axis23]); // _getCuboidDimension(axis23);

	for (int i = 0; i < 2; i++) {
		if (range[i][0] > range[i][1]) {
			double buffer = range[i][0];
			range[i][0] = range[i][1];
			range[i][1] = buffer;
		}
	}

	return MesherBoundaryCondition(
		faceType,
		range
	);
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
	ProblemD3& problem,
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

const Mesh<MeshDim::D3> CartesianMesher<MeshDim::D3>::generateMesh()
{
	// TODO: Something breaks up when there's more than 1 boundary condition.

	MesherMesh mesh = MesherMesh<MeshDim::D3>();

	// Set up boundary conditions for the mesher

	std::vector<MesherBoundaryCondition> mesherBC{};
	std::vector<MesherBoundaryConditionRaw> mesherBCDefault(problem.boundaryConditions.size());

	for(auto& bcVariable : problem.boundaryConditions)
	for (auto& bc : bcVariable) {
		mesherBC.emplace_back(_getMesherBCFromSurface(bc.geometry));
	}

	// Patch the divisions for the boundaries to stick to the nodes:

	auto sortUnique = [](std::vector<double>& v) {
		sort(v.begin(), v.end());
		auto it = unique(v.begin(), v.end(), calc::approxEq<double>);
		v.erase(it, v.end());
	};

	if (!mesherBC.empty()) {
		std::array<std::vector<double>, 3> divisionPatches;

		for (int dim = 0; dim < 3; dim++) {

			std::vector<int> indices = { 0, 1, 2 };
			indices.erase(indices.begin() + dim);

				for (const auto& bc : mesherBC) {
					if (bc.face == Cuboid::faceOrder[2 * dim] || bc.face == Cuboid::faceOrder[2 * dim + 1]) {
						for (int i = 0; i < 2; i++) {
							for (int j = 0; j < 2; j++) {
								divisionPatches[indices[i]].emplace_back(bc.range[i][j]/_getCuboidDimension(indices[i]));
							}
						}
					}
				};
		}

		for (int i = 0; i < 3; i++) {
			if (!divisionPatches[i].empty()) {
				sortUnique(divisionPatches[i]);

				if (divisionPatches[i][divisionPatches[i].size() - 1] != 1.0) {
					divisionPatches[i].emplace_back(1.0);
				}
				if (divisionPatches[i][0] != 0) {
					divisionPatches[i].insert(divisionPatches[i].begin(), 0);
				}
				divisionPattern[i] = mathUtils::linearlyInterpolatePointsWithSpacing(divisionPatches[i], (1 / (double)refinments[i]));
				
				/*for (auto& t : divisionPattern[i]) {
					std::cout << "[ " << i << " ] " << cuboid.points[0]->pos[i] + (_getCuboidDimension(i) * t) << std::endl;
				}*/
			}
		}
	}

	auto test = divisionPattern[2][0] == divisionPattern[2][1];

	const std::array<int, 3> sizes = {
		divisionPattern[0].size(),
		divisionPattern[1].size(),
		divisionPattern[2].size()
	};

	// Nodes --------------------------------------------------

	for (int z = 0; z < sizes[2]; z++) {
		for (int y = 0; y < sizes[1]; y++) {
			for (int x = 0; x < sizes[0]; x++) {
				mesh.nodes.emplace_back(std::array<double, 3>{
					cuboid.points[0]->pos[0] + (cuboid.a * divisionPattern[0][x]),
					cuboid.points[0]->pos[1] + (cuboid.b * divisionPattern[1][y]),
					cuboid.points[0]->pos[2] + (cuboid.c * divisionPattern[2][z])}
				);
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

				C cell{};
				std::vector<F> faces{};

				// TODO: Clean up this shit and make it readable \/

				for (int i = 0; i < 6; i++) {

					std::array<int, 4> pointIDs = nodeIDsFromFace(i, { x, y, z });

					if (currentNeighbours[i] == true) {
						int neighbourCellId = globalIndex2Cell(neighbourCellIndices(i, {x, y, z}));

						int neighbourFaceId = mesh.cells[neighbourCellId].faceIDs[switchOrderOfTheFaces(i)];
						mesh.faces[neighbourFaceId].neighbourCellID = mesh.cells.size();

						// Cell
						cell.faceIDs.push_back(neighbourFaceId);
						cell.neighbourCellsIDs.push_back(neighbourCellId);
						mesh.cells[neighbourCellId].neighbourCellsIDs.emplace_back(mesh.cells.size());

					}
					else {

						const uint32_t newFaceID = mesh.faces.size();

						std::array<int, 4> facePointIDs = nodeIDsFromFace(i, { x, y, z });
						RectangleD3 surface(
							new Point<GeometryDim::D3>(mesh.nodes[facePointIDs[0]].pos),
							new Point<GeometryDim::D3>(mesh.nodes[facePointIDs[1]].pos),
							new Point<GeometryDim::D3>(mesh.nodes[facePointIDs[2]].pos),
							new Point<GeometryDim::D3>(mesh.nodes[facePointIDs[3]].pos)
						);

						auto test = surface.getAreaVector();

						// FACE
		
						// TODO: Optimize that \/
						F face{
							VectorData<GeometryDim::D3>(surface.getAreaVector()),
							surface.getCentroid(),
							std::vector<int>(facePointIDs.begin(), facePointIDs.end()),
							mesh.cells.size()
						};

						mesh.faces.push_back(face);

						cell.faceIDs.push_back(newFaceID);

						if (currentBoundaries[i] == true) {

							int mesherBCIndex = 0;

							for (int j = 0; j < problem.boundaryConditions.size(); j++) {

								bool isDefaultBC = true;

								for (int k = 0; k < problem.boundaryConditions[j].size(); k++) {

									if (Cuboid::faceOrder[i] == mesherBC[mesherBCIndex].face) {

										const std::array<double, 3>& p1 = mesh.nodes[pointIDs[0]].pos;
										const std::array<double, 3>& p2 = mesh.nodes[pointIDs[2]].pos;

										const std::array<std::array<double, 3>, 2> p = { p1, p2 };

										std::vector<int> indices = { 0, 1, 2 };
										for (int l = 0; l < 3; l++) {
											if (p1[l] == p2[l]) {
												indices.erase(indices.begin() + l);
												break;
											}
										}

										bool isInsideBoundaryGeometry = true;

										for (int l = 0; l < 2; l++) {
											for (int m = 0; m < 2; m++) {
												if (!(p[l][indices[m]] >= mesherBC[mesherBCIndex].range[m][0] && 
													p[l][indices[m]] <= mesherBC[mesherBCIndex].range[m][1])) {
													isInsideBoundaryGeometry = false;
													break;
												};
											}
											if (!isInsideBoundaryGeometry) { break; }
										}

										if (isInsideBoundaryGeometry) {
											mesherBC[mesherBCIndex].faceIDs.emplace_back(newFaceID);
											isDefaultBC = false;
										}
									}

									mesherBCIndex++;

								}

								if (isDefaultBC) {
									mesherBCDefault[j].faceIDs.emplace_back(newFaceID);
								}

							}
						}
					}
					cell.nodeIDs.insert(cell.nodeIDs.end(), std::begin(pointIDs), std::end(pointIDs));
				}

				std::sort(cell.nodeIDs.begin(), cell.nodeIDs.end());
				auto it = std::unique(cell.nodeIDs.begin(), cell.nodeIDs.end());
				cell.nodeIDs.erase(it, cell.nodeIDs.end());

				std::iter_swap(cell.nodeIDs.begin() + 2, cell.nodeIDs.begin() + 3);
				std::iter_swap(cell.nodeIDs.begin() + 6, cell.nodeIDs.begin() + 7);

				Cuboid cellCuboid(
					mesh.nodes[cell.nodeIDs[1]].pos[0] - mesh.nodes[cell.nodeIDs[0]].pos[0],
					mesh.nodes[cell.nodeIDs[2]].pos[1] - mesh.nodes[cell.nodeIDs[0]].pos[1],
					mesh.nodes[cell.nodeIDs[4]].pos[2] - mesh.nodes[cell.nodeIDs[0]].pos[2],
					Point<GeometryDim::D3>({
						mesh.nodes[cell.nodeIDs[0]].pos[0],
						mesh.nodes[cell.nodeIDs[0]].pos[1],
						mesh.nodes[cell.nodeIDs[0]].pos[2]
					})
				);

				cell.centroid = cellCuboid.getCentroid();
				cell.volume = cellCuboid.getVolume();

				mesh.cells.push_back(cell);
			}
		}
	}

	// Setting up rest of the data inside faces

	for (auto& face : mesh.faces) {
		C& ownerCell = mesh.cells[face.ownerCellID];
		V ownerToFace(ownerCell.centroid, face.centroid);

		// d_Cf
		face.ownerData = CellData<GeometryDim::D3>{
			VectorData<GeometryDim::D3>(ownerToFace)
		};

		// Check if the area vector is pointing outwards the owner cell

		double d_Cf_dot_e_f = geomOp::vecDotProduct(
			face.ownerData.centroidToFace.vector,
			face.area.normal
		);

		if (d_Cf_dot_e_f < 0) {
			face.area.vector.flip();
			face.area.normal.flip();
			d_Cf_dot_e_f *= -1;
		}
		
		if (face.neighbourCellID.has_value()) {
			C& neighbourCell = mesh.cells[face.neighbourCellID.value()];
			V neighbourToFace(neighbourCell.centroid, face.centroid);

			// d_Ff
			face.neighbourData = CellData<GeometryDim::D3>{
				VectorData<GeometryDim::D3>(neighbourToFace)
			};

			// d_CF
			V ownerToNeighbour(ownerCell.centroid, neighbourCell.centroid);
			face.ownerToNeighbourCell = VectorData<GeometryDim::D3>(ownerToNeighbour);
			
			// g_C

			double denominator = d_Cf_dot_e_f - geomOp::vecDotProduct(
				face.neighbourData.value().centroidToFace.vector,
				face.area.normal
			);

			face.ownerFaceWeightFactor = d_Cf_dot_e_f / denominator;
		}
	}

	problem.initBoundaryPatches(mesherBC, mesherBCDefault);

	return Mesh<MeshDim::D3>(mesh);
};
