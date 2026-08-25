#include "../include/mesh/meshers/mesherElements.h"

// ------------------------------------- NODE ----------------------------------------------

template<MeshDim dim>
MesherNode<dim>::MesherNode(const std::array<double, meshDimSize(dim)>& pos) : pos(pos) {};

template<MeshDim dim>
MesherNode<dim>::MesherNode(const Point<meshDimToGeometryDim(dim)>& point) : pos(point.pos) {};

template class MesherNode<MeshDim::D2>;
template class MesherNode<MeshDim::D3>;

MesherBoundaryCondition::MesherBoundaryCondition(
	const Cuboid::FaceType face,
	const std::array<std::array<double, 2>, 2> range
):
	face(face),
	range(range)
{};