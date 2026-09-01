#pragma once

#include "boundaryPatch.h"

#include <type_traits>
#include <cuda_runtime.h>

template<typename DataType, typename StoragePlace>
struct Field {
    CudaAllocatedObj<DataType> values;
    DataType initialObj;

    CudaAllocatedObj<BoundaryPatch> boundaryPatches;

    CudaAllocatedObj<uint32_t> bpFaceIDs;
    CudaAllocatedObj<double> bpValues;

    Field(const DataType& obj = DataType()) : initialObj(obj) {};
    
    bool isInitilized() {
        if (values.length != 0) {
            return true;
        }
        return false;
    };

    void initFiled(const CudaAllocatedObj<StoragePlace>& meshElements) {
        cudaMallocManaged(this->values.getDataPointer(), meshElements.length * sizeof(DataType));
        this->values.length = meshElements.length;

        for (int i = 0; i < meshElements.length; i++) {
            values[i] = initialObj;
        }
    };



    void initBoundaryPatches(const std::vector<ProblemBoundaryPatch>& boundaryPatches) {

        cudaMallocManaged(this->boundaryPatches.getDataPointer(), boundaryPatches.size() * sizeof(BoundaryPatch));
        this->boundaryPatches.length = boundaryPatches.size();

        std::vector<uint32_t> bpFaceIDs;
        std::vector<uint32_t> bpFaceOffset;

        std::vector<double> values;
        std::vector<double> valuesOffset;

        for (size_t i = 0; i < boundaryPatches.size(); i++) {
            bpFaceOffset.emplace_back(bpFaceIDs.size());
            bpFaceIDs.insert(
                bpFaceIDs.end(), 
                boundaryPatches[i].faceIDs.begin(), 
                boundaryPatches[i].faceIDs.end());

            valuesOffset.emplace_back(valuesOffset.size());
            values.insert(
                values.end(), 
                boundaryPatches[i].values.begin(), 
                boundaryPatches[i].values.end());
        }

        cudaMallocManaged(this->bpFaceIDs.getDataPointer(), bpFaceIDs.size() * sizeof(uint32_t));
        this->bpFaceIDs.length = bpFaceIDs.size();

        if (values.size() > 0) {
            cudaMallocManaged(this->bpValues.getDataPointer(), values.size() * sizeof(double));
            this->bpValues.length = values.size();
        }

        for (size_t i = 0; i < boundaryPatches.size(); i++) {
            this->boundaryPatches[i] = BoundaryPatch{
                boundaryPatches[i].type,
                CudaArray<uint32_t>(
                    this->bpFaceIDs.getData(),
                    bpFaceOffset[i],
                    static_cast<uint32_t>(boundaryPatches[i].faceIDs.size())
                ),
                CudaArray<double>(
                    this->bpValues.getData(),
                    valuesOffset[i],
                    static_cast<uint32_t>(boundaryPatches[i].values.size())
                )
            };
        }

        std::copy(
            bpFaceIDs.begin(),
            bpFaceIDs.end(),
            this->bpFaceIDs.getData()
        );

        if (values.size() == 0) { return; }

        std::copy(
            values.begin(),
            values.end(),
            this->bpValues.getData()
        );
    }
};