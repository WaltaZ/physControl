#pragma once

#include <utility/cudaUtils.h>
#include <mesh/meshElements/meshElements.h>
#include <problem/field.h>
#include <geometry/vector.h>
#include <simulation/linearSolver/linearSolverMatrix.h>

class UnsteadyBase {
protected:
	using C = Cell<MeshDim::D3>;
	using V = Vector<GeometryDim::D3>;

public:
	__device__
	virtual void assemble(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<double, C>* field,
		CudaLinearSolverMatrix<double>* matrix,
		double delta_t
	) = 0;

	__device__
	virtual void assemble(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<V, C>* field,
		CudaLinearSolverMatrix<V>* matrix,
		double delta_t
	) = 0;
};

template<class Derived>
class Unsteady : public UnsteadyBase {
public:
	__device__
		void assemble(
			CudaMesh<MeshDim::D3>* mesh,
			CudaField<double, C>* field,
			CudaLinearSolverMatrix<double>* matrix,
			double delta_t
		) override 
	{
		static_cast<Derived*>(this)-> 
			template assembleImpl<double>(
				mesh,
				field,
				matrix,
				delta_t
			);
	};

	__device__
		void assemble(
			CudaMesh<MeshDim::D3>* mesh,
			CudaField<V, C>* field,
			CudaLinearSolverMatrix<V>* matrix,
			double delta_t
		) override 
	{
		static_cast<Derived*>(this)->
			template assembleImpl<V>(
				mesh,
				field,
				matrix,
				delta_t
			);
	};
};