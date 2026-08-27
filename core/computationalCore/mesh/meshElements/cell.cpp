#include "../include/mesh/meshElements/cell.h"

#include <algorithm>
#include <cuda_runtime.h>

template<MeshDim dim>
Cell<dim>::Cell(
    const MesherCell<dim>& mesherCell,
    const CudaArray<uint32_t>& cellNodeIDs,
    const CudaArray<uint32_t>& cellFaceIDs,
    const CudaArray<uint32_t>& cellNeighbourCells
)
    :
    cellNodeIDs(cellNodeIDs),
    cellFaceIDs(cellFaceIDs),
    cellNeighbourCells(cellNeighbourCells),

    centroid(mesherCell.centroid),
    volume(mesherCell.volume)
{}

template class Cell<MeshDim::D2>;
template class Cell<MeshDim::D3>;