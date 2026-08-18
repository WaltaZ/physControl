#pragma once

#include "../meshEnums.h"
#include "node.h"
#include "face.h"
#include "cell.h"

template<MeshDim dim>
class Mesh {
public:
	std::vector<Node<dim>> nodes{};
	std::vector<Face<dim>> faces{};
	std::vector<Cell<dim>> cells{};

	Mesh();
};