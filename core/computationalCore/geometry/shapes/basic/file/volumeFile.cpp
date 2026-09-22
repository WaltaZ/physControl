#include <geometry/shapes/basic/volume.h>

#include <algorithm>

void Volume::save(BinWriter& writer)
{
	size_t numOfPoints = points.size();
	writer.writeObj(numOfPoints);

	for (P* point : points) {
		point->save(writer);
	}

	size_t numOfSurfaces = surfaces.size();
	writer.writeObj(numOfSurfaces);

	for (S* surface : surfaces)
	{
		size_t numOfSurfaceVertices = surface->vertices.size();
		writer.writeObj(numOfSurfaceVertices);

		for (P* point : surface->vertices) {

			auto it = find(points.begin(), points.end(), point);
			uint32_t d = distance(points.begin(), it);
			writer.writeObj(d);

		}

	}
};

Volume Volume::load(BinReader& reader) 
{
	size_t numOfPoints;
	reader.readObj(numOfPoints);

	std::vector<P*> points;
	points.reserve(numOfPoints);

	for (size_t i = 0; i < numOfPoints; i++)
	{
		points.push_back(
			new P(P::load(reader))
		);
	}

	size_t numOfSurfaces;
	reader.readObj(numOfSurfaces);

	std::vector<S*> surfaces;
	surfaces.reserve(numOfSurfaces);

	for (size_t i = 0; i < numOfSurfaces; i++)
	{
		size_t numOfSurfaceVertices;
		reader.readObj(numOfSurfaceVertices);

		std::vector<P*> surfaceVertices;
		surfaceVertices.reserve(numOfSurfaceVertices);

		for (size_t j = 0; j < numOfSurfaceVertices; j++)
		{
			uint32_t vertexId;
			reader.readObj(vertexId);

			surfaceVertices.push_back(points[vertexId]);
		}

		surfaces.push_back(
			new Surface(surfaceVertices)
		);
	}

	return Volume(points, surfaces);
};