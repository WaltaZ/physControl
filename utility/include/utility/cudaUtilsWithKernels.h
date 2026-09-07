#pragma once

#include <cuda_runtime.h>

namespace cudaUtils {
	namespace kernels {
		template<class Obj, typename... Args>
		__global__
			void _createInDeviceKernel(Obj* obj, Args... args)
		{
			new(obj) Obj(args...);
		};
	}

	template<class Obj, typename... Args>
	Obj* createInDevice(Args&&... args) {
		Obj* obj = nullptr;
		cudaMallocManaged(&obj, sizeof(Obj));

		kernels::_createInDeviceKernel <<<1, 1 >>> (obj, std::forward<Args>(args)...);
		fetchError(cudaDeviceSynchronize);

		return obj;
	};
}