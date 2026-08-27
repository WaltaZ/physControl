#pragma once

#include "../meshEnums.h"
#include "supportStructs.h"
#include "../meshers/mesherElements.h"

#include <optional>
#include <utility/cudaUtils.h>

template<MeshDim dim>
class Face {
protected:
	using V = VectorData<meshDimToGeometryDim(dim)>;
	using P = Point<meshDimToGeometryDim(dim)>;
public:

	Face(
		const MesherFace<dim>& mesherFace,
		const CudaArray<uint32_t>& faceNodeIDs
	);

	V area;

	CudaArray<uint32_t> faceNodeIDs;

	uint32_t ownerCellID;
	CellData<meshDimToGeometryDim(dim)> ownerData;

	std::optional<int> neighbourCellID = std::nullopt;
	std::optional<CellData<meshDimToGeometryDim(dim)>> neighbourData = std::nullopt;

	std::optional<V> ownerToNeighbourCell = std::nullopt;

	bool isBoundary() const;
};

// TODO: Implement Boundary Face recognision.