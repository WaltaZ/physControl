#pragma once

#include "../meshEnums.h"
#include <geometry/vector.h>
#include <geometry/point.h>

#include <vector>
#include <optional>

template<MeshDim dim>
class Face {
protected:
	using V = Vector<meshDimToGeometryDim(dim)>;
	using P = Point<meshDimToGeometryDim(dim)>;
public:
	std::vector<int> nodeIDs;
	int ownerCellID;
	std::optional<int> neighbourCellID;

	double area;
	V normalVector;
	P centroid;

	Face(
		double area,
		const V& normalVector,
		const P& centroid,
		const std::vector<int>& nodeIDs,
		int ownerCellID,
		std::optional<int> neighbourCellID = std::nullopt
	);

	bool isBoundary() const;
};

// TODO: Implement Boundary Face recognision.