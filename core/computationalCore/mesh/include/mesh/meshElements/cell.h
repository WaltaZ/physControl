#pragma once

#include "../meshEnums.h"
#include "supportStructs.h"

#include <vector>

template<MeshDim dim>
class Cell {
private:
	using P = Point<meshDimToGeometryDim(dim)>;
	using V = Vector<meshDimToGeometryDim(dim)>;
public:
	std::vector<int> pointIDs;
	std::vector<int> faceIDs;
	std::vector<int> neighbourCellsIDs;

	P centroid;
	double volume;

	std::vector<NeighbourVectors<meshDimToGeometryDim(dim)>> neighboursVectors;
	std::vector<NeighbourVectorsMagnitudes> neighboursVectorsMagnitudes;

	Cell(
		std::vector<int> pointIDs,
		std::vector<int> faceIDs,
		std::vector<int> neighbourCellsIDs,
		P centroid,
		double volume,
		std::vector<NeighbourVectors<meshDimToGeometryDim(dim)>> neighboursVectors,
		std::vector<NeighbourVectorsMagnitudes> neighboursVectorsMagnitudes,
		std::vector<int> faceNormalsOrient
	);
};