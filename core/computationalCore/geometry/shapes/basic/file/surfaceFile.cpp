#include <geometry/shapes/basic/surface.h>

template<GeometryDim dim>
void Surface<dim>::save(BinWriter& writer)
{
	size_t surfDim = geometryDimSize(dim);
	writer.writeObj(surfDim);

	size_t numOfVertices = vertices.size();
	writer.writeObj(numOfVertices);

	for (Point<dim>* vertex : vertices)
	{
		vertex->save(writer);
	}
}

template<GeometryDim dim>
Surface<dim> Surface<dim>::load(BinReader& reader) 
{
	size_t f_surfDim;
	reader.readObj(f_surfDim);

	if(f_surfDim != geometryDimSize(dim)) {
		throw("Surface dimension are incompatible with the dimensions inside the file!"); 
	}

	size_t numOfVertices;
	reader.readObj(numOfVertices);

	std::vector<P*> vertices;
	vertices.reserve(numOfVertices);

	for (size_t i = 0; i < numOfVertices; i++)
	{
		vertices.push_back(
			new P(P::load(reader))
		);
	}

	return Surface<dim>(vertices);
}

template class Surface<GeometryDim::D2>;
template class Surface<GeometryDim::D3>;