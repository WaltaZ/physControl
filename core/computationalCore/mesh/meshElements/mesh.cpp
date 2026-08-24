#include "../include/mesh/meshElements/mesh.h"

#include <cuda_runtime.h>

template<MeshDim dim>
Mesh<dim>::Mesh(const MesherMesh<dim>& mesherMesh) {
	// Allocating data into CPU and GPU

	nodes.length = mesherMesh.nodes.size();
	faces.length = mesherMesh.faces.size();
	cells.length = mesherMesh.cells.size();

	// Nodes --------------------------------------------------------------
	cudaMallocManaged(&nodes.data, nodes.length * sizeof(Node<dim>));
	for (int i = 0; i < nodes.length; i++) {
		nodes.data[i] = Node<dim>(mesherMesh.nodes[i]);
	}

	// Faces --------------------------------------------------------------
	std::vector<uint32_t> faceNodeIDs;

	cudaMallocManaged(&faces.data, faces.length * sizeof(Face<dim>));
	for (int i = 0; i < faces.length; i++) {
		faces.data[i] = Face<dim>(
			mesherMesh.faces[i],
			{
				static_cast<uint32_t>(faceNodeIDs.size()),
				static_cast<uint32_t>(mesherMesh.faces[i].nodeIDs.size())
			}
		);
		faceNodeIDs.insert(faceNodeIDs.end(), mesherMesh.faces[i].nodeIDs.begin(), mesherMesh.faces[i].nodeIDs.end());
	}

	elementsIDs.faceNodeIDs.length = faceNodeIDs.size();
	cudaMallocManaged(&(elementsIDs.faceNodeIDs.data), faceNodeIDs.size() * sizeof(uint32_t));
	for (int i = 0; i < faceNodeIDs.size(); i++) {
		elementsIDs.faceNodeIDs.data[i] = faceNodeIDs[i];
	}

	// Cells --------------------------------------------------------------
 	std::vector<uint32_t> cellNodeIDs;
	std::vector<uint32_t> cellFaceIDs;
	std::vector<uint32_t> cellNeighbourCells;

	cudaMallocManaged(&cells.data, cells.length * sizeof(Cell<dim>));
	for (int i = 0; i < cells.length; i++) {
		cells.data[i] = Cell<dim>(
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

	elementsIDs.cellNodeIDs.length = cellNodeIDs.size();
	cudaMallocManaged(&(elementsIDs.cellNodeIDs.data), cellNodeIDs.size() * sizeof(uint32_t));
	for (int i = 0; i < cellNodeIDs.size(); i++) {
		elementsIDs.cellNodeIDs.data[i] = cellNodeIDs[i];
	}

	elementsIDs.cellFaceIDs.length = cellNodeIDs.size();
	cudaMallocManaged(&(elementsIDs.cellFaceIDs.data), cellFaceIDs.size() * sizeof(uint32_t));
	for (int i = 0; i < cellFaceIDs.size(); i++) {
		elementsIDs.cellFaceIDs.data[i] = cellFaceIDs[i];
	}

	elementsIDs.cellNeighbourCells.length = cellNeighbourCells.size();
	cudaMallocManaged(&(elementsIDs.cellNeighbourCells.data), cellNeighbourCells.size() * sizeof(uint32_t));
	for (int i = 0; i < cellNeighbourCells.size(); i++) {
		elementsIDs.cellNeighbourCells.data[i] = cellNeighbourCells[i];
	}
}

template<MeshDim dim>
Mesh<dim>::~Mesh() {
	/*cudaFree(nodes.data);
	cudaFree(faces.data);
	cudaFree(cells.data);*/
};

MeshElementsIDs::~MeshElementsIDs()
{
	/*cudaFree(faceNodeIDs.data);

	cudaFree(cellNodeIDs.data);
	cudaFree(cellFaceIDs.data);
	cudaFree(cellNeighbourCells.data);*/
}

template class Mesh<MeshDim::D2>;
template class Mesh<MeshDim::D3>;