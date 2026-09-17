#pragma once

#include "geometryEnums.h"

#include <array>
#include <cuda_runtime.h>
#include <nlohmann/json.hpp>

template<typename Derived, GeometryDim dim, int rank>
class Tensor {
public:

	static constexpr int numOfComp = [] {
		int result = 1;
		for (int i = 0; i < rank; i++)
			result *= geometryDimSize(dim);
		return result;
		}();

	std::array<double, numOfComp> comp;

	__host__ __device__ 
	Tensor(double initValue = 0)
	{
		comp = std::array<double, numOfComp>{};
		for (size_t i = 0; i < numOfComp; i++)
		{
			comp[i] = initValue;
		}
	};

	__host__ __device__
	Tensor(std::array<double, numOfComp> comp) : comp(comp) {};

	__host__ __device__
	Tensor(const double* comp) {
		this->comp = std::array<double, numOfComp>{};
		std::copy(comp, comp + numOfComp - 1, this->comp.data());
	}

	// OPERATIONS ------------------------------------

	__host__ __device__
	double& operator[](int index) {
		return comp[index];
	}

	__host__ __device__
	const double& operator[](int index) const {
		return comp[index];
	}

	__host__ __device__
	Derived operator+(const Derived& tensor) const
	{
		std::array<double, numOfComp> finalComp{};
		for (int i = 0; i < numOfComp; i++) {
			finalComp[i] = comp[i] + tensor.comp[i];
		}
		return Derived(finalComp);
	};

	__host__ __device__
	Derived& operator+=(const Derived& tensor) 
	{
		for (size_t i = 0; i < numOfComp; i++)
		{
			comp[i] += tensor.comp[i];
		}
		return *static_cast<Derived*>(this);
	}

	__host__ __device__
	Derived& operator-=(const Derived& tensor) 
	{
		for (size_t i = 0; i < numOfComp; i++)
		{
			comp[i] -= tensor.comp[i];
		}
		return *static_cast<Derived*>(this);
	}

	__host__ __device__
	Derived& operator*=(const double& scalar) 
	{
		for (size_t i = 0; i < numOfComp; i++)
		{
			comp[i] *= scalar;
		}
		return *static_cast<Derived*>(this);
	}

	__host__ __device__
	Derived& operator/=(const double& scalar) 
	{
		for (size_t i = 0; i < numOfComp; i++)
		{
			comp[i] /= scalar;
		}
		return *static_cast<Derived*>(this);
	}

	__host__ __device__
	Derived operator-(const Derived& tensor) const
	{
		std::array<double, numOfComp> finalComp{};
		for (int i = 0; i < numOfComp; i++) {
			finalComp[i] = comp[i] - tensor.comp[i];
		}
		return Derived(finalComp);
	};

	__host__ __device__
	Derived operator-() const
	{
		std::array<double, numOfComp> finalComp{};
		for (int i = 0; i < numOfComp; i++) {
			finalComp[i] = -comp[i];
		}
		return Derived(finalComp);
	};

	__host__ __device__
	Derived operator*(const double& scalar) const
	{
		std::array<double, numOfComp> finalComp{};
		for (int i = 0; i < numOfComp; i++) {
			finalComp[i] = comp[i] * scalar;
		}
		return Derived(finalComp);
	}
	
	__host__ __device__
	friend Derived operator*(const double& scalar, const Derived& tensor)
	{
		return tensor * scalar;
	}

	__host__ __device__
	Derived operator/(const double& scalar) const
	{
		std::array<double, numOfComp> finalComp{};
		for (int i = 0; i < numOfComp; i++) {
			finalComp[i] = comp[i] / scalar;
		}
		return Derived(finalComp);
	}

	__host__ __device__
	bool operator==(const Derived& tensor) const
	{
		bool isTheSame = true;
		for (int i = 0; i < numOfComp; i++) {
			isTheSame = (comp[i] == tensor.comp[i]);
			if (!isTheSame) { break; }
		}
		return isTheSame;
	}
};