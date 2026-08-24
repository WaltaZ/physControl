#include "../include/mesh/meshElements/face.h"

#include <algorithm>

#include <cuda_runtime.h>
#include <utility/cudaConfig.h>

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
    const V& area,
    int nodeIDsLength,
    const int* nodeIDs,
    int ownerCellID,
    const CellData<meshDimToGeometryDim(dim)>& ownerData,
    std::optional<int> neighbourCellID,
    std::optional<CellData<meshDimToGeometryDim(dim)>> neighbourData,
    std::optional<V> ownerToNeighbourCell
)
    : area(area),
    nodeIDsLength(nodeIDsLength),
    ownerCellID(ownerCellID),
    ownerData(ownerData),
    neighbourCellID(neighbourCellID),
    neighbourData(neighbourData),
    ownerToNeighbourCell(ownerToNeighbourCell)
{
    //cudaMallocManaged(&(this->nodeIDs), nodeIDsLength * sizeof(int));
    //cudaMemAdvise(this->nodeIDs, nodeIDsLength * sizeof(int), cudaMemAdviseSetPreferredLocation, cudaConfig::hostLocation);

    /*std::copy(
        nodeIDs,
        nodeIDs + nodeIDsLength,
        this->nodeIDs
    );*/
}

template<MeshDim dim>
Face<dim>::~Face()
{
    //cudaFree(this->nodeIDs);
};

template<MeshDim dim>
Face<dim>::Face(const Face<dim>& other) : Face(
    other.area,
    other.nodeIDsLength,
    other.nodeIDs,
    other.ownerCellID,
    other.ownerData,
    other.neighbourCellID,
    other.neighbourData,
    other.ownerToNeighbourCell
    ) {};

template<MeshDim dim>
Face<dim>::Face(Face<dim>&& other) noexcept
    : area(other.area),
    nodeIDsLength(other.nodeIDsLength),
    nodeIDs(other.nodeIDs),
    ownerCellID(other.ownerCellID),
    ownerData(other.ownerData),
    neighbourCellID(other.neighbourCellID),
    neighbourData(other.neighbourData),
    ownerToNeighbourCell(other.ownerToNeighbourCell)
{
    other.nodeIDs = nullptr;
    other.nodeIDsLength = 0;
};

template<MeshDim dim>
Face<dim>& Face<dim>::operator=(const Face<dim>& other)
{
    if (this != &other) {
        int* newNodeIDs;

        //cudaMallocManaged(&newNodeIDs, other.nodeIDsLength * sizeof(int));
        //cudaMemAdvise(newNodeIDs, other.nodeIDsLength * sizeof(int), cudaMemAdviseSetPreferredLocation, cudaConfig::hostLocation);

        /*std::copy(
            other.nodeIDs,
            other.nodeIDs + other.nodeIDsLength,
            newNodeIDs
        );

        cudaFree(this->nodeIDs);*/

        area = other.area;
        //nodeIDs = newNodeIDs;
        nodeIDsLength = other.nodeIDsLength;
        ownerCellID = other.ownerCellID;
        ownerData = other.ownerData;
        neighbourCellID = other.neighbourCellID;
        neighbourData = other.neighbourData;
        ownerToNeighbourCell = other.ownerToNeighbourCell;
    };
    return *this;
};

template<MeshDim dim>
Face<dim>& Face<dim>::operator=(Face<dim>&& other) noexcept
{
    if (this != &other) {
        //cudaFree(this->nodeIDs);

        area = other.area;
        nodeIDs = other.nodeIDs;
        nodeIDsLength = other.nodeIDsLength;
        ownerCellID = other.ownerCellID;
        ownerData = other.ownerData;
        neighbourCellID = std::move(other.neighbourCellID);
        neighbourData = std::move(other.neighbourData);
        ownerToNeighbourCell = std::move(other.ownerToNeighbourCell);

        other.nodeIDs = nullptr;
        other.nodeIDsLength = 0;
    }

    return *this;
};

template class Face<MeshDim::D2>;
template class Face<MeshDim::D3>;