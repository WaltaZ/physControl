#pragma once

#include <cuda_runtime.h>
#include <mesh/meshElements/mesh.h>
#include <problem/field/field.h>
#include <simulation/linearSolver/linearSolverMatrix.h>

class ConvectionBase {
protected:
	using V = Vector<GeometryDim::D3>;
	using T = MatrixTensor<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;
	using F = Face<MeshDim::D3>;
public:
	__device__ virtual void assembleInner(
		const Mesh<MeshDim::D3>* mesh,
		Field<double, C>* field,
		Field<V, C>* gradField,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<double>* matrix
	) = 0;

	__device__ virtual void assembleInner(
		const Mesh<MeshDim::D3>* mesh,
		Field<V, C>* field,
		Field<T, C>* gradField,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<V>* matrix
	) = 0;


	__device__ virtual void assembleBoundaries(
		const Mesh<MeshDim::D3>* mesh,
		Field<double, C>* field,
		Field<V, C>* gradField,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<double>* matrix
	) = 0;

	__device__ virtual void assembleBoundaries(
		const Mesh<MeshDim::D3>* mesh,
		Field<V, C>* field,
		Field<T, C>* gradField,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<V>* matrix
	) = 0;
};

template<class Derived>
class Convection : public ConvectionBase {
public:
	__device__ void assembleInner(
		const Mesh<MeshDim::D3>* mesh,
		Field<double, C>* field,
		Field<V, C>* gradField,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<double>* matrix
	) override
	{
		static_cast<Derived*>(this)->
			template assembleInnerImpl<double, V>(
				mesh, field, gradField, massFlowRate, matrix);
	}

	__device__ void assembleInner(
		const Mesh<MeshDim::D3>* mesh,
		Field<V, C>* field,
		Field<T, C>* gradField,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<V>* matrix
	) override
	{
		static_cast<Derived*>(this)->
			template assembleInnerImpl<V, T>(
				mesh, field, gradField, massFlowRate, matrix);
	}


	__device__ void assembleBoundaries(
		const Mesh<MeshDim::D3>* mesh,
		Field<double, C>* field,
		Field<V, C>* gradField,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<double>* matrix
	) override
	{
		static_cast<Derived*>(this)->
			template assembleBoundariesImpl<double, V>(
				mesh, field, gradField, massFlowRate, matrix);
	}

	__device__ void assembleBoundaries(
		const Mesh<MeshDim::D3>* mesh,
		Field<V, C>* field,
		Field<T, C>* gradField,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<V>* matrix
	) override
	{
		static_cast<Derived*>(this)->
			template assembleBoundariesImpl<V, T>(
				mesh, field, gradField, massFlowRate, matrix);
	}
};