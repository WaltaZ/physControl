#pragma once

#include "geometryEnums.h"
#include "tensor.h"

#include <array>

template<GeometryDim dim>
class MatrixTensor : public Tensor<MatrixTensor<dim>, dim, 2> {
public:
	using ThisTensor = Tensor<MatrixTensor<dim>, dim, 2>;
	using ThisTensor::ThisTensor;
	using ThisTensor::comp;
	using ThisTensor::numOfComp;

	__host__ __device__
	MatrixTensor();

	__host__ __device__
	MatrixTensor(std::array<double, numOfComp> comp);

	__host__ __device__
	MatrixTensor(const double* comp);
};