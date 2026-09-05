#pragma once

#include "geometryEnums.h"
#include "point.h"
#include "tensor.h"
#include "matrixTensor.h"

#include <array>

template <GeometryDim dim>
class Vector : public Tensor<Vector<dim>, dim, 1> {
public:

	using ThisTensor = Tensor<Vector<dim>, dim, 1>;
	using ThisTensor::comp;
	using ThisTensor::_numOfComp;
	using ThisTensor::operator*;

	__host__ __device__ 
	Vector();

	__host__ __device__ 
	Vector(std::array<double, _numOfComp> comp);

	__host__ __device__ 
	Vector(const double* comp);

	__host__ __device__
	Vector(Point<dim> point) : ThisTensor(point.pos) {};

	__host__ __device__ 
	Vector(const Point<dim>& pointA, const Point<dim>& pointB);

	__host__ __device__
	MatrixTensor<dim> operator*(const Vector<dim>& vec) const; // Tensor product

	__host__ __device__ 
	double getMagnitude() const;

	__host__ __device__ 
	Vector<dim> getNormal() const;

	__host__ __device__ 
	void flip();
};
