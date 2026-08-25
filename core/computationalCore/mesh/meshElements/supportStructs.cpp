#include "../include/mesh/meshElements/supportStructs.h"

template<GeometryDim dim>
VectorData<dim>::VectorData(){};

template<GeometryDim dim>
VectorData<dim>::VectorData(
	const V& vector,
	const V& normal,
	const double& magnitude) :
	vector(vector), normal(normal), magnitude(magnitude)
{};

template<GeometryDim dim>
VectorData<dim>::VectorData(Vector<dim> vector) :
	vector(vector), normal(vector.getNormal()), magnitude(vector.getMagnitude())
{};

template class VectorData<GeometryDim::D2>;
template class VectorData<GeometryDim::D3>;