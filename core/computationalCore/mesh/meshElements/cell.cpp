#include "../include/mesh/meshElements/cell.h"

template<MeshDim dim>
Cell<dim>::Cell(
    int* pointIDs,
    int pointIDsLength,

    int* faceIDs,
    int faceIDsLength,

    int* neighbourCellsIDs,
    int neighbourCellsIDsLength,

    const P& centroid,
    double volume
)
    : pointIDs(pointIDs),
    pointIDsLength(pointIDsLength),

    faceIDs(faceIDs),
    faceIDsLength(faceIDsLength),

    neighbourCellsIDs(neighbourCellsIDs),
    neighbourCellsIDsLength(neighbourCellsIDsLength),

    centroid(centroid),
    volume(volume)
{}

template<MeshDim dim>
Cell<dim>::Cell() {};

template<MeshDim dim>
Cell<dim>::~Cell()
{
    delete[] pointIDs;
    delete[] faceIDs;
    delete[] neighbourCellsIDs;
};

template class Cell<MeshDim::D2>;
template class Cell<MeshDim::D3>;