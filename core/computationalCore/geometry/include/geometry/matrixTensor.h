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
	using ThisTensor::_numOfComp;

	MatrixTensor();
	MatrixTensor(std::array<double, _numOfComp> comp);
};