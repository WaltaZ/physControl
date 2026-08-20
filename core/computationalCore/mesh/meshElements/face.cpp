#include "../include/mesh/meshElements/face.h"

template<MeshDim dim>
bool Face<dim>::isBoundary() const
{
	return !neighbourCellID.has_value();
}

template<MeshDim dim>
Face<dim>::Face(
    const V& area,
    int* nodeIDs,
    int nodeIDsLength,
    int ownerCellID,
    const CellData<meshDimToGeometryDim(dim)>& ownerData,
    std::optional<int> neighbourCellID,
    std::optional<CellData<meshDimToGeometryDim(dim)>> neighbourData,
    std::optional<V> ownerToNeighbourCell
)
    : area(area),
    nodeIDs(nodeIDs),
    nodeIDsLength(nodeIDsLength),
    ownerCellID(ownerCellID),
    ownerData(ownerData),
    neighbourCellID(neighbourCellID),
    neighbourData(neighbourData),
    ownerToNeighbourCell(ownerToNeighbourCell)
{};

template class Face<MeshDim::D2>;
template class Face<MeshDim::D3>;