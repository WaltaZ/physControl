#include <problem/field/field.h>

namespace fieldKernel {
	template<class Obj, class StoragePlace>
	__global__
	void moveTraceToNextStep(Field<Obj, StoragePlace>* field) 
	{
        int C_id = blockDim.x * blockIdx.x + threadIdx.x;

        if (C_id >= field->values.length) { return; }

        for (size_t i = 0; i < field->pastValues.length - 1; i++)
        {
            field->pastValues[i + 1][C_id] = field->pastValues[i][C_id];
        }

        field->pastValues[0][C_id] = field->values[C_id];
	}

	template<class StoragePlace>
	__global__
	void updateMaxMin(Field<double, StoragePlace>* field, int* mutex) 
	{
		int tId = threadIdx.x;
		int bId = blockIdx.x;

		int globId = bId * blockDim.x + tId;

		uint32_t valuesLength = field->values.length;

		__shared__ double s_max[128];
		__shared__ double s_min[128];

		if (globId < valuesLength) {
			s_max[tId] = field->values[globId];
			s_min[tId] = field->values[globId];
		}
		else {
			s_max[tId] = field->values[valuesLength - 1];
			s_min[tId] = field->values[valuesLength - 1];
		}

		__syncthreads();

		for (size_t s = 1; s < blockDim.x; s *= 2)
		{
			int index = 2 * s * tId;
			if (index < blockDim.x)
			{
				if (s_max[index + s] > s_max[index]) { s_max[index] = s_max[index + s]; }
				if (s_min[index + s] < s_min[index]) { s_min[index] = s_min[index + s]; }
			}

			__syncthreads();
		}

		if (tId == 0) {
			while (atomicCAS(mutex, 0, 1) == 1);
			if (s_max[0] > field->maxValue) { field->maxValue = s_max[0]; }
			if (s_min[0] < field->minValue) { field->minValue = s_min[0]; }
			*mutex = 0;
		}
	}
}