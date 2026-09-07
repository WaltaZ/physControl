#include <simulation/discretization/gradient/gradientGauss.h>

template<typename Obj, typename GradObj>
void GradientGauss::computeImpl(
	const Field<Obj, Cell<MeshDim::D3>>* field,
	Field<GradObj, Cell<MeshDim::D3>>* gradField,
	const Mesh<MeshDim::D3>* mesh
) {
	// Calculating Gradient inside inner faces

	using namespace CUDA_GradientGauss;

	KernelArgs args = cudaUtils::getKernelArgs(mesh->cells.length);

	CUDA_compute_EC_internalFaces
		<< <args.blocks, args.threads >> > (
			field,
			gradField,
			mesh);

	cudaUtils::fetchError();
	cudaUtils::fetchError(cudaDeviceSynchronize);

	if (field->boundaryPatches.length == 0) {
		args = cudaUtils::getKernelArgs(mesh->faces.length);
		CUDA_compute_EF_noBC
			<<< args.blocks, args.threads >>> (
				field,
				gradField,
				mesh);
		cudaUtils::fetchError();
		cudaUtils::fetchError(cudaDeviceSynchronize);

		return;
	}

	args = cudaUtils::getKernelArgs(field->bpFaceIDsLengthMax);
	CUDA_compute_EF_BC
	<<<args.blocks, args.threads>>>(
		field,
		gradField,
		mesh);

	cudaUtils::fetchError();
	cudaUtils::fetchError(cudaDeviceSynchronize);
};

template
void GradientGauss::computeImpl(
	const Field<double, Cell<MeshDim::D3>>* field,
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* gradField,
	const Mesh<MeshDim::D3>* mesh
);

template
void GradientGauss::computeImpl(
	const Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field,
	Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>* gradField,
	const Mesh<MeshDim::D3>* mesh
);