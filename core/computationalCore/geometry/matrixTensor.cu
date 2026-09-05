#include "include/geometry/matrixTensor.h"

template<GeometryDim dim>
__host__ __device__
MatrixTensor<dim>::MatrixTensor() : ThisTensor() {};

template<GeometryDim dim>
__host__ __device__
MatrixTensor<dim>::MatrixTensor(std::array<double, _numOfComp> comp) : ThisTensor(comp) {};

template<GeometryDim dim>
__host__ __device__
MatrixTensor<dim>::MatrixTensor(const double* comp) : ThisTensor(comp) {};

template class MatrixTensor<GeometryDim::D2>;
template class MatrixTensor<GeometryDim::D3>;