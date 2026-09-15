#pragma once

#include "../meshEnums.h"
#include "node.h"
#include "face.h"
#include "cell.h"

#include "../meshers/mesherElements.h"
#include <algorithm>

struct MeshElementsIDs {
	CudaPackedArray<uint32_t> faceNodeIDs;

	CudaPackedArray<uint32_t> cellNodeIDs;
	CudaPackedArray<uint32_t> cellFaceIDs;
	CudaPackedArray<uint32_t> cellNeighbourCells;
};

template<MeshDim dim>
class Mesh {
public:
	CudaPackedArray<Node<dim>> nodes;
	CudaPackedArray<Face<dim>> faces;
	CudaPackedArray<Cell<dim>> cells;

	MeshElementsIDs elementsIDs;
	
	Mesh(const MesherMesh<dim>& mesherMesh);

	__device__
	Face<dim>* getCommonFace(const uint32_t C_id, const uint32_t F_id);

	//size_t getMeshSize();
};