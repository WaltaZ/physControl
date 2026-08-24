#include "../include/mesh/meshers/mesherElements.h"

#include <iostream>
#include <utility/cudaConfig.h>

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
		static_cast<int>(nodeIDs.size()),
		nodeIDs.data(),
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
		pointIDs.data(),
		pointIDs.size(),

		faceIDs.data(),
		faceIDs.size(),

		neighbourCellsIDs.data(),
		neighbourCellsIDs.size(),

		centroid,
		volume
	);
}

template class MesherCell<MeshDim::D2>;
template class MesherCell<MeshDim::D3>;

// --------------------------------------- MESH -----------------------------------------


template<MeshDim dim>
const Mesh<dim> MesherMesh<dim>::allocateMesh()
{

	std::cout << "Allocating in heap" << '\n';

	const int nodesLength = this->nodes.size();
	const int facesLength = this->faces.size();
	const int cellsLength = this->cells.size();

	Node<dim>* heapNodes;
	cudaMallocManaged(&heapNodes, sizeof(Node<dim>) * nodesLength);
	//cudaMemAdvise(heapNodes, sizeof(Node<dim>) * nodesLength, cudaMemAdviseSetPreferredLocation, cudaConfig::hostLocation);
	for (int i = 0; i < nodesLength; i++) {
		heapNodes[i] = this->nodes[i].getNode();
	}
	/*
	Face<dim>* heapFaces;
	cudaMallocManaged(&heapFaces, sizeof(Face<dim>) * facesLength);
	//cudaMemAdvise(heapFaces, sizeof(Face<dim>) * facesLength, cudaMemAdviseSetPreferredLocation, cudaConfig::hostLocation);
	for (int i = 0; i < facesLength; i++) {
		heapFaces[i] = this->faces[i].getFace();
	}
	
	Cell<dim>* heapCells;
	cudaMallocManaged(&heapCells, sizeof(Cell<dim>) * cellsLength);
	//cudaMemAdvise(heapCells, sizeof(Cell<dim>) * cellsLength, cudaMemAdviseSetPreferredLocation, cudaConfig::hostLocation);
	for (int i = 0; i < cellsLength; i++) {
		heapCells[i] = this->cells[i].getCell();
	}*/

	std::cout << "sizeof(Node): " << sizeof(Node<dim>) << '\n';
	std::cout << "sizeof(Face): " << sizeof(Face<dim>) << '\n';
	std::cout << "sizeof(Cell): " << sizeof(Cell<dim>) << '\n';

	std::cout << "nodes allocation: "
		<< sizeof(Node<dim>) * nodesLength
		<< " bytes\n";

	return Mesh<dim>(
		heapNodes,
		nodesLength,
		nullptr,
		0,
		nullptr,
		0
	);
}

template class MesherMesh<MeshDim::D2>;
template class MesherMesh<MeshDim::D3>;