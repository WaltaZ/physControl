#pragma once

#include "../meshEnums.h"
#include "supportStructs.h"
#include "../meshers/mesherElements.h"

#include <vector>
#include <utility/cudaUtils.h>

template<MeshDim dim>
class Cell {
private:
	using P = Point<meshDimToGeometryDim(dim)>;
	using V = VectorData<meshDimToGeometryDim(dim)>;
public:

    Cell(
        const MesherCell<dim>& mesherCell,
        const CudaArray& cellNodeIDs,
        const CudaArray& cellFaceIDs,
        const CudaArray& cellNeighbourCells
    );

    Cell();
    
    CudaArray cellNodeIDs;
    CudaArray cellFaceIDs;
    CudaArray cellNeighbourCells;

    P centroid;
    double volume;
};