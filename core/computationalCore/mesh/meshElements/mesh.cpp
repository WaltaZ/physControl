#include "../include/mesh/meshElements/mesh.h"

#include <cuda_runtime.h>

template<MeshDim dim>
CudaMesh<dim>::CudaMesh(const MesherMesh<dim>& mesherMesh) {
	// Allocating data into CPU and GPU

	nodes.length = mesherMesh.nodes.size();
	faces.length = mesherMesh.faces.size();
	cells.length = mesherMesh.cells.size();

	// Nodes --------------------------------------------------------------
	cudaMallocManaged(nodes.getDataPointer(), nodes.length * sizeof(Node<dim>));
	for (int i = 0; i < nodes.length; i++) {
		nodes[i] = Node<dim>(mesherMesh.nodes[i]);
	}

	// Faces --------------------------------------------------------------
	std::vector<uint32_t> faceNodeIDs;

	cudaMallocManaged(faces.getDataPointer(), faces.length * sizeof(Face<dim>));
	for (int i = 0; i < faces.length; i++) {
		faces[i] = Face<dim>(
			mesherMesh.faces[i],
			CudaArray<uint32_t>(
				elementsIDs.faceNodeIDs.getDataPointer(),
				static_cast<uint32_t>(faceNodeIDs.size()),
				static_cast<uint32_t>(mesherMesh.faces[i].nodeIDs.size())
			)
		);
		faceNodeIDs.insert(faceNodeIDs.end(), mesherMesh.faces[i].nodeIDs.begin(), mesherMesh.faces[i].nodeIDs.end());
	}

	elementsIDs.faceNodeIDs.length = faceNodeIDs.size();
	cudaMallocManaged(elementsIDs.faceNodeIDs.getDataPointer(), faceNodeIDs.size() * sizeof(uint32_t));
	for (int i = 0; i < faceNodeIDs.size(); i++) {
		elementsIDs.faceNodeIDs[i] = faceNodeIDs[i];
	}

	// Cells --------------------------------------------------------------
 	std::vector<uint32_t> cellNodeIDs;
	std::vector<uint32_t> cellFaceIDs;
	std::vector<uint32_t> cellNeighbourCells;

	cudaMallocManaged(cells.getDataPointer(), cells.length * sizeof(Cell<dim>));
	for (int i = 0; i < cells.length; i++) {
		cells[i] = Cell<dim>(
			mesherMesh.cells[i],
			CudaArray<uint32_t>(
				elementsIDs.cellNodeIDs.getDataPointer(),
				static_cast<uint32_t>(cellNodeIDs.size()),
				static_cast<uint32_t>(mesherMesh.cells[i].nodeIDs.size())
			),
			CudaArray<uint32_t>(
				elementsIDs.cellFaceIDs.getDataPointer(),
				static_cast<uint32_t>(cellFaceIDs.size()),
				static_cast<uint32_t>(mesherMesh.cells[i].faceIDs.size())
			),
			CudaArray<uint32_t>(
				elementsIDs.cellNeighbourCells.getDataPointer(),
				static_cast<uint32_t>(cellNeighbourCells.size()),
				static_cast<uint32_t>(mesherMesh.cells[i].neighbourCellsIDs.size())
			)
		);
		cellNodeIDs.insert(cellNodeIDs.end(), mesherMesh.cells[i].nodeIDs.begin(), mesherMesh.cells[i].nodeIDs.end());
		cellFaceIDs.insert(cellFaceIDs.end(), mesherMesh.cells[i].faceIDs.begin(), mesherMesh.cells[i].faceIDs.end());
		cellNeighbourCells.insert(cellNeighbourCells.end(), mesherMesh.cells[i].neighbourCellsIDs.begin(), mesherMesh.cells[i].neighbourCellsIDs.end());
	}

	elementsIDs.cellNodeIDs.length = cellNodeIDs.size();
	cudaMallocManaged(elementsIDs.cellNodeIDs.getDataPointer(), cellNodeIDs.size() * sizeof(uint32_t));
	for (int i = 0; i < cellNodeIDs.size(); i++) {
		elementsIDs.cellNodeIDs[i] = cellNodeIDs[i];
	}

	elementsIDs.cellFaceIDs.length = cellNodeIDs.size();
	cudaMallocManaged(elementsIDs.cellFaceIDs.getDataPointer(), cellFaceIDs.size() * sizeof(uint32_t));
	for (int i = 0; i < cellFaceIDs.size(); i++) {
		elementsIDs.cellFaceIDs[i] = cellFaceIDs[i];
	}

	elementsIDs.cellNeighbourCells.length = cellNeighbourCells.size();
	cudaMallocManaged(elementsIDs.cellNeighbourCells.getDataPointer(), cellNeighbourCells.size() * sizeof(uint32_t));
	for (int i = 0; i < cellNeighbourCells.size(); i++) {
		elementsIDs.cellNeighbourCells[i] = cellNeighbourCells[i];
	}
}

template<MeshDim dim>
CudaMesh<dim>::~CudaMesh() {
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

template class CudaMesh<MeshDim::D2>;
template class CudaMesh<MeshDim::D3>;

template<MeshDim dim>
Mesh<dim>::Mesh(const MesherMesh<dim>& mesherMesh)
{
	cudaMallocManaged(&_mesh, sizeof(CudaMesh<dim>));
	new(_mesh) CudaMesh<dim>(mesherMesh);
}

template<MeshDim dim>
CudaMesh<dim>* Mesh<dim>::getElements()
{
	return _mesh;
}

template<MeshDim dim>
const CudaMesh<dim>* Mesh<dim>::getElements() const
{
	return _mesh;
}

template class Mesh<MeshDim::D2>;
template class Mesh<MeshDim::D3>;