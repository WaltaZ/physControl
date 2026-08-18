#include "../include/mesh/meshElements/face.h"

template<MeshDim dim>
Face<dim>::Face(
	double area,
	const V& normalVector,
	const P& centroid,
	const std::vector<int>& nodeIDs,
	int ownerCellID,
	std::optional<int> neighbourCellID
)
	: area(area),
	normalVector(normalVector),
	centroid(centroid),
	nodeIDs(nodeIDs),
	ownerCellID(ownerCellID),
	neighbourCellID(neighbourCellID) {
}

template<MeshDim dim>
bool Face<dim>::isBoundary() const
{
	return !neighbourCellID.has_value();
}

template class Face<MeshDim::D2>;
template class Face<MeshDim::D3>;