#include "../include/mesh/meshElements/face.h"

template<MeshDim dim>
bool Face<dim>::isBoundary() const
{
	return !neighbourCellID.has_value();
}

template<MeshDim dim>
Face<dim>::Face() {}

// Rule of 5 ------------------------------------------------------------

template<MeshDim dim>
Face<dim>::Face(
    const MesherFace<dim>& mesherFace,
    const CudaArray& faceNodeIDs
)
    : area(mesherFace.area),
    faceNodeIDs(faceNodeIDs),
    ownerCellID(mesherFace.ownerCellID),
    ownerData(mesherFace.ownerData),
    neighbourCellID(mesherFace.neighbourCellID),
    neighbourData(mesherFace.neighbourData),
    ownerToNeighbourCell(mesherFace.ownerToNeighbourCell)
{}

template class Face<MeshDim::D2>;
template class Face<MeshDim::D3>;