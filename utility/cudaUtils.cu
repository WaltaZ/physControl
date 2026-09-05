#include <utility/cudaUtils.h>


namespace cudaUtils {

	using V = Vector<GeometryDim::D3>;
	using T = MatrixTensor<GeometryDim::D3>;

	void fetchError() {
		cudaError_t err = cudaGetLastError();

		if (err != cudaSuccess) {
			printf("Kernel error: %s\n", cudaGetErrorString(err));
		}
	}

	void fetchError(cudaError_t(*operation)()) {
		cudaError_t err = operation();

		if (err != cudaSuccess) {
			printf("Kernel error: %s\n", cudaGetErrorString(err));
		}
	}

	KernelArgs getKernelArgs(
		int numOfElements,
		int threadsPerBlock)
	{
		return KernelArgs{
			static_cast<int>(
				ceil( (double)numOfElements / 
					(double)threadsPerBlock)),

			threadsPerBlock
		};
	};

	template<class Obj>
	__device__
		void contributeTo(Obj& dest, const Obj& obj)
	{
		for (size_t i = 0; i < obj.comp.size(); i++)
		{
			atomicAdd(&(dest.comp[i]), obj.comp[i]);
		}
	};

	template __device__ void contributeTo(V& dest, const V& obj);
	template __device__ void contributeTo(T& dest, const T& obj);

	template<>
	__device__
		void contributeTo(double& dest, const double& obj)
	{
		atomicAdd(&dest, obj);
	};
}