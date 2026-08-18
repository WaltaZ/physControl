#include "include/mesh/meshElements/node.h"

template<MeshDim dim>
Node<dim>::Node(const std::array<double, meshDimSize(dim)>& pos) : pos(pos) {};

template<MeshDim dim>
Node<dim>::Node(const Point<meshDimToGeometryDim(dim)>& point) : pos(point.pos) {};

template class Node<MeshDim::D2>;
template class Node<MeshDim::D3>;