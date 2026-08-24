#include "../include/mesh/meshers/mesherElements.h"

// ------------------------------------- NODE ----------------------------------------------

template<MeshDim dim>
MesherNode<dim>::MesherNode(const std::array<double, meshDimSize(dim)>& pos) : pos(pos) {};

template<MeshDim dim>
MesherNode<dim>::MesherNode(const Point<meshDimToGeometryDim(dim)>& point) : pos(point.pos) {};

template class MesherNode<MeshDim::D2>;
template class MesherNode<MeshDim::D3>;