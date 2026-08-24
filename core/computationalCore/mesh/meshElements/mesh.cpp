#include "../include/mesh/meshElements/mesh.h"

#include <cuda_runtime.h>

template<MeshDim dim>
Mesh<dim>::Mesh(const MesherMesh<dim>& mesherMesh) {
	// Allocating data into CPU and GPU

	this->nodesLength = mesherMesh.nodes.size();
	this->facesLength = mesherMesh.faces.size();
	this->cellsLength = mesherMesh.cells.size();

	// Nodes --------------------------------------------------------------
	cudaMallocManaged(&nodes, nodesLength * sizeof(Node<dim>));
	for (int i = 0; i < nodesLength; i++) {
		nodes[i] = Node<dim>(mesherMesh.nodes[i]);
	}

	// Faces --------------------------------------------------------------
	std::vector<uint32_t> faceNodeIDs;

	cudaMallocManaged(&faces, facesLength * sizeof(Face<dim>));
	for (int i = 0; i < facesLength; i++) {
		faces[i] = Face<dim>(
			mesherMesh.faces[i],
			{
				static_cast<uint32_t>(faceNodeIDs.size()),
				static_cast<uint32_t>(mesherMesh.faces[i].nodeIDs.size())
			}
		);
		faceNodeIDs.insert(faceNodeIDs.end(), mesherMesh.faces[i].nodeIDs.begin(), mesherMesh.faces[i].nodeIDs.end());
	}

	cudaMallocManaged(&(elementsIDs.faceNodeIDs), faceNodeIDs.size() * sizeof(uint32_t));
	for (int i = 0; i < faceNodeIDs.size(); i++) {
		elementsIDs.faceNodeIDs[i] = faceNodeIDs[i];
	}

	// Cells --------------------------------------------------------------
 	std::vector<uint32_t> cellNodeIDs;
	std::vector<uint32_t> cellFaceIDs;
	std::vector<uint32_t> cellNeighbourCells;

	cudaMallocManaged(&cells, cellsLength * sizeof(Cell<dim>));
	for (int i = 0; i < cellsLength; i++) {
		cells[i] = Cell<dim>(
			mesherMesh.cells[i],
			{
				static_cast<uint32_t>(cellNodeIDs.size()),
				static_cast<uint32_t>(mesherMesh.cells[i].nodeIDs.size())
			},
			{
				static_cast<uint32_t>(cellFaceIDs.size()),
				static_cast<uint32_t>(mesherMesh.cells[i].faceIDs.size())
			},
			{
				static_cast<uint32_t>(cellNeighbourCells.size()),
				static_cast<uint32_t>(mesherMesh.cells[i].neighbourCellsIDs.size())
			}
		);
		cellNodeIDs.insert(cellNodeIDs.end(), mesherMesh.cells[i].nodeIDs.begin(), mesherMesh.cells[i].nodeIDs.end());
		cellFaceIDs.insert(cellFaceIDs.end(), mesherMesh.cells[i].faceIDs.begin(), mesherMesh.cells[i].faceIDs.end());
		cellNeighbourCells.insert(cellNeighbourCells.end(), mesherMesh.cells[i].neighbourCellsIDs.begin(), mesherMesh.cells[i].neighbourCellsIDs.end());
	}

	cudaMallocManaged(&(elementsIDs.cellNodeIDs), cellNodeIDs.size() * sizeof(uint32_t));
	for (int i = 0; i < cellNodeIDs.size(); i++) {
		elementsIDs.cellNodeIDs[i] = cellNodeIDs[i];
	}

	cudaMallocManaged(&(elementsIDs.cellFaceIDs), cellFaceIDs.size() * sizeof(uint32_t));
	for (int i = 0; i < cellFaceIDs.size(); i++) {
		elementsIDs.cellFaceIDs[i] = cellFaceIDs[i];
	}

	cudaMallocManaged(&(elementsIDs.cellNeighbourCells), cellNeighbourCells.size() * sizeof(uint32_t));
	for (int i = 0; i < cellNeighbourCells.size(); i++) {
		elementsIDs.cellNeighbourCells[i] = cellNeighbourCells[i];
	}
}

template<MeshDim dim>
Mesh<dim>::~Mesh() {
	cudaFree(nodes);
	cudaFree(faces);
	cudaFree(cells);
};

MeshElementsIDs::~MeshElementsIDs()
{
	cudaFree(faceNodeIDs);

	cudaFree(cellNodeIDs);
	cudaFree(cellFaceIDs);
	cudaFree(cellNeighbourCells);
}

template class Mesh<MeshDim::D2>;
template class Mesh<MeshDim::D3>;