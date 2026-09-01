#pragma once

#include "../meshEnums.h"
#include "supportStructs.h"
#include "../meshers/mesherElements.h"

#include <vector>
#include <utility/cudaUtils.h>

template<MeshDim dim>
class Cell {
private:
	using P = Point<mesh2geom(dim)>;
	using V = VectorData<mesh2geom(dim)>;
public:

    Cell(
        const MesherCell<dim>& mesherCell,
        const CudaArray<uint32_t>& cellNodeIDs,
        const CudaArray<uint32_t>& cellFaceIDs,
        const CudaArray<uint32_t>& cellNeighbourCells
    );
    
    CudaArray<uint32_t> cellNodeIDs;
    CudaArray<uint32_t> cellFaceIDs;
    CudaArray<uint32_t> cellNeighbourCells;

    P centroid;
    double volume;
};