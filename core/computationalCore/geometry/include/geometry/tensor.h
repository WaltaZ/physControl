#pragma once

#include "geometryEnums.h"

#include <array>
#include <cuda_runtime.h>

template<typename Derived, GeometryDim dim, int rank>
class Tensor {
protected:
	static constexpr int _numOfComp = [] {
		int result = 1;
		for (int i = 0; i < rank; i++)
			result *= geometryDimSize(dim);
		return result;
		}();
public:
	std::array<double, _numOfComp> comp;

	__host__ __device__ Tensor()
	{
		comp = std::array<double, _numOfComp>{};
		std::fill(std::begin(comp), std::end(comp), 0);
	};

	__host__ __device__
	Tensor(std::array<double, _numOfComp> comp) : comp(comp) {};

	__host__ __device__
	Tensor(const double* comp) {
		this->comp = std::array<double, _numOfComp>{};
		std::copy(comp, comp + _numOfComp - 1, this->comp.data());
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
		std::array<double, _numOfComp> finalComp{};
		for (int i = 0; i < _numOfComp; i++) {
			finalComp[i] = comp[i] + tensor.comp[i];
		}
		return Derived(finalComp);
	};

	__host__ __device__
	Derived operator-(const Derived& tensor) const
	{
		std::array<double, _numOfComp> finalComp{};
		for (int i = 0; i < _numOfComp; i++) {
			finalComp[i] = comp[i] - tensor.comp[i];
		}
		return Derived(finalComp);
	};

	__host__ __device__
	Derived operator*(const double& scalar) const
	{
		std::array<double, _numOfComp> finalComp{};
		for (int i = 0; i < _numOfComp; i++) {
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
		std::array<double, _numOfComp> finalComp{};
		for (int i = 0; i < _numOfComp; i++) {
			finalComp[i] = comp[i] / scalar;
		}
		return Derived(finalComp);
	}

	__host__ __device__
	bool operator==(const Derived& tensor) const
	{
		bool isTheSame = true;
		for (int i = 0; i < _numOfComp; i++) {
			isTheSame = (comp[i] == tensor.comp[i]);
			if (!isTheSame) { break; }
		}
		return isTheSame;
	}
};