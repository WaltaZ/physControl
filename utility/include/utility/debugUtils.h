#pragma once

#include <problem/field/field.h>
#include <mesh/meshElements/meshElements.h>
#include <simulation/linearSolver/linearSolverMatrix.h>

#include <string>

class Printer {
	
	char buf[1024];

public:

};

namespace debug {

	template<class Obj>
	__host__ __device__
	void printObj(const Obj* obj, bool withNewLine = true)
	{
		if (withNewLine) {
			printf("%lf\n", *obj);
		}
		else {
			printf("%lf", *obj);
		}
	};

	template<GeometryDim dim>
	__host__ __device__
	void printObj(const Vector<dim>* obj, bool withNewLine) {

		int gDim = geometryDimSize(dim);

		const auto& c = obj->comp;

		if (withNewLine) {
			if (gDim == 2) {
				printf("[ %lf, %lf ]\n", c[0], c[1]);
			}
			else if (gDim == 3) {
				printf("[ %lf, %lf, %lf ]\n", c[0], c[1], c[2]);
			}
		}
		else {
			if (gDim == 2) {
				printf("[ %lf, %lf ]", c[0], c[1]);
			}
			else if (gDim == 3) {
				printf("[ %lf, %lf, %lf ]", c[0], c[1], c[2]);
			}
		}
	};

	template<GeometryDim dim>
	__host__ __device__
		void printObj(const MatrixTensor<dim>* obj, bool withNewLine) {

		int gDim = geometryDimSize(dim);

		const auto& c = obj->comp;

		if (withNewLine) {
			if (gDim == 2) {
				printf("[ %lf, %lf ]\n[ %lf, %lf ]\n", c[0], c[1], c[2], c[3]);
			}
			else if (gDim == 3) {
				printf("[ %lf, %lf, %lf ]\n[ %lf, %lf, %lf ]\n[ %lf, %lf, %lf ]\n",
					c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
			}
		}
		else {
			if (gDim == 2) {
				printf("[ %lf, %lf ]\n[ %lf, %lf ]", c[0], c[1], c[2], c[3]);
			}
			else if (gDim == 3) {
				printf("[ %lf, %lf, %lf ]\n[ %lf, %lf, %lf ]\n[ %lf, %lf, %lf ]",
					c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
			}
		};
	}

	template<GeometryDim dim>
	__host__ __device__
		void printObj(const Point<dim>* obj, bool withNewLine) {

		int gDim = geometryDimSize(dim);

		const auto& p = obj->pos;

		if (withNewLine) {
			if (gDim == 2) {
				printf("( %lf, %lf )\n", p[0], p[1]);
			}
			else if (gDim == 3) {
				printf("( %lf, %lf, %lf )\n", p[0], p[1], p[2]);
			}
		}
		else {
			if (gDim == 2) {
				printf("( %lf, %lf )", p[0], p[1]);
			}
			else if (gDim == 3) {
				printf("( %lf, %lf, %lf )", p[0], p[1], p[2]);
			}
		}

	};

	template<class Obj>
	__host__ __device__
	void printObj(const Obj& obj, bool withNewLine = true) {
		printObj(&obj, withNewLine); 
	};

	template<class Obj, class StoragePlace>
	void printField(
		Field<Obj, StoragePlace>* field,
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
			const M& phi = values[C_id];

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