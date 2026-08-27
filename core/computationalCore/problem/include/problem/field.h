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
    };

    void initFiled(const CudaAllocatedObj<StoragePlace>& meshElements) {
        cudaMallocManaged(&(this->values.data), meshElements.length * sizeof(DataType));

        for (int i = 0; i < meshElements.length; i++) {
            values.data[i] = initialObj;
        }
    };

    Field(const DataType& initialObj = DataType()) : initialObj(initialObj) {};
};

template<typename DataType, typename StoragePlace>
struct MainField : public Field<DataType, StoragePlace> {

    CudaAllocatedObj<BoundaryPatch> boundaryPatches;

    CudaAllocatedObj<uint32_t> bpFaceIDs;
    CudaAllocatedObj<uint32_t> values;

    MainField(
        const DataType& obj = DataType()) : Field<DataType, StoragePlace>(obj) {};

    void initBoundaryPatches(const std::vector<ProblemBoundaryPatch>& boundaryPatches) {

        cudaMallocManaged(&(this->boundaryPatches.data), boundaryPatches.size() * sizeof(BoundaryPatch));
        this->boundaryPatches.length = boundaryPatches.size();

        std::vector<uint32_t> bpFaceIDs;
        std::vector<uint32_t> values;

        for (size_t i = 0; i < boundaryPatches.size(); i++) {
            this->boundaryPatches.data[i] = BoundaryPatch{
                boundaryPatches[i].type,
                CudaArray<uint32_t>(
                    &(this->bpFaceIDs.data),
                    static_cast<uint32_t>(bpFaceIDs.size()),
                    static_cast<uint32_t>(boundaryPatches[i].faceIDs.size())
                ),
                CudaArray<uint32_t>(
                    &(this->values.data),
                    static_cast<uint32_t>(values.size()),
                    static_cast<uint32_t>(boundaryPatches[i].values.size())
                )
            };
            bpFaceIDs.insert(bpFaceIDs.end(), boundaryPatches[i].faceIDs.begin(), boundaryPatches[i].faceIDs.end());
            values.insert(values.end(), boundaryPatches[i].values.begin(), boundaryPatches[i].values.end());
        }

        cudaMallocManaged(&(this->bpFaceIDs.data), bpFaceIDs.size() * sizeof(uint32_t));
        this->bpFaceIDs.length = bpFaceIDs.size();
        for (size_t i = 0; i < bpFaceIDs.size(); i++) {
            this->bpFaceIDs.data[i] = bpFaceIDs[i];
        }

        if (values.size() == 0) { return; }

        cudaMallocManaged(&(this->values.data), values.size() * sizeof(uint32_t));
        this->values.length = values.size();
        for (size_t i = 0; i < values.size(); i++) {
            this->values.data[i] = values[i];
        }
    }
};