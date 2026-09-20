#include <problem/field/field.h>

#include <fileStream/binaryFiles/binStream.h>

namespace fieldFileKernel {
	__global__
	void setBoundaryPatchesPointers(
		CudaPackedArray<BoundaryPatch>& bp,
		uint32_t* faceIDs,
		double* values
	)
	{
		int i = blockDim.x * blockIdx.x + threadIdx.x;

		if (i >= bp.length) { return; }

		bp[i].faceIDs.setDataSource(faceIDs);
		bp[i].values.setDataSource(values);
	}
}

template<class Data, class StoragePlace>
void Field<Data, StoragePlace>::saveBoundaryPatches(const std::string& path) 
{
	if (boundaryPatches.length == 0) { return; }

	std::string fileName = name + "_BPs.bin";

	BinWriter bpFile({ path, fileName });

	bpFile.write(&bpFaceIDs);
	bpFile.writeArray(bpFaceIDs.getData(), bpFaceIDs.length);

	bpFile.write(&bpValues);
	bpFile.writeArray(bpValues.getData(), bpValues.length);

	bpFile.write(&boundaryPatches);
	bpFile.writeArray(boundaryPatches.getData(), boundaryPatches.length);
	
};

template<class Data, class StoragePlace>
void Field<Data, StoragePlace>::loadBoundaryPatches(const std::string& path) 
{
	if (boundaryPatches.length != 0) { return; }

	std::string fileName = name + "_BPs.bin";

	try {
		
		BinReader bpFile({ path, fileName });

		bpFile.read(&bpFaceIDs);
		cudaMallocManaged(bpFaceIDs.getDataPointer(), bpFaceIDs.length * sizeof(uint32_t));
		bpFile.readArray(bpFaceIDs.getData(), bpFaceIDs.length);

		bpFile.read(&bpValues);
		cudaMallocManaged(bpValues.getDataPointer(), bpValues.length * sizeof(double));
		bpFile.readArray(bpValues.getData(), bpValues.length);

		bpFile.read(&boundaryPatches);
		cudaMallocManaged(
			boundaryPatches.getDataPointer(), 
			boundaryPatches.length * sizeof(BoundaryPatch));
		bpFile.readArray(boundaryPatches.getData(), boundaryPatches.length);

		KernelArgs args = cudaUtils::getKernelArgs(boundaryPatches.length);
		fieldFileKernel::setBoundaryPatchesPointers <<< args.blocks, args.threads >>> (
			boundaryPatches,
			bpFaceIDs.getData(),
			bpValues.getData()
		);
		cudaUtils::fetchError(cudaDeviceSynchronize);

	} catch(...) {}
};



template class Field<double, Cell<MeshDim::D3>>;
template class Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>;
template class Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>;
template class Field<double, Face<MeshDim::D3>>;