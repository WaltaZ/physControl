#include <simulation/discretization/gradient/gradientGauss.h>

template<typename Obj, typename ObjDest>
void GradientGauss::computeImpl(
	Field<Obj, Cell<MeshDim::D3>>& field,
	Field<ObjDest, Cell<MeshDim::D3>>& destField,
	Mesh<MeshDim::D3>& mesh
) {
	// Calculating Gradient inside inner faces

	using namespace CUDA_GradientGauss;

	KernelArgs args = cudaUtils::getKernelArgs(mesh.getElements()->cells.length);

	CUDA_compute_EC_internalFaces
		<< <args.blocks, args.threads >> > (
			field.getElements(),
			destField.getElements(),
			mesh.getElements());

	cudaUtils::fetchError();
	cudaUtils::fetchError(cudaDeviceSynchronize);

	if (field.getElements()->boundaryPatches.length == 0) {
		args = cudaUtils::getKernelArgs(mesh.getElements()->faces.length);
		CUDA_compute_EF_noBC
			<<< args.blocks, args.threads >>> (
				field.getElements(),
				destField.getElements(),
				mesh.getElements());
		cudaUtils::fetchError();
		cudaUtils::fetchError(cudaDeviceSynchronize);

		return;
	}

	args = cudaUtils::getKernelArgs(field.getElements()->bpFaceIDsLengthMax);
	CUDA_compute_EF_BC
	<<<args.blocks, args.threads>>>(
		field.getElements(),
		destField.getElements(),
		mesh.getElements());

	cudaUtils::fetchError();
	cudaUtils::fetchError(cudaDeviceSynchronize);

	/*for (int i = 0; i < destField.getElements()->values.length; i++) {
		std::cout << "Cell: " << i << " | Gradient: ";
		geomPrint::printV(destField.getElements()->values[i]);
	}*/
	
};

template
void GradientGauss::computeImpl(
	Field<double, Cell<MeshDim::D3>>& field,
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
	Mesh<MeshDim::D3>& mesh
);

template
void GradientGauss::computeImpl(
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field,
	Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
	Mesh<MeshDim::D3>& mesh
);