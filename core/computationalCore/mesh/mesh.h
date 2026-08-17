#pragma once

#include "meshDependencies.h"

#include "meshEnums.h"
#include "meshElements/meshElements.h"

template<MeshDim dim>
class Mesh {
public:
	std::vector<Node<dim>> nodes{};
	std::vector<Face<dim>> faces{};
	std::vector<Cell<dim>> cells{};

	Mesh() {};
};

#include "meshers/mesher.h"