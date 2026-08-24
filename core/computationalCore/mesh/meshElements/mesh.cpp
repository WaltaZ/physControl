#include "../include/mesh/meshElements/mesh.h"

#include <cuda_runtime.h>

template<MeshDim dim>
Mesh<dim>::~Mesh()
{
	for (int i = 0; i < cellsLength; i++) {
		cells[i].~Cell();
	}

	for (int i = 0; i < facesLength; i++) {
		faces[i].~Face();
	}
	for (int i = 0; i < nodesLength; i++) {
		nodes[i].~Node();
	}

	cudaFree(nodes);
	cudaFree(faces);
	cudaFree(cells);
}

template<MeshDim dim>
Mesh<dim>::Mesh(
		Node<dim>* nodes,
		const double& nodesLength,

		Face<dim>* faces,
		const double& facesLength,

		Cell<dim>* cells,
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
size_t Mesh<dim>::getMeshSize()
{
	size_t size = 0;
	for (int i = 0; i < nodesLength; i++) {
		size = size + sizeof(Node<dim>);
	}

	for (int i = 0; i < facesLength; i++) {
		size = size + sizeof(Face<dim>);
		//size = size + (faces[i].nodeIDsLength * sizeof(int));
	}

	for (int i = 0; i < cellsLength; i++) {
		size = size + sizeof(Cell<dim>);
		size = size + (cells[i].nodeIDsLength * sizeof(int));
		size = size + (cells[i].faceIDsLength * sizeof(int));
		size = size + (cells[i].neighbourCellsIDsLength * sizeof(int));
	}

	return size;
}

template class Mesh<MeshDim::D2>;
template class Mesh<MeshDim::D3>;