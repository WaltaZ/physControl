#pragma once

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace cudaConfig {
	constexpr cudaMemLocation deviceLocation = {
		cudaMemLocationTypeDevice,
		0
	};

	constexpr cudaMemLocation hostLocation = {
		cudaMemLocationTypeHost,
		cudaCpuDeviceId
	};
}

struct CudaArray {
	uint32_t offset = 0;
	uint32_t length = 0;
};

template<typename T>
struct CudaAllocatedObj {
	T* data = nullptr;
	uint32_t length = 0;

	~CudaAllocatedObj() {
		if (data != nullptr) {
			cudaFree(data);
		}
	}
};