#pragma once

#include "boundaryPatch.h"

#include <type_traits>
#include <cuda_runtime.h>

template<typename DataType, typename StoragePlace>
struct Field {
    CudaAllocatedObj<DataType> values;
    DataType initialObj;

    bool isInitilized() {
        if (values.data != nullptr) {
            return true;
        }
        return false;
    }

    Field(const DataType& initialObj = DataType()) : initialObj(initialObj) {};
};

template<typename DataType, typename StoragePlace>
struct MainField : public Field<DataType, StoragePlace> {

    CudaAllocatedObj<BoundaryPatch> boundaryPatches;

    MainField(
        const DataType& obj = DataType()) : Field<DataType, StoragePlace>(obj) {};
};