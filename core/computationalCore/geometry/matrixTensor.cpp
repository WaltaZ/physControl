#include "include/geometry/matrixTensor.h"

template<GeometryDim dim>
MatrixTensor<dim>::MatrixTensor() : ThisTensor() {};

template<GeometryDim dim>
MatrixTensor<dim>::MatrixTensor(std::array<double, _numOfComp> comp) : ThisTensor(comp) {};

template class MatrixTensor<GeometryDim::D2>;
template class MatrixTensor<GeometryDim::D3>;