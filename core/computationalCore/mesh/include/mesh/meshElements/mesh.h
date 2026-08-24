#pragma once

#include "../meshEnums.h"
#include "node.h"
#include "face.h"
#include "cell.h"

template<MeshDim dim>
class Mesh {
public:
	Node<dim>* nodes;
	double nodesLength;

	Face<dim>* faces;
	double facesLength;

	Cell<dim>* cells;
	double cellsLength;

	~Mesh();
	
	Mesh(
		Node<dim>* nodes,
		const double& nodesLength,

		Face<dim>* faces,
		const double& facesLength,

		Cell<dim>* cells,
		const double& cellsLength);

	size_t getMeshSize();
};