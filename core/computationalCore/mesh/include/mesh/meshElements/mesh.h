#pragma once

#include "../meshEnums.h"
#include "node.h"
#include "face.h"
#include "cell.h"

#include "../meshers/mesherElements.h"
#include <algorithm>

struct MeshElementsIDs {
	uint32_t* faceNodeIDs = nullptr;

	uint32_t* cellNodeIDs = nullptr;
	uint32_t* cellFaceIDs = nullptr;
	uint32_t* cellNeighbourCells = nullptr;

	~MeshElementsIDs();
};

template<MeshDim dim>
class Mesh {
public:
	Node<dim>* nodes = nullptr;
	uint32_t nodesLength = 0;

	Face<dim>* faces = nullptr;
	uint32_t facesLength = 0;

	Cell<dim>* cells = nullptr;
	uint32_t cellsLength = 0;

	MeshElementsIDs elementsIDs;

	~Mesh();
	
	Mesh(const MesherMesh<dim>& mesherMesh);

	//size_t getMeshSize();
};