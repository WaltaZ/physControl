#include "include/geometry/vector.h"

template<GeometryDim dim>
__host__ __device__
Vector<dim>::Vector() : ThisTensor() {}

template<GeometryDim dim>
__host__ __device__
Vector<dim>::Vector(std::array<double, _numOfComp> comp): ThisTensor(comp) {}

template<GeometryDim dim>
__host__ __device__
Vector<dim>::Vector(const double* comp) : ThisTensor(comp) {};

template<GeometryDim dim>
__host__ __device__
Vector<dim>::Vector(const Point<dim>& pointA, const Point<dim>& pointB)
{
	comp = std::array<double, _numOfComp>{};
	for (int i = 0; i < _numOfComp; i++) {
		comp[i] = pointB.pos[i] - pointA.pos[i];
	};
};

template<GeometryDim dim>
__host__ __device__
MatrixTensor<dim> Vector<dim>::operator*(const Vector<dim>& vec) const
{
	std::array<double, _numOfComp*_numOfComp> tensorComp{};

	for (size_t i = 0; i < _numOfComp; i++)
	{
		for (size_t j = 0; j < _numOfComp; j++)
		{
			tensorComp[i * _numOfComp + j] = comp[i] * vec.comp[j];
		}
	}

	return MatrixTensor<dim>(tensorComp);
};

template<GeometryDim dim>
__host__ __device__
double Vector<dim>::getMagnitude() const
{
	double mag = 0;
	for (int i = 0; i < _numOfComp; i++) {
		mag = mag + pow(comp[i], 2);
	}
	return sqrt(mag);
}

template<GeometryDim dim>
__host__ __device__
Vector<dim> Vector<dim>::getNormal() const
{
	return Vector<dim>(comp) / getMagnitude();
}

template<GeometryDim dim>
__host__ __device__
void Vector<dim>::flip()
{
	for (int i = 0; i < _numOfComp; i++) {
		if (comp[i] != 0) {
			comp[i] = -comp[i];
		}
	}
}

template class Vector<GeometryDim::D2>;
template class Vector<GeometryDim::D3>;