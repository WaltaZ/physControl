#pragma once

#include "geometryEnums.h"
#include "shapes/basic/point.h"
#include "tensor.h"

#include <array>

template <GeometryDim dim>
class Vector : public Tensor<Vector<dim>, dim, 1> {
public:

	using ThisTensor = Tensor<Vector<dim>, dim, 1>;
	using ThisTensor::comp;
	using ThisTensor::_numOfComp;

	Vector();
	Vector(std::array<double, _numOfComp> comp);
	Vector(Point<dim> point) : ThisTensor(point.pos) {};
	Vector(const Point<dim>& pointA, const Point<dim>& pointB);

	double getMagnitude();

	Vector<dim> getNormal();
};
