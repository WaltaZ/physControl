#pragma once

#include <type_traits>
#include <mesh/meshElements/boundaryPatch.h>

template<typename DataType, typename StoragePlace>
struct Field {

    DataType* data;
	const int dataLength;

    //BoundaryPatch* boundaryPatches;
    //const int boundaryPatchesLength;

    Field(
            const int numberOfElements,
            const DataType& initObj = DataType()) 
        : 
            dataLength(numberOfElements) 
    {
        this->data = new DataType[numberOfElements];
        for (int i = 0; i < numberOfElements; i++) {
            this->data[i] = initObj;
        }
    };

    ~Field() {
        delete[] data;
    }
};