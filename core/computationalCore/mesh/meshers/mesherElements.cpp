#include "../include/mesh/meshers/mesherElements.h"

#include <iostream>

// ------------------------------------- NODE ----------------------------------------------

template<MeshDim dim>
MesherNode<dim>::MesherNode(const std::array<double, meshDimSize(dim)>& pos) : pos(pos) {};

template<MeshDim dim>
MesherNode<dim>::MesherNode(const Point<meshDimToGeometryDim(dim)>& point) : pos(point.pos) {};

template<MeshDim dim>
const Node<dim> MesherNode<dim>::getNode()
{
	return Node<dim>(this->pos);
}

template class MesherNode<MeshDim::D2>;
template class MesherNode<MeshDim::D3>;

// -------------------------------------- FACE --------------------------------------------

template<MeshDim dim>
const Face<dim> MesherFace<dim>::getFace()
{
	return Face<dim>(
		area,
		nodeIDs.data(),
		nodeIDs.size(),
		ownerCellID,
		ownerData,
		neighbourCellID,
		neighbourData,
		ownerToNeighbourCell
	);
}

template class MesherFace<MeshDim::D2>;
template class MesherFace<MeshDim::D3>;

// --------------------------------------- CELL -----------------------------------------

template<MeshDim dim>
const Cell<dim> MesherCell<dim>::getCell()
{
	return Cell<dim>(
		this->pointIDs.data(),
		this->pointIDs.size(),

		this->faceIDs.data(),
		this->faceIDs.size(),

		this->neighbourCellsIDs.data(),
		this->neighbourCellsIDs.size(),

		this->centroid,
		this->volume
	);
}

template class MesherCell<MeshDim::D2>;
template class MesherCell<MeshDim::D3>;

// --------------------------------------- MESH -----------------------------------------


template<MeshDim dim>
const Mesh<dim> MesherMesh<dim>::createMeshInHeap()
{

	std::cout << "Allocating in heap" << '\n';

	const int nodesSize = this->nodes.size();
	const int facesSize = this->faces.size();
	const int cellsSize = this->cells.size();

	// TODO: This solution is obviously to be improved \/

	Node<dim>* nodes = new Node<dim>[nodesSize];
	Face<dim>* faces = new Face<dim>[facesSize];
	Cell<dim>* cells = new Cell<dim>[cellsSize];

	for(int i = 0; i < nodesSize; i++) {
		nodes[i] = this->nodes[i].getNode();
	}

	for(int i = 0; i < facesSize; i++) {
		faces[i] = this->faces[i].getFace();
	}

	for(int i = 0; i < cellsSize; i++) {
		cells[i] = this->cells[i].getCell();
	}

	std::cout << "Finished allocating in heap" << '\n';

	return Mesh<dim>(
		nodes,
		nodesSize,
		faces,
		facesSize,
		cells,
		cellsSize
	);
}

template class MesherMesh<MeshDim::D2>;
template class MesherMesh<MeshDim::D3>;