#include "../include/mesh/meshElements/face.h"

template<MeshDim dim>
bool Face<dim>::isBoundary() const
{
	return !neighbourCellID.has_value();
}

// Rule of 5 ------------------------------------------------------------

template<MeshDim dim>
Face<dim>::Face(
    const MesherFace<dim>& mesherFace,
    const CudaArray<uint32_t>& faceNodeIDs
)
    : area(mesherFace.area),
    centroid(mesherFace.centroid),
    faceNodeIDs(faceNodeIDs),
    ownerCellID(mesherFace.ownerCellID),
    ownerData(mesherFace.ownerData),
    ownerFaceWeightFactor(mesherFace.ownerFaceWeightFactor),
    neighbourCellID(mesherFace.neighbourCellID),
    neighbourData(mesherFace.neighbourData),
    ownerToNeighbourCell(mesherFace.ownerToNeighbourCell)
{}

template<MeshDim dim>
__host__ __device__
const CellData<mesh2geom(dim)>& Face<dim>::getCellData(uint32_t id) const {
    if (id == ownerCellID) {
        return ownerData;
    }
    return neighbourData.value();
}

template<MeshDim dim>
__host__ __device__
double Face<dim>::getWeightFactor(uint32_t id) const {
    if (id == ownerCellID) {
        return ownerFaceWeightFactor.value();
    }
    return 1 - ownerFaceWeightFactor.value();
}

template<MeshDim dim>
__host__ __device__
uint32_t Face<dim>::getNeighbourCellID(uint32_t id) const {
    if (id == ownerCellID) {
        return neighbourCellID.value();
    }
    return ownerCellID;
};

template<MeshDim dim>
__host__ __device__
VectorData<mesh2geom(dim)> Face<dim>::getArea(uint32_t id) const {
    if (id == ownerCellID) {
        return VectorData<mesh2geom(dim)>{
            Vector<mesh2geom(dim)>(),
            Vector<mesh2geom(dim)>(),
            0
        };
    }
    return VectorData<mesh2geom(dim)>{
        area.vector * (-1),
        area.normal * (-1),
        area.magnitude
    };
};

template class Face<MeshDim::D2>;
template class Face<MeshDim::D3>;