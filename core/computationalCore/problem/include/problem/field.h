#pragma once

#include <type_traits>
#include <mesh/meshElements/boundaryPatch.h>
#include <cuda_runtime.h>

template<typename DataType, typename StoragePlace>
struct SupportField {
    DataType* data;

    SupportField(const uint32_t& length, const DataType& obj = DataType()) {
        cudaMallocManaged(&data, length);
        for (int i = 0; i < length; i++) {
            data[i] = obj;
        };
    };

    ~SupportField() {
        cudaFree(data);
    };
};

template<typename DataType, typename StoragePlace>
struct Field : public SupportField<DataType, StoragePlace> {

    CudaAllocatedObj<BoundaryPatch> boundaryPatches;

    Field(
        const uint32_t& length,
        const std::vector<BoundaryPatch>& boundaryPatches,
        const DataType& obj = DataType()) : SupportField<DataType, StoragePlace>(length, obj)
    {

    };
};