#include "../../include/geometry/shapes/basic/line.h"

template<GeometryDim dim>
_Line<dim>::_Line(P* start, P* end, const std::optional<int>& id) : start(start), end(end), id(id) {}

template<GeometryDim dim>
_Line<dim>::~_Line(){};

template<GeometryDim dim>
_Line<dim>::P _Line<dim>::getCentroid()
{
	return geometryOperations::calculateGeoCenter(std::vector<P*>({ start, end }));
}

template<GeometryDim dim>
double _Line<dim>::getLength()
{
	return V(*start, *end).getMagnitude();
}

template class _Line<GeometryDim::D2>;
template class _Line<GeometryDim::D3>;

// ---------------------------------------- 2D LINE -------------------------------------------

Line<GeometryDim::D2>::Line(P* start, P* end, const std::optional<int>& id) : _Line(start, end, id) {};
Line<GeometryDim::D2>::~Line() {};
Line<GeometryDim::D2>::V Line<GeometryDim::D2>::getAreaVector()
{
	V lengthVector = V(*start, *end);
	return V({ -lengthVector.comp[1], lengthVector.comp[0] });
}

// ---------------------------------------- 3D LINE -------------------------------------------

Line<GeometryDim::D3>::Line(P * start, P * end, const std::optional<int>& id) : _Line(start, end, id) {};