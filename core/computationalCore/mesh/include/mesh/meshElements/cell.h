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
        const int* nodeIDs,
        int nodeIDsLength,

        const int* faceIDs,
        int faceIDsLength,

        const int* neighbourCellsIDs,
        int neighbourCellsIDsLength,

        const P& centroid,
        double volume
    );

    Cell();

    ~Cell();
    Cell(const Cell<dim>& other);
    Cell(Cell<dim>&& other) noexcept;
    Cell<dim>* operator=(const Cell<dim>& other);
    Cell<dim>* operator=(Cell<dim>&& other) noexcept;
    
    int* nodeIDs = nullptr;
    int nodeIDsLength = 0;

    int* faceIDs = nullptr;
    int faceIDsLength = 0;

    int* neighbourCellsIDs = nullptr;
    int neighbourCellsIDsLength = 0;


    P centroid;
    double volume;
};