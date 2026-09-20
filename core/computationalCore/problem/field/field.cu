#include <problem/field/field.h>

#include "fieldKernel.cu"
#include <limits.h>

template<class Data, class StoragePlace>
Field<Data, StoragePlace>::Field(
    const std::string& name,
    const Data& obj) : name(name), initialObj(obj) {};

template<class Data, class StoragePlace>
bool Field<Data, StoragePlace>::isInitilized() 
{
    if (values.length != 0) {
        return true;
    }
    return false;
};

template<class Data, class StoragePlace>
void Field<Data, StoragePlace>::initField(const CudaPackedArray<StoragePlace>& meshElements) 
{
    cudaMallocManaged(this->values.getDataPointer(), meshElements.length * sizeof(Data));
    this->values.length = meshElements.length;

    for (int i = 0; i < meshElements.length; i++) {
        values[i] = initialObj;
    }
};

template<class Data, class StoragePlace>
void Field<Data, StoragePlace>::initPastTrace(uint32_t traceLength) 
{ // Works
    assert(traceLength > 0);
    assert(isInitilized());

    // TODO: Write a kernel which would allocate it

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

    cudaUtils::fetchError(cudaDeviceSynchronize);
}

template<class Data, class StoragePlace>
void Field<Data, StoragePlace>::initBoundaryPatches(
    const std::vector<ProblemBoundaryPatch>& boundaryPatches) 
{

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

template<class Data, class StoragePlace>
__host__ __device__
Data& Field<Data, StoragePlace>::operator[](int index)
{
    return values[index];
}

template<class Data, class StoragePlace>
__host__ __device__
const Data& Field<Data, StoragePlace>::operator[](int index) const
{
    return values[index];
}

template<class Data, class StoragePlace>
__host__ __device__
uint32_t Field<Data, StoragePlace>::getLength() const 
{
    return values.length;
}

template<class Data, class StoragePlace>
void Field<Data, StoragePlace>::moveTraceToNextStep() 
{
    KernelArgs args = cudaUtils::getKernelArgs(values.length);
    fieldKernel::moveTraceToNextStep <<<args.blocks, args.threads>>>(this);
    cudaUtils::fetchError(cudaDeviceSynchronize);
}

template<class Data, class StoragePlace>
void Field<Data, StoragePlace>::updateMaxMin() 
{
    throw("Bad field type");
}

template<>
void Field<double, Cell<MeshDim::D3>>::updateMaxMin()
{

    // TODO: Fix in the future - make a general function

    minValue = DBL_MAX;
    maxValue = DBL_MIN;

    int* mutex;
    cudaMalloc(&mutex, sizeof(int));
    cudaMemset(mutex, 0, sizeof(int));

    KernelArgs args = cudaUtils::getKernelArgs(values.length, 128);
    fieldKernel::updateMaxMin <<<args.blocks, args.threads >>> (this, mutex);
    cudaUtils::fetchError(cudaDeviceSynchronize);

    cudaFree(mutex);
}

template class Field<double, Cell<MeshDim::D3>>;
template class Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>;
template class Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>;
template class Field<double, Face<MeshDim::D3>>;