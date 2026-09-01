#pragma once

#include "../meshEnums.h"
#include "../meshers/mesherElements.h"

#include <array>
#include <geometry/point.h>

template <MeshDim dim>
struct Node {

	std::array<double, meshDimSize(dim)> pos{};

	Node(const std::array<double, meshDimSize(dim)>& pos);
	Node(const Point<mesh2geom(dim)>& point);
	Node(const MesherNode<dim>& mesherNode);
	Node();
};