#pragma once

#include "../meshEnums.h"
#include "node.h"
#include "face.h"
#include "cell.h"

template<MeshDim dim>
class Mesh {
public:
	const Node<dim>* nodes;
	const double nodesLength;

	const Face<dim>* faces;
	const double facesLength;

	const Cell<dim>* cells;
	const double cellsLength;

	Mesh(
		const std::vector<Node<dim>>* nodes,
		const std::vector<Face<dim>>* faces,
		const std::vector<Cell<dim>>* cells);
	
	Mesh(
		const Node<dim>* nodes,
		const double& nodesLength,

		const Face<dim>* faces,
		const double& facesLength,

		const Cell<dim>* cells,
		const double& cellsLength);

	Mesh<dim> getMeshFromNewPointers(const Node<dim>* nodes, const Face<dim>* faces, const Cell<dim>* cells);
};