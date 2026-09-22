#include <geometry/shapes/basic/point.h>

template<GeometryDim dim>
void Point<dim>::save(BinWriter& writer)
{
	writer.writeObj(pos.size());
	writer.writeArray(pos.data(), pos.size());
};

template<GeometryDim dim>
Point<dim> Point<dim>::load(BinReader& reader)
{
	std::array<double, geometryDimSize(dim)> pos{};

	size_t pointDim;
	reader.readObj(pointDim);

	if (pointDim != pos.size()) { throw("Point dimension are incompatible with the dimensions inside the file!"); }

	reader.readArray(pos.data(), pos.size());
	return Point<dim>(pos);
};

template class Point<GeometryDim::D2>;
template class Point<GeometryDim::D3>;