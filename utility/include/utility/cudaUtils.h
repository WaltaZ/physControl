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

template<typename T>
class CudaArray {
public:
	uint32_t offset;
	uint32_t length;

	CudaArray(T** dataSource, uint32_t offset, uint32_t length) : 
		dataSource(dataSource), offset(offset), length(length) {};

	T& operator[](size_t index) {
		return (*dataSource)[index + offset];
	};

	const T& operator[](size_t index) const {
		return (*dataSource)[index + offset];
	};

	T* getData() {
		return ((*dataSource) + offset);
	}

private:
	T** dataSource;
};

template<typename T>
class CudaAllocatedObj {
public:
	uint32_t length = 0;

	CudaAllocatedObj() {};

	T& operator[](int index) {
		return data[index];
	};

	const T& operator[](int index) const {
		return data[index];
	};

	T** getDataPointer() {
		return &data;
	};

	~CudaAllocatedObj() {
		if (data != nullptr) {
			cudaFree(data);
		}
	}

private:
	T* data = nullptr;
};