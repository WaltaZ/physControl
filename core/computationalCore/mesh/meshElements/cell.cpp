#include "../include/mesh/meshElements/cell.h"

#include <algorithm>
#include <cuda_runtime.h>

template<MeshDim dim>
Cell<dim>::Cell(
    const int* nodeIDs,
    int nodeIDsLength,

    const int* faceIDs,
    int faceIDsLength,

    const int* neighbourCellsIDs,
    int neighbourCellsIDsLength,

    const P& centroid,
    double volume
)
    :
    nodeIDsLength(nodeIDsLength),
    faceIDsLength(faceIDsLength),
    neighbourCellsIDsLength(neighbourCellsIDsLength),

    centroid(centroid),
    volume(volume)
{
    cudaMallocManaged(&(this->nodeIDs), nodeIDsLength * sizeof(int));
    cudaMallocManaged(&(this->faceIDs), faceIDsLength * sizeof(int));
    cudaMallocManaged(&(this->neighbourCellsIDs), neighbourCellsIDsLength * sizeof(int));

    std::copy(nodeIDs, nodeIDs + nodeIDsLength, this->nodeIDs);
    std::copy(faceIDs, faceIDs + faceIDsLength, this->faceIDs);
    std::copy(neighbourCellsIDs, neighbourCellsIDs + neighbourCellsIDsLength, this->neighbourCellsIDs);
}

template<MeshDim dim>
Cell<dim>::Cell() {};

// rule of 5 ----------------------------------------

template<MeshDim dim>
Cell<dim>::~Cell()
{
    cudaFree(nodeIDs);
    cudaFree(faceIDs);
    cudaFree(neighbourCellsIDs);
};

template<MeshDim dim>
Cell<dim>::Cell(const Cell<dim>& other) : Cell(
    other.nodeIDs,
    other.nodeIDsLength,
    other.faceIDs,
    other.faceIDsLength,
    other.neighbourCellsIDs,
    other.neighbourCellsIDsLength,
    other.centroid,
    other.volume
)
{}

template<MeshDim dim>
Cell<dim>::Cell(Cell<dim>&& other) noexcept :
    nodeIDs(other.nodeIDs),
    nodeIDsLength(other.nodeIDsLength),
    faceIDs(other.faceIDs),
    faceIDsLength(other.faceIDsLength),
    neighbourCellsIDs(other.neighbourCellsIDs),
    neighbourCellsIDsLength(other.neighbourCellsIDsLength),
    centroid(other.centroid),
    volume(other.volume)
{
    other.nodeIDs = nullptr;
    other.nodeIDsLength = 0;
    other.faceIDs = nullptr;
    other.faceIDsLength = 0;
    other.neighbourCellsIDs = nullptr;
    other.neighbourCellsIDsLength = 0;
}

template<MeshDim dim>
Cell<dim>* Cell<dim>::operator=(const Cell<dim>& other)
{
    if (this != &other) {
        Cell<dim> temp(other);

        std::swap(nodeIDs, temp.nodeIDs);
        std::swap(nodeIDsLength, temp.nodeIDsLength);

        std::swap(faceIDs, temp.faceIDs);
        std::swap(faceIDsLength, temp.faceIDsLength);

        std::swap(neighbourCellsIDs, temp.neighbourCellsIDs);
        std::swap(neighbourCellsIDsLength, temp.neighbourCellsIDsLength);

        std::swap(centroid, temp.centroid);
        std::swap(volume, temp.volume);
    }
    return this;
}

template<MeshDim dim>
Cell<dim>* Cell<dim>::operator=(Cell<dim>&& other) noexcept
{
    if (this != &other) {
        cudaFree(nodeIDs);
        cudaFree(faceIDs);
        cudaFree(neighbourCellsIDs);

        this->nodeIDs = other.nodeIDs;
        this->nodeIDsLength = other.nodeIDsLength;
        this->faceIDs = other.faceIDs;
        this->faceIDsLength = other.faceIDsLength;
        this->neighbourCellsIDs = other.neighbourCellsIDs;
        this->neighbourCellsIDsLength = other.neighbourCellsIDsLength;

        this->centroid = other.centroid;
        this->volume = other.volume;

        other.nodeIDs = nullptr;
        other.nodeIDsLength = 0;
        other.faceIDs = nullptr;
        other.faceIDsLength = 0;
        other.neighbourCellsIDs = nullptr;
        other.neighbourCellsIDsLength = 0;
    }
    return this;
}

template class Cell<MeshDim::D2>;
template class Cell<MeshDim::D3>;