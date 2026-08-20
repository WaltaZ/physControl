#pragma once

#include "../meshEnums.h"
#include <geometry/vector.h>
#include <geometry/point.h>
#include "supportStructs.h"

#include <vector>
#include <optional>

template<MeshDim dim>
class Face {
protected:
	using V = VectorData<meshDimToGeometryDim(dim)>;
	using P = Point<meshDimToGeometryDim(dim)>;
public:

	Face(
		const V& area,
		int* nodeIDs,
		int nodeIDsLength,
		int ownerCellID,
		const CellData<meshDimToGeometryDim(dim)>& ownerData,
		std::optional<int> neighbourCellID = std::nullopt,
		std::optional<CellData<meshDimToGeometryDim(dim)>> neighbourData = std::nullopt,
		std::optional<V> ownerToNeighbourCell = std::nullopt
	);

	Face();

	~Face();

	V area;

	int* nodeIDs;
	int nodeIDsLength;

	int ownerCellID;
	CellData<meshDimToGeometryDim(dim)> ownerData;

	std::optional<int> neighbourCellID = std::nullopt;
	std::optional<CellData<meshDimToGeometryDim(dim)>> neighbourData = std::nullopt;
	std::optional<V> ownerToNeighbourCell = std::nullopt;

	bool isBoundary() const;
};

// TODO: Implement Boundary Face recognision.