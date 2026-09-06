#pragma once

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>
#include <geometry/geometryUtils.h>

namespace debug {
	template<class StoragePlace>
	void printField(
		const Mesh<MeshDim::D3>& mesh, 
		Field<double, StoragePlace>& field,
		uint32_t t = 0
		) 
	{
		assert(t <= field.getElements()->pastValues.length);

		double* values = field.getElements()->values.getData();

		if (t > 0) { values = field.getElements()->pastValues[t - 1].getData(); }

		for (int C_id = 0; C_id < field.getElements()->values.length; C_id++)
		{
			const auto& cell = mesh.getElements()->cells[C_id];
			const auto& p = cell.centroid;
			const auto& phi = values[C_id];

			printf("Cell %d (%lf, %lf, %lf) | Value: %lf\n", C_id, p.pos[0], p.pos[1], p.pos[2], phi);
		}
	}

	template<class StoragePlace>
	void printField(
		const Mesh<MeshDim::D3>& mesh, 
		const Field<Vector<GeometryDim::D3>, StoragePlace>& field,
		uint32_t t = 0
	) 
	{
		assert(t <= field.getElements()->pastValues.length);

		using V = Vector<GeometryDim::D3>;

		V* values = field.getElements()->values.getData();

		if (t > 0) { values = field.getElements()->pastValues[t - 1].getData(); }

		for (int C_id = 0; C_id < field.getElements()->values.length; C_id++)
		{
			const auto& cell = mesh.getElements()->cells[C_id];
			const auto& p = cell.centroid;
			const V& phi = values[C_id];

			printf("Cell %d (%lf, %lf, %lf) | Value: ", C_id, p.pos[0], p.pos[1], p.pos[2]);
			geomPrint::printV(phi);
		}
	}
}