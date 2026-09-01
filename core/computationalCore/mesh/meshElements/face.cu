#include "../include/mesh/meshElements/face.h"

template<MeshDim dim>
Face<dim>::Face(
    const MesherFace<dim>& mesherFace,
    const CudaArray<uint32_t>& faceNodeIDs
)
    : area(mesherFace.area),
    centroid(mesherFace.centroid),
    isBoundary(!mesherFace.neighbourCellID.has_value()),
    faceNodeIDs(faceNodeIDs),
    ownerCellID(mesherFace.ownerCellID),
    ownerData(mesherFace.ownerData)
{
    if (isBoundary) { return; }

    ownerFaceWeightFactor = mesherFace.ownerFaceWeightFactor.value();
    neighbourCellID = mesherFace.neighbourCellID.value();
    neighbourData = mesherFace.neighbourData.value();
    ownerToNeighbourCell = mesherFace.ownerToNeighbourCell.value();
}

template<MeshDim dim>
__host__ __device__
const CellData<mesh2geom(dim)>& Face<dim>::getCellData(uint32_t id) const {
    if (id == ownerCellID || isBoundary) {
        return ownerData;
    }
    return neighbourData;
}

template<MeshDim dim>
__host__ __device__
double Face<dim>::getWeightFactor(uint32_t id) const {
    if (isBoundary) { printf("Face is a boundary face. Cannot access weight factor."); }
    if (id == ownerCellID) {
        return ownerFaceWeightFactor;
    }
    return 1 - ownerFaceWeightFactor;
}

template<MeshDim dim>
__host__ __device__
uint32_t Face<dim>::getNeighbourCellID(uint32_t id) const {
    if (isBoundary) { printf("Face is a boundary face. Cannot access neighbour cell ID"); }
    if (id == ownerCellID) {
        return neighbourCellID;
    }
    return ownerCellID;
};

template<MeshDim dim>
__host__ __device__
VectorData<mesh2geom(dim)> Face<dim>::getArea(uint32_t id) const {
    if (id == ownerCellID || isBoundary) {
        return area;
    }
    return VectorData<mesh2geom(dim)>{
        area.vector * (-1),
        area.normal * (-1),
        area.magnitude
    };
};

template class Face<MeshDim::D2>;
template class Face<MeshDim::D3>;