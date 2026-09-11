#include <simulation/heatTransfer/simulationKernel.h>

#include <simulation/heatTransfer/simulationConfig.h>
#include <simulation/discretization/relaxation/relaxationPatankar.h>

namespace simKernel {

	using V = Vector<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;
	using F = Face<MeshDim::D3>;
	using M = MatrixTensor<GeometryDim::D3>;

	__global__
		void assembleInnerVelocity(
			const Mesh<MeshDim::D3>* mesh,
			Field<V, C>* velocity,
			Field<M, C>* gradVelocity,
			Field<double, C>* temperature,
			Field<double, F>* massFlowRate,
			Field<V, C>* gradPressure,
			LinearSolverMatrix<V>* matrix,
			HeatTransferSimulationMethods* methods
		) 
	{

		int C_id = blockDim.x * blockIdx.x + threadIdx.x;

		methods->diffusion->assembleInner(
			mesh,
			velocity,
			matrix,
			simConfig::KINEMATIC_VISCOSITY
		);

		/*methods->convection->assembleInner(
			mesh,
			velocity,
			gradVelocity,
			massFlowRate,
			matrix
		);*/

		methods->unsteady->assemble(
			mesh,
			velocity,
			matrix,
			simConfig::DT
		);

		/*methods->sourceGravity->assembleInner(
			mesh,
			matrix,
			V({0, 0, -simConfig::G_CONSTANT })
		);*/

		/*methods->sourceBoussinesq->assembleInner(
			mesh,
			matrix,
			V({ 0, 0, -simConfig::G_CONSTANT }),
			temperature,
			simConfig::THERM_EXPANSION
		);*/

		//matrix->B[C_id] += (-mesh->cells[C_id].volume) * gradPressure->values[C_id];

	};

	__global__
		void assembleBoundariesVelocity(
			const Mesh<MeshDim::D3>* mesh,
			Field<V, C>* velocity,
			Field<M, C>* gradVelocity,
			Field<double, C>* temperature,
			Field<double, F>* massFlowRate,
			Field<V, C>* gradPressure,
			LinearSolverMatrix<V>* matrix,
			HeatTransferSimulationMethods* methods
		) 
	{

		int C_id = blockDim.x * blockIdx.x + threadIdx.x;

		
	};

}