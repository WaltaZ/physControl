#include <utility/cudaUtils.h>

namespace cudaUtils {
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
}