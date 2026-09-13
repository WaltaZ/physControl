#include <simulation/simulationCore/SIMPLE/SIMPLE.h>

#include <simulation/simulationCore/SIMPLE/SIMPLEKernel.h>

SIMPLE::SIMPLE(const SimulationData& data) : 
	data(data),
	_pressureCorr(
		cudaUtils::create<Field<double, Cell<MeshDim::D3>>>()
	),
	_gradPressureCorr(
		cudaUtils::create<Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>>()
	)
{
	auto& fields = data.problem.fields;

	_pressureCorr->initField(data.mesh->cells);
	_gradPressureCorr->initField(data.mesh->cells);

	_velocitySolver =
		data.methods->solverFactory->getSolver(data.mesh, fields->velocity);

	_pressureCorrSolver =
		data.methods->solverFactory->getSolver(data.mesh, _pressureCorr);
};

void SIMPLE::nextStep()
{
	uint32_t numOfCells = data.mesh->cells.length;
	uint32_t numOfFaces = data.mesh->faces.length;

	auto& fields = data.problem.fields;

	fields->velocity->moveTraceToNextStep();
	fields->temperature->moveTraceToNextStep();
	fields->temperature->updateMaxMin();

	for (size_t iter = 0; iter < 10; iter++)
	{
		data.methods->gradient->compute(
			fields->pressure,
			fields->gradPressure,
			data.mesh
		);

		KernelArgs args_EC = cudaUtils::getKernelArgs(numOfCells);
		SIMPLEKernel::assembleInnerVelocity << <args_EC.blocks, args_EC.threads >> > (
			data.mesh,
			fields,
			_velocitySolver->matrix,
			data.methods
			);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		KernelArgs args = cudaUtils::getKernelArgs(fields->velocity->bpFaceIDsLengthMax);
		SIMPLEKernel::assembleBoundariesVelocity << <args.blocks, args.threads >> > (
			data.mesh,
			fields,
			_velocitySolver->matrix
			);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		_velocitySolver->solve();

		KernelArgs args_EF = cudaUtils::getKernelArgs(numOfFaces);
		SIMPLEKernel::updateMassFlow << <args_EF.blocks, args_EF.threads >> > (
			data.mesh,
			fields,
			_velocitySolver->matrix
			);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		//debug::printField(mesh, fields.massFlowRate);

		SIMPLEKernel::assemblePressure << <args_EC.blocks, args_EC.threads >> > (
			data.mesh,
			fields,
			_pressureCorrSolver->matrix,
			_velocitySolver->matrix
			);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		_pressureCorrSolver->solve();

		data.methods->gradient->compute(
			_pressureCorr,
			_gradPressureCorr,
			data.mesh);

		SIMPLEKernel::correctPressureAndVelocity<<<args_EC.blocks, args_EC.threads>>>(
			data.mesh,
			fields,
			_pressureCorr,
			_gradPressureCorr,
			_velocitySolver->matrix
		);

		SIMPLEKernel::correctMassFlow<<<args_EF.blocks, args_EF.threads>>>(
			data.mesh, 
			fields, 
			_pressureCorr, 
			_gradPressureCorr, 
			_velocitySolver->matrix
		);
		cudaUtils::fetchError(cudaDeviceSynchronize);
	}
};