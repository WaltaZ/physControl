#include <mesh/meshElements/mesh.h>
#include <utility/logger.h>

template<MeshDim dim>
const std::string Mesh<dim>::MESH_CACHE = path::parse({ path::CACHE_DIR, "mesh" });

const std::string f_MESH = "mesh.bin";
const std::string f_NODES = "nodes.bin";
const std::string f_FACES = "faces.bin";
const std::string f_CELLS = "cells.bin";
const std::string f_ELEMENTS_MAPPING = "elementsMapping.bin";

namespace meshFileKernel {
	template<MeshDim dim>
	__global__
		void setFacePointers(
			CudaPackedArray<Face<dim>>& faces,
			uint32_t* faceNodeIDs)
	{
		int i = blockDim.x * blockIdx.x + threadIdx.x;

		if (i > faces.length) { return; }

		faces[i].faceNodeIDs.setDataSource(faceNodeIDs);
	}

	template<MeshDim dim>
	__global__
		void setCellPointers(
			CudaPackedArray<Cell<dim>>& cell,
			uint32_t* cellNodeIDs,
			uint32_t* cellFaceIDs,
			uint32_t* cellNeighbourCells
		)
	{
		int i = blockDim.x * blockIdx.x + threadIdx.x;

		if (i > cell.length) { return; }

		cell[i].cellNodeIDs.setDataSource(cellNodeIDs);
		cell[i].cellFaceIDs.setDataSource(cellFaceIDs);
		cell[i].cellNeighbourCells.setDataSource(cellNeighbourCells);
	}
}

template <MeshDim dim>
void Mesh<dim>::save(const std::string& path)
{
	log("[MESH] Started saving the mesh");

	BinWriter f_mesh({ path, f_MESH });
	f_mesh.write(this);

	BinWriter f_nodes({ path, f_NODES });
	f_nodes.writeArray(nodes.getData(), nodes.length);

	BinWriter f_faces({ path, f_FACES });
	f_faces.writeArray(faces.getData(), faces.length);

	BinWriter f_cells({ path, f_CELLS });
	f_cells.writeArray(cells.getData(), cells.length);

	BinWriter f_elementsMapping({ path, f_ELEMENTS_MAPPING });
	f_elementsMapping.writeArray(elementsIDs.faceNodeIDs.getData(), elementsIDs.faceNodeIDs.length);
	f_elementsMapping.writeArray(elementsIDs.cellNodeIDs.getData(), elementsIDs.cellNodeIDs.length);
	f_elementsMapping.writeArray(elementsIDs.cellFaceIDs.getData(), elementsIDs.cellFaceIDs.length);
	f_elementsMapping.writeArray(elementsIDs.cellNeighbourCells.getData(), elementsIDs.cellNeighbourCells.length);

	log("[MESH] Finished saving the mesh");
};

template <MeshDim dim>
Mesh<dim>* Mesh<dim>::load(const std::string& path)
{
	log("[MESH] Started loading the mesh");

	// Mesh -----------------------------------------------------

	BinReader f_mesh({ path, f_MESH });

	Mesh<dim>* mesh = nullptr;
	cudaMallocManaged(&mesh, sizeof(Mesh<dim>));
	f_mesh.read(mesh);

	// Elements mapping -----------------------------------------

	BinReader f_elementsMapping({ path, f_ELEMENTS_MAPPING });

	uint32_t* faceNodeIDs = nullptr;
	cudaMallocManaged(
		&faceNodeIDs, 
		sizeof(uint32_t) * mesh->elementsIDs.faceNodeIDs.length);
	mesh->elementsIDs.faceNodeIDs.setDataPointer(faceNodeIDs);
	f_elementsMapping.readArray(
		faceNodeIDs, mesh->elementsIDs.faceNodeIDs.length);

	uint32_t* cellNodeIDs = nullptr;
	cudaMallocManaged(
		&cellNodeIDs,
		sizeof(uint32_t) * mesh->elementsIDs.cellNodeIDs.length);
	mesh->elementsIDs.cellNodeIDs.setDataPointer(cellNodeIDs);
	f_elementsMapping.readArray(
		cellNodeIDs, mesh->elementsIDs.cellNodeIDs.length);

	uint32_t* cellFaceIDs = nullptr;
	cudaMallocManaged(
		&cellFaceIDs,
		sizeof(uint32_t) * mesh->elementsIDs.cellFaceIDs.length);
	mesh->elementsIDs.cellFaceIDs.setDataPointer(cellFaceIDs);
	f_elementsMapping.readArray(
		cellFaceIDs, mesh->elementsIDs.cellFaceIDs.length);

	uint32_t* cellNeighbourCells = nullptr;
	cudaMallocManaged(
		&cellNeighbourCells,
		sizeof(uint32_t) * mesh->elementsIDs.cellNeighbourCells.length);
	mesh->elementsIDs.cellNeighbourCells.setDataPointer(cellNeighbourCells);
	f_elementsMapping.readArray(
		cellNeighbourCells, mesh->elementsIDs.cellNeighbourCells.length);

	// NODES --------------------------------------------------

	BinReader f_nodes({ path, f_NODES });

	Node<dim>* nodes = nullptr;
	cudaMallocManaged(
		&nodes,
		sizeof(Node<dim>) * mesh->nodes.length
	);
	mesh->nodes.setDataPointer(nodes);
	f_nodes.readArray(nodes, mesh->nodes.length);

	// FACES --------------------------------------------------

	BinReader f_faces({ path, f_FACES });

	Face<dim>* faces = nullptr;
	cudaMallocManaged(
		&faces,
		sizeof(Face<dim>) * mesh->faces.length
	);
	mesh->faces.setDataPointer(faces);
	f_faces.readArray(faces, mesh->faces.length);

	KernelArgs faceArgs = cudaUtils::getKernelArgs(mesh->faces.length);
	meshFileKernel::setFacePointers <<<faceArgs.blocks, faceArgs.threads >>>(
		mesh->faces,
		faceNodeIDs);

	cudaUtils::fetchError(cudaDeviceSynchronize);

	// CELLS --------------------------------------------------

	BinReader f_cells({ path, f_CELLS });

	Cell<dim>* cells = nullptr;
	cudaMallocManaged(
		&cells,
		sizeof(Cell<dim>) * mesh->cells.length
	);
	mesh->cells.setDataPointer(cells);
	f_cells.readArray(cells, mesh->cells.length);

	KernelArgs cellArgs = cudaUtils::getKernelArgs(mesh->cells.length);
	meshFileKernel::setCellPointers <<<cellArgs.blocks, cellArgs.threads >>> (
		mesh->cells,
		cellNodeIDs,
		cellFaceIDs,
		cellNeighbourCells);

	cudaUtils::fetchError(cudaDeviceSynchronize);

	log("[MESH] Finished loading the mesh");

	return mesh;
}

template class Mesh<MeshDim::D2>;
template class Mesh<MeshDim::D3>;