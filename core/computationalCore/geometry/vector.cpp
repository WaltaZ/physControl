#include "include/geometry/vector.h"

template<GeometryDim dim>
Vector<dim>::Vector() : ThisTensor() {}

template<GeometryDim dim>
Vector<dim>::Vector(std::array<double, _numOfComp> comp): ThisTensor(comp) {}

template<GeometryDim dim>
Vector<dim>::Vector(const Point<dim>& pointA, const Point<dim>& pointB)
{
	comp = std::array<double, _numOfComp>{};
	for (int i = 0; i < _numOfComp; i++) {
		comp[i] = pointB.pos[i] - pointA.pos[i];
	};
}

template<GeometryDim dim>
double Vector<dim>::getMagnitude() const
{
	double mag = 0;
	for (int i = 0; i < _numOfComp; i++) {
		mag = mag + pow(comp[i], 2);
	}
	return sqrt(mag);
}

template<GeometryDim dim>
Vector<dim> Vector<dim>::getNormal() const
{
	return Vector<dim>(comp) / getMagnitude();
}

template class Vector<GeometryDim::D2>;
template class Vector<GeometryDim::D3>;