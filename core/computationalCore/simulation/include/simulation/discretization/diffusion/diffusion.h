#pragma once

#include <cuda_runtime.h>
#include <mesh/meshElements/mesh.h>
#include <problem/field.h>
#include <simulation/linearSolver/linearSolverMatrix.h>

class DiffusionBase {
protected:
	using V = Vector<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;
public:
	__device__ virtual void assembleInner(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<double, C>* field,
		CudaLinearSolverMatrix<double>* matrix,
		double diffCoeff
	) = 0;

	__device__ virtual void assembleInner(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<V, C>* field,
		CudaLinearSolverMatrix<V>* matrix,
		double diffCoeff
	) = 0;


	__device__ virtual void assembleBoundaries(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<double, C>* field,
		CudaLinearSolverMatrix<double>* matrix,
		double diffCoeff
	) = 0;

	__device__ virtual void assembleBoundaries(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<V, C>* field,
		CudaLinearSolverMatrix<V>* matrix,
		double diffCoeff
	) = 0;
};

template<class Derived>
class Diffusion : public DiffusionBase {
public:
	__device__ void assembleInner(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<double, C>* field,
		CudaLinearSolverMatrix<double>* matrix,
		double diffCoeff
	) override 
	{
		static_cast<Derived*>(this)-> 
			template assembleInnerImpl<double>(
				mesh, field, matrix, diffCoeff);
	}

	__device__ void assembleInner(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<V, C>* field,
		CudaLinearSolverMatrix<V>* matrix,
		double diffCoeff
	) override
	{
		static_cast<Derived*>(this)-> 
			template assembleInnerImpl<V>(
				mesh, field, matrix, diffCoeff);
	}


	__device__ void assembleBoundaries(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<double, C>* field,
		CudaLinearSolverMatrix<double>* matrix,
		double diffCoeff
	) override 
	{
		static_cast<Derived*>(this)-> 
			template assembleBoundariesImpl<double>(
				mesh, field, matrix, diffCoeff);
	}

	__device__ void assembleBoundaries(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<V, C>* field,
		CudaLinearSolverMatrix<V>* matrix,
		double diffCoeff
	) override
	{
		static_cast<Derived*>(this)-> 
			template assembleBoundariesImpl<V>(
				mesh, field, matrix, diffCoeff);
	}
};