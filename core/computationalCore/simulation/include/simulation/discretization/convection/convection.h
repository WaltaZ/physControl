#pragma once

#include <cuda_runtime.h>
#include <mesh/meshElements/mesh.h>
#include <problem/field.h>
#include <simulation/linearSolver/linearSolverMatrix.h>

class ConvectionBase {
protected:
	using V = Vector<GeometryDim::D3>;
	using T = MatrixTensor<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;
	using F = Face<MeshDim::D3>;
public:
	__device__ virtual void assembleInner(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<double, C>* field,
		CudaField<V, C>* gradField,
		CudaField<double, F>* massFlowRate,
		CudaLinearSolverMatrix<double>* matrix
	) = 0;

	__device__ virtual void assembleInner(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<V, C>* field,
		CudaField<T, C>* gradField,
		CudaField<double, F>* massFlowRate,
		CudaLinearSolverMatrix<V>* matrix
	) = 0;


	__device__ virtual void assembleBoundaries(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<double, C>* field,
		CudaField<V, C>* gradField,
		CudaField<double, F>* massFlowRate,
		CudaLinearSolverMatrix<double>* matrix
	) = 0;

	__device__ virtual void assembleBoundaries(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<V, C>* field,
		CudaField<T, C>* gradField,
		CudaField<double, F>* massFlowRate,
		CudaLinearSolverMatrix<V>* matrix
	) = 0;
};

template<class Derived>
class Convection : public ConvectionBase {
public:
	__device__ void assembleInner(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<double, C>* field,
		CudaField<V, C>* gradField,
		CudaField<double, F>* massFlowRate,
		CudaLinearSolverMatrix<double>* matrix
	) override
	{
		static_cast<Derived*>(this)->
			template assembleInnerImpl<double, V>(
				mesh, field, gradField, massFlowRate, matrix);
	}

	__device__ void assembleInner(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<V, C>* field,
		CudaField<T, C>* gradField,
		CudaField<double, F>* massFlowRate,
		CudaLinearSolverMatrix<V>* matrix
	) override
	{
		static_cast<Derived*>(this)->
			template assembleInnerImpl<V, T>(
				mesh, field, gradField, massFlowRate, matrix);
	}


	__device__ void assembleBoundaries(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<double, C>* field,
		CudaField<V, C>* gradField,
		CudaField<double, F>* massFlowRate,
		CudaLinearSolverMatrix<double>* matrix
	) override
	{
		static_cast<Derived*>(this)->
			template assembleBoundariesImpl<double, V>(
				mesh, field, gradField, massFlowRate, matrix);
	}

	__device__ void assembleBoundaries(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<V, C>* field,
		CudaField<T, C>* gradField,
		CudaField<double, F>* massFlowRate,
		CudaLinearSolverMatrix<V>* matrix
	) override
	{
		static_cast<Derived*>(this)->
			template assembleBoundariesImpl<V, T>(
				mesh, field, gradField, massFlowRate, matrix);
	}
};