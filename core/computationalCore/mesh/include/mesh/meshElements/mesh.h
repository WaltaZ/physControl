#pragma once

#include "../meshEnums.h"
#include "node.h"
#include "face.h"
#include "cell.h"

#include "../meshers/mesherElements.h"
#include <algorithm>
#include <array>
#include <string_view>

#include <fileStream/path.h>
#include <fileStream/binaryFiles/binStream.h>

struct MeshElementsIDs {
	CudaPackedArray<uint32_t> faceNodeIDs;

	CudaPackedArray<uint32_t> cellNodeIDs;
	CudaPackedArray<uint32_t> cellFaceIDs;
	CudaPackedArray<uint32_t> cellNeighbourCells;
};

template<MeshDim dim>
class Mesh {
	static const std::string MESH_CACHE;
public:
	CudaPackedArray<Node<dim>> nodes;
	CudaPackedArray<Face<dim>> faces;
	CudaPackedArray<Cell<dim>> cells;

	MeshElementsIDs elementsIDs;

	Mesh() = default;
	
	Mesh(const MesherMesh<dim>& mesherMesh);

	__device__ __host__
	uint32_t getCommonFaceId(
		const Cell<dim>& C,
		const uint32_t C_neighbourCellIndex) const;

	void save(const std::string& path = MESH_CACHE);
	static Mesh<dim>* load(const std::string& path = MESH_CACHE);
};