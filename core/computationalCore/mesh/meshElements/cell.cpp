#include "include/mesh/meshElements/cell.h"

template<MeshDim dim>
Cell<dim>::Cell(
	std::vector<int> pointIDs,
	std::vector<int> faceIDs,
	std::vector<int> neighbourCellsIDs,
	P centroid,
	double volume,
	std::vector<NeighbourVectors<meshDimToGeometryDim(dim)>> neighboursVectors,
	std::vector<NeighbourVectorsMagnitudes> neighboursVectorsMagnitudes,
	std::vector<int> faceNormalsOrient
) :
	pointIDs(pointIDs),
	faceIDs(faceIDs),
	neighbourCellsIDs(neighbourCellsIDs),
	centroid(centroid),
	volume(volume),
	neighboursVectors(neighboursVectors),
	neighboursVectorsMagnitudes(neighboursVectorsMagnitudes)
{};