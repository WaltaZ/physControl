#include "../include/mesh/meshElements/cell.h"

#include <algorithm>
#include <cuda_runtime.h>

template<MeshDim dim>
Cell<dim>::Cell(
    const MesherCell<dim>& mesherCell,
    const CudaArray& cellNodeIDs,
    const CudaArray& cellFaceIDs,
    const CudaArray& cellNeighbourCells
)
    :
    cellNodeIDs(cellNodeIDs),
    cellFaceIDs(cellFaceIDs),
    cellNeighbourCells(cellNeighbourCells),

    centroid(mesherCell.centroid),
    volume(mesherCell.volume)
{}

template<MeshDim dim>
Cell<dim>::Cell() {};

template class Cell<MeshDim::D2>;
template class Cell<MeshDim::D3>;