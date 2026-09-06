#pragma once

#include "boundaryPatch.h"

#include <type_traits>
#include <cuda_runtime.h>

template<typename Data, typename StoragePlace>
struct CudaField {
public:
    CudaPackedArray<Data> values;
    CudaPackedArray<CudaArray<Data>> pastValues;
    Data initialObj;

    CudaPackedArray<BoundaryPatch> boundaryPatches;
    uint32_t bpFaceIDsLengthMax = 0;

    CudaPackedArray<uint32_t> bpFaceIDs;
    CudaPackedArray<double> bpValues;

    CudaField(const Data& obj = Data()) : initialObj(obj) {};
    
    bool isInitilized() {
        if (values.length != 0) {
            return true;
        }
        return false;
    };

    void initFiled(const CudaPackedArray<StoragePlace>& meshElements) {
        cudaMallocManaged(this->values.getDataPointer(), meshElements.length * sizeof(Data));
        this->values.length = meshElements.length;

        for (int i = 0; i < meshElements.length; i++) {
            values[i] = initialObj;
        }
    };

    void initPastTrace(uint32_t traceLength) { // Works
        assert(traceLength > 0);
        assert(isInitilized());

        uint32_t fieldLength = values.length;

        cudaMallocManaged(
            _pastValuesAll.getDataPointer(), 
            traceLength * fieldLength * sizeof(Data)
        );
        _pastValuesAll.length = traceLength * fieldLength;

        cudaMallocManaged(
            pastValues.getDataPointer(),
            traceLength * sizeof(CudaArray<Data>)
        );
        pastValues.length = traceLength;

        uint32_t offset = 0;
        for (size_t i = 0; i < traceLength; i++)
        {
            pastValues[i] = CudaArray(
                _pastValuesAll.getData(),
                offset,
                fieldLength
            );

            std::copy(
                values.getData(),
                values.getData() + fieldLength,
                pastValues[i].getData()
            );

            offset += fieldLength;
        }
    }

    void initBoundaryPatches(const std::vector<ProblemBoundaryPatch>& boundaryPatches) {

        cudaMallocManaged(
            this->boundaryPatches.getDataPointer(), 
            boundaryPatches.size() * sizeof(BoundaryPatch));

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
            if (boundaryPatches[i].faceIDs.size() > bpFaceIDsLengthMax) {
                bpFaceIDsLengthMax = boundaryPatches[i].faceIDs.size();
            }

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

    __device__
    void moveTraceToNextStep() { // Works
        int C_id = blockDim.x * blockIdx.x + threadIdx.x;

        if (C_id >= values.length) { return; }

        for (size_t i = 0; i < pastValues.length - 1; i++)
        {
            pastValues[i + 1][C_id] = pastValues[i][C_id];
        }

        pastValues[0][C_id] = values[C_id];
    }

private:
    CudaPackedArray<Data> _pastValuesAll;
};

template<typename DataType, typename StoragePlace>
class Field {
public:

    Field(const DataType& obj = DataType()) 
    {
        cudaMallocManaged(&_field, sizeof(CudaField<DataType, StoragePlace>));

        new(_field) CudaField<DataType, StoragePlace>(obj);
    };

    CudaField<DataType, StoragePlace>* getElements() { return _field; };
    const CudaField<DataType, StoragePlace>* getElements() const { return _field; };
private:
    CudaField<DataType, StoragePlace>* _field;
};