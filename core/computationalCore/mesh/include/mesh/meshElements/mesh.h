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
class CudaMesh {
public:
	CudaPackedArray<Node<dim>> nodes;
	CudaPackedArray<Face<dim>> faces;
	CudaPackedArray<Cell<dim>> cells;

	MeshElementsIDs elementsIDs;

	~CudaMesh();
	
	CudaMesh(const MesherMesh<dim>& mesherMesh);

	//size_t getMeshSize();
};

template<MeshDim dim>
class Mesh {
	CudaMesh<dim>* _mesh;

public:
	Mesh(const MesherMesh<dim>& mesherMesh);

	CudaMesh<dim>* getElements();

	const CudaMesh<dim>* getElements() const;
};