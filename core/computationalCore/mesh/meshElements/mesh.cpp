#include "../include/mesh/meshElements/mesh.h"

template<MeshDim dim>
Mesh<dim>::Mesh(
		const std::vector<Node<dim>>* nodes,
		const std::vector<Face<dim>>* faces,
		const std::vector<Cell<dim>>* cells)
	:
		nodes((*nodes).data()),
		nodesLength((*nodes).size()),
		faces((*faces).data()),
		facesLength((*faces).size()),
		cells((*cells).data()),
		cellsLength((*cells).size())
{}

template<MeshDim dim>
Mesh<dim>::Mesh(
		const Node<dim>*nodes,
		const double& nodesLength,

		const Face<dim>*faces,
		const double& facesLength,

		const Cell<dim>*cells,
		const double& cellsLength)
	:
		nodes(nodes),
		nodesLength(nodesLength),

		faces(faces),
		facesLength(facesLength),

		cells(cells),
		cellsLength(cellsLength)
{}

template<MeshDim dim>
Mesh<dim> Mesh<dim>::getMeshFromNewPointers(const Node<dim>* nodes, const Face<dim>* faces, const Cell<dim>* cells)
{
	return Mesh<dim>{
		nodes,
		this->nodesLength,

		faces,
		this->facesLength,

		cells,
		this->cellsLength
	};
};

template class Mesh<MeshDim::D2>;
template class Mesh<MeshDim::D3>;