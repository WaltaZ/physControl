#pragma once

#include "../meshEnums.h"
#include "supportStructs.h"

#include <vector>

template<MeshDim dim>
class Cell {
private:
	using P = Point<meshDimToGeometryDim(dim)>;
	using V = VectorData<meshDimToGeometryDim(dim)>;
public:

    Cell(
        int* pointIDs,
        int pointIDsLength,

        int* faceIDs,
        int faceIDsLength,

        int* neighbourCellsIDs,
        int neighbourCellsIDsLength,

        const P& centroid,
        double volume
    );

    int* pointIDs;
    int pointIDsLength;

    int* faceIDs;
    int faceIDsLength;

    int* neighbourCellsIDs;
    int neighbourCellsIDsLength;


    P centroid;
    double volume;
};