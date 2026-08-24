#pragma once

#include "../meshEnums.h"
#include <geometry/vector.h>
#include <geometry/point.h>
#include "supportStructs.h"

#include <vector>
#include <optional>
#include <utility>

template<MeshDim dim>
class Face {
protected:
	using V = VectorData<meshDimToGeometryDim(dim)>;
	using P = Point<meshDimToGeometryDim(dim)>;
public:

	Face(
		const V& area,
		int nodeIDsLength,
		const int* nodeIDs,
		int ownerCellID,
		const CellData<meshDimToGeometryDim(dim)>& ownerData,
		std::optional<int> neighbourCellID = std::nullopt,
		std::optional<CellData<meshDimToGeometryDim(dim)>> neighbourData = std::nullopt,
		std::optional<V> ownerToNeighbourCell = std::nullopt
	);

	Face();

	// Rule of 5

	Face(const Face<dim>& other);
	Face(Face<dim>&& other) noexcept;

	Face<dim>& operator=(const Face<dim>& other);
	Face<dim>& operator=(Face<dim>&& other) noexcept;

	~Face();

	V area;

	int* nodeIDs = nullptr;
	int nodeIDsLength = 0;

	int ownerCellID;
	CellData<meshDimToGeometryDim(dim)> ownerData;

	std::optional<int> neighbourCellID = std::nullopt;
	std::optional<CellData<meshDimToGeometryDim(dim)>> neighbourData = std::nullopt;
	std::optional<V> ownerToNeighbourCell = std::nullopt;

	bool isBoundary() const;
};

// TODO: Implement Boundary Face recognision.