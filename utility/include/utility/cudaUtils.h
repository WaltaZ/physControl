#pragma once

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cstdio>

namespace cudaConfig {
	constexpr cudaMemLocation deviceLocation = {
		cudaMemLocationTypeDevice,
		0
	};

	constexpr cudaMemLocation hostLocation = {
		cudaMemLocationTypeHost,
		cudaCpuDeviceId
	};

	constexpr int defaultThreadsPerBlock = 256;
}

namespace cudaUtils {
	void fetchError();

	void fetchError(cudaError_t(*operation)());
}

template<typename T>
class CudaArray {
public:
	uint32_t offset;
	uint32_t length;

	__host__ __device__
	CudaArray(T* data, uint32_t offset, uint32_t length) : 
		data(data), offset(offset), length(length) {};

	__host__ __device__ T& operator[](size_t index) {
		return data[index + offset];
	};

	__host__ __device__ const T& operator[](size_t index) const {
		return data[index + offset];
	};

	__host__ __device__ T** getDataPointer() {
		return &data;
	};

	__host__ __device__ const T** getDataPointer() const {
		return &data;
	};

	__host__ __device__ T* getData() {
		return (data + offset);
	}

	__host__ __device__ const T* getData() const {
		return (data + offset);
	}

private:
	T* data;
};

template<typename T>
class CudaAllocatedObj {
public:
	uint32_t length = 0;

	__host__ __device__
	CudaAllocatedObj() {};

	__host__ __device__ T& operator[](int index) {
		return data[index];
	};

	__host__ __device__ const T& operator[](int index) const {
		return data[index];
	};

	__host__ __device__ T** getDataPointer() {
		return &data;
	};

	__host__ __device__ const T** getDataPointer() const {
		return &data;
	};

	__host__ __device__ T* getData() {
		return data;
	}

	__host__ __device__ const T* getData() const {
		return data;
	}

	~CudaAllocatedObj() {
		if (data != nullptr) {
			cudaFree(data);
		}
		
	}

private:
	T* data = nullptr;
};