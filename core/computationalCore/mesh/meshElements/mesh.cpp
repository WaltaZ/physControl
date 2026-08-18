#include "../include/mesh/meshElements/mesh.h"

template<MeshDim dim>
Mesh<dim>::Mesh() {};

template class Mesh<MeshDim::D2>;
template class Mesh<MeshDim::D3>;