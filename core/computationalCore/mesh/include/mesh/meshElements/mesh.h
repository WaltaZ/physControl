#pragma once

#include "../meshEnums.h"
#include "node.h"
#include "face.h"
#include "cell.h"

#include "../meshers/mesherElements.h"
#include <algorithm>

struct MeshElementsIDs {
	CudaAllocatedObj<uint32_t> faceNodeIDs;

	CudaAllocatedObj<uint32_t> cellNodeIDs;
	CudaAllocatedObj<uint32_t> cellFaceIDs;
	CudaAllocatedObj<uint32_t> cellNeighbourCells;

	~MeshElementsIDs();
};

template<MeshDim dim>
class CudaMesh {
public:
	CudaAllocatedObj<Node<dim>> nodes;
	CudaAllocatedObj<Face<dim>> faces;
	CudaAllocatedObj<Cell<dim>> cells;

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