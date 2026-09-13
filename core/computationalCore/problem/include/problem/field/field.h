#pragma once

#include "boundaryPatch.h"

#include <type_traits>
#include <cuda_runtime.h>

template<class Data, class StoragePlace>
struct Field {
public:
    CudaPackedArray<Data> values;

    double maxValue;
    double minValue;

    CudaPackedArray<CudaArray<Data>> pastValues;
    Data initialObj;

    CudaPackedArray<BoundaryPatch> boundaryPatches;
    uint32_t bpFaceIDsLengthMax = 0;

    CudaPackedArray<uint32_t> bpFaceIDs;
    CudaPackedArray<double> bpValues;

    Field(const Data& obj = Data());
    
    bool isInitilized();

    void initField(const CudaPackedArray<StoragePlace>& meshElements);

    void initPastTrace(uint32_t traceLength);

    void initBoundaryPatches(const std::vector<ProblemBoundaryPatch>& boundaryPatches);

    __host__
    void moveTraceToNextStep();

    __host__
    void updateMaxMin();

    __host__ __device__
    Data& operator[](int index);

    __host__ __device__
    const Data& operator[](int index) const;

    __host__ __device__
    uint32_t getLength() const;

private:
    CudaPackedArray<Data> _pastValuesAll;
};