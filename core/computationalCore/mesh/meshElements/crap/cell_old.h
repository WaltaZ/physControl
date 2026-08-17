#pragma once

#include <vector>
#include <meshTypes.h>

#include "face.h"
#include "point.h"
#include "vector.h"
#include "../supportStructs.h"

template<MeshDim dim>
class _Cell {
private:
	using P = Point<dim>;
	using V = Vector<dim>;
	using F = Face<dim>;
public:
	std::vector<int> pointIDs;
	std::vector<int> faceIDs;
	std::vector<int> neighbourCellsIDs;

	std::vector<int> faceNormalsOrient;
	P centroid;

	std::vector<NeighbourVectors<dim>> neighboursVectors;
	std::vector<NeighbourVectorsMagnitudes> neighboursVectorsMagnitudes;

	bool isCentroidInitilized = false;
	bool isInitilized = false;

	_Cell(
		std::vector<int> pointIDs,
		std::vector<int> faceIDs,
		std::vector<int> neighbourCellsIDs
	) :
		pointIDs(pointIDs),
		faceIDs(faceIDs),
		neighbourCellsIDs(neighbourCellsIDs)
	{};

	virtual void initCentroid(Face<dim> test) = 0;
};

template<MeshDim dim>
class Cell : public _Cell<dim> {};

template<>
class Cell<MeshDim::D2> : public _Cell<MeshDim::D2>{

};