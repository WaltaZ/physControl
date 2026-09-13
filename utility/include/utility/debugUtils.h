#pragma once

#include <problem/field/field.h>
#include <mesh/meshElements/meshElements.h>
#include <simulation/linearSolver/linearSolverMatrix.h>

namespace debug {

	template<class Obj>
	__host__ __device__
	void printObj(const Obj* obj, bool withNewLine = true)
	{
		printf("%lf", *obj);
		if (withNewLine) { printf("\n"); };
	};

	template<GeometryDim dim>
	__host__ __device__
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
	__host__ __device__
	void printObj(const MatrixTensor<dim>* obj, bool withNewLine) {

		int gDim = geometryDimSize(dim);

		for (size_t i = 0; i < gDim; i++)
		{
			printf("[ ");
			for (size_t j = 0; j < gDim - 1; j++)
			{
				printf("%lf, ", obj->comp[gDim * i + j]);
			}
			printf("%lf ]\n", obj->comp[gDim * i]);
		}
		
	};

	template<GeometryDim dim>
	__host__ __device__
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
	__host__ __device__
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
		uint32_t length = field->getLength();

		for (size_t i = 0; i < length; i++)
		{
			printObj(field->values[i]);
		}
	};

	template<class StoragePlace>
	void printField(
		const Mesh<MeshDim::D3>* mesh,
		Field<MatrixTensor<GeometryDim::D3>, StoragePlace>* field,
		uint32_t t = 0
	)
	{
		assert(t <= field->pastValues.length);

		using M = MatrixTensor<GeometryDim::D3>;

		M* values = field->values.getData();

		if (t > 0) { values = field->pastValues[t - 1].getData(); }

		for (int C_id = 0; C_id < field->values.length; C_id++)
		{
			const auto& cell = mesh->cells[C_id];
			const auto& p = cell.centroid;
			const M& phi = values[C_id];

			printf("Cell %d (%lf, %lf, %lf)\n", C_id, p.pos[0], p.pos[1], p.pos[2]);
			printObj(phi);
			printf("\n");
		}
	};

	template<class Obj>
	void printSolverMatrix(const LinearSolverMatrix<Obj>* matrix) {
		for (size_t C_id = 0; C_id < matrix->A_C.length; C_id++)
		{
			const auto& A_C = matrix->A_C[C_id];
			const auto& A_F = matrix->A_F[C_id];
			const auto& B = matrix->B[C_id];

			printf("Cell %d | A_C = ", C_id);
			printObj(A_C, false);
			printf(" | B = ");
			printObj(B);

			for (size_t f_id = 0; f_id < A_F.length; f_id++)
			{
				printf("A_F%d = ", f_id);
				printObj(A_F[f_id]);
			}

			printf("\n----------------------------------------------------------------\n");
		}
	};
}