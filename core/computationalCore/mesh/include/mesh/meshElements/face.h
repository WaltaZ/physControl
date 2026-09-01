#pragma once

#include "../meshEnums.h"
#include "supportStructs.h"
#include "../meshers/mesherElements.h"

#include <optional>
#include <utility/cudaUtils.h>

template<MeshDim dim>
class Face {
protected:
	using V = VectorData<mesh2geom(dim)>;
	using P = Point<mesh2geom(dim)>;
public:

	Face(
		const MesherFace<dim>& mesherFace,
		const CudaArray<uint32_t>& faceNodeIDs
	);

	V area;
	P centroid;

	bool isBoundary = false;

	CudaArray<uint32_t> faceNodeIDs;

	uint32_t ownerCellID;
	CellData<mesh2geom(dim)> ownerData;

	// --------------------------- Only valid for inner faces:
	double ownerFaceWeightFactor;


	uint32_t neighbourCellID;
	CellData<mesh2geom(dim)> neighbourData;

	V ownerToNeighbourCell;

	__host__ __device__
	const CellData<mesh2geom(dim)>& getCellData(uint32_t id) const;

	__host__ __device__
	double getWeightFactor(uint32_t id) const;

	__host__ __device__
	uint32_t getNeighbourCellID(uint32_t id) const;

	__host__ __device__
	VectorData<mesh2geom(dim)> getArea(uint32_t id) const;

};