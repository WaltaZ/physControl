#pragma once

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>
#include <simulation/linearSolver/linearSolverMatrix.h>

namespace debug {

	template<class Obj>
	void printObj(const Obj* obj, bool withNewLine = true)
	{
		printf("%lf", *obj);
		if (withNewLine) { printf("\n"); };
	};

	template<GeometryDim dim>
	void printObj(const Vector<dim>* obj, bool withNewLine) {
		printf("[ ");
		for (size_t comp = 0; comp < obj->numOfComp - 1; comp++)
		{
			printf("%lf, ", obj->comp[comp]);
		}
		printf("%lf ]", obj->comp[obj->numOfComp - 1]);
		if (withNewLine) { printf("\n"); }
	};

	template<GeometryDim dim>
	void printObj(const Point<dim>* obj, bool withNewLine) {
		printf("( ");
		for (size_t pos = 0; pos < geometryDimSize(dim) - 1; pos++)
		{
			printf("%lf, ", obj->pos[pos]);
		}
		printf("%lf )", obj->pos[geometryDimSize(dim) - 1]);
		if (withNewLine) { printf("\n"); }
	};

	template<class Obj>
	void printObj(const Obj& obj, bool withNewLine = true) {
		printObj(&obj, withNewLine); 
	};

	template<class StoragePlace>
	void printField(
		const Mesh<MeshDim::D3>* mesh,
		Field<double, StoragePlace>* field,
		uint32_t t = 0
	)
	{
		assert(t <= field->pastValues.length);

		double* values = field->values.getData();

		if (t > 0) { values = field->pastValues[t - 1].getData(); }

		for (int C_id = 0; C_id < field->values.length; C_id++)
		{
			const auto& cell = mesh->cells[C_id];
			const auto& p = cell.centroid;
			const auto& phi = values[C_id];

			printf("Cell %d (%lf, %lf, %lf) | Value: %lf\n", C_id, p.pos[0], p.pos[1], p.pos[2], phi);
		}
	};

	template<class StoragePlace>
	void printField(
		const Mesh<MeshDim::D3>* mesh,
		Field<Vector<GeometryDim::D3>, StoragePlace>* field,
		uint32_t t = 0
	)
	{
		assert(t <= field->pastValues.length);

		using V = Vector<GeometryDim::D3>;

		V* values = field->values.getData();

		if (t > 0) { values = field->pastValues[t - 1].getData(); }

		for (int C_id = 0; C_id < field->values.length; C_id++)
		{
			const auto& cell = mesh->cells[C_id];
			const auto& p = cell.centroid;
			const V& phi = values[C_id];

			printf("Cell %d (%lf, %lf, %lf) | Value: ", C_id, p.pos[0], p.pos[1], p.pos[2]);
			printObj(phi);
		}
	};

	template<class Obj>
	void printSolverMatrix(const LinearSolverMatrix<Obj>* matrix) {
		for (size_t C_id = 0; C_id < matrix->A_C.length; C_id++)
		{
			const double A_C = matrix->A_C[C_id];
			const auto& A_F = matrix->A_F[C_id];
			const auto& B = matrix->B[C_id];

			printf("Cell %d | A_C = %lf | B = ", C_id, A_C);
			printObj(B, false);
			printf(" | A_F = ", C_id, A_C);

			for (size_t f_id = 0; f_id < A_F.length - 1; f_id++)
			{
				printf("%lf, ", A_F[f_id]);
			}

			printf("%lf\n", A_F[A_F.length - 1]);
		}
	};
}