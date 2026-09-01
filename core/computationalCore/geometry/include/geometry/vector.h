#pragma once

#include "geometryEnums.h"
#include "point.h"
#include "tensor.h"

#include <array>

template <GeometryDim dim>
class Vector : public Tensor<Vector<dim>, dim, 1> {
public:

	using ThisTensor = Tensor<Vector<dim>, dim, 1>;
	using ThisTensor::comp;
	using ThisTensor::_numOfComp;

	__host__ __device__ 
	Vector();

	__host__ __device__ 
	Vector(std::array<double, _numOfComp> comp);

	__host__ __device__
	Vector(Point<dim> point) : ThisTensor(point.pos) {};

	__host__ __device__ 
	Vector(const Point<dim>& pointA, const Point<dim>& pointB);

	__host__ __device__ 
	double getMagnitude() const;

	__host__ __device__ 
	Vector<dim> getNormal() const;

	__host__ __device__ 
	void flip();
};
