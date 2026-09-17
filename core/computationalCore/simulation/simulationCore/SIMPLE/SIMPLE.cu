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
	_gradPressureCorr->initPastTrace(1);

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

	/*for (size_t i = 0; i < data.mesh->cells.length; i++)
	{
		const auto& C = data.mesh->cells[i];
		printf("Cell %d | Face ids: ", i);
		for (size_t j = 0; j < C.cellFaceIDs.length; j++)
		{
			printf("%d, ", C.cellFaceIDs[j]);
		}
		printf("\n");
	}*/

	for (size_t iter = 0; iter < 10; iter++)
	{
		printf("\n===================== ITERATION %d =======================\n\n", iter);

		printf("Calculating pressure gradient\n");
		data.methods->gradient->compute(
			fields->pressure,
			fields->gradPressure,
			data.mesh
		);

		printf("Assembling velocity matrix (inner)\n");
		KernelArgs args_EC = cudaUtils::getKernelArgs(numOfCells);
		SIMPLEKernel::assembleInnerVelocity << <args_EC.blocks, args_EC.threads >> > (
			data.mesh,
			fields,
			_velocitySolver->matrix,
			data.methods
			);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		printf("Assembling velocity matrix (boundaries)\n");
		KernelArgs args = cudaUtils::getKernelArgs(fields->velocity->bpFaceIDsLengthMax);
		SIMPLEKernel::assembleBoundariesVelocity << <args.blocks, args.threads >> > (
			data.mesh,
			fields,
			_velocitySolver->matrix
			);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		printf("Solving for velocity field\n");
		_velocitySolver->solve();

		printf("Updating mass flow rate\n");
		KernelArgs args_EF = cudaUtils::getKernelArgs(numOfFaces);
		SIMPLEKernel::updateMassFlow << <args_EF.blocks, args_EF.threads >> > (
			data.mesh,
			fields,
			_velocitySolver->matrix
			);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		printf("Assembling pressure correction matrix\n");
		SIMPLEKernel::assemblePressure << <args_EC.blocks, args_EC.threads >> > (
			data.mesh,
			fields,
			_pressureCorrSolver->matrix,
			_velocitySolver->matrix
			);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		printf("Solving for pressure correction field\n");
		_pressureCorrSolver->solve();

		//debug::printField(_pressureCorr);

		printf("Calculating pressure correction gradient\n");
		data.methods->gradient->compute(
			_pressureCorr,
			_gradPressureCorr,
			data.mesh);

		printf("Correcting velocity and pressure\n");
		SIMPLEKernel::correctPressureAndVelocity<<<args_EC.blocks, args_EC.threads>>>(
			data.mesh,
			fields,
			_pressureCorr,
			_gradPressureCorr,
			_velocitySolver->matrix
		);

		printf("Correcting mass flow rate\n");
		SIMPLEKernel::correctMassFlow << <args_EF.blocks, args_EF.threads >> > (
			data.mesh,
			fields,
			_pressureCorr,
			_gradPressureCorr,
			_velocitySolver->matrix
		);

		printf("Reseting matrices\n");
		SIMPLEKernel::resetMatrix <<< args_EC.blocks, args_EC.threads >>> (_velocitySolver->matrix);
		SIMPLEKernel::resetMatrix <<< args_EC.blocks, args_EC.threads >>> (_pressureCorrSolver->matrix);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		_gradPressureCorr->moveTraceToNextStep();

		// debug
		for (size_t i = 0; i < data.mesh->cells.length; i++)
		{
			const auto& C = data.mesh->cells[i];

			double sum = 0;

			for (size_t j = 0; j < C.cellFaceIDs.length; j++)
			{
				double m_f = fields->massFlowRate->values[C.cellFaceIDs[j]];
				if (data.mesh->faces[C.cellFaceIDs[j]].neighbourCellID == i) { m_f = -m_f; }

				sum += m_f;
			}
			printf("Cell: %d | Sum of all mass fluxes: %.10lf\n", i, sum);
		}
		printf("Iteration done!\n");
	 }
};

/* TO TEST SOLVER \/
for (size_t i = 0; i < 8; i++)
	{
		_pressureCorrSolver->matrix->B[i] = i + 1;
		_pressureCorrSolver->matrix->A_C[i] = 10;
		_pressureCorrSolver->matrix->A_F[i][0] = 1;
		_pressureCorrSolver->matrix->A_F[i][1] = -1;
		_pressureCorrSolver->matrix->A_F[i][2] = 1;
	}

	_pressureCorrSolver->matrix->A_F[0][0] = 0.5;
	_pressureCorrSolver->matrix->A_F[0][1] = -0.7;
	_pressureCorrSolver->matrix->A_F[0][2] = 0.2;

	_pressureCorrSolver->matrix->A_F[7][0] = 0.25;
	_pressureCorrSolver->matrix->A_F[7][1] = -0.8;
	_pressureCorrSolver->matrix->A_F[7][2] = 0.1;

	for (size_t i = 0; i < 8; i++)
	{
		const auto& a = data.mesh->cells[i].cellNeighbourCells;

		printf("[ ");
		for (size_t j = 0; j < 8; j++)
		{
			double num2Print = 0;
			if (j == i) {
				num2Print = _pressureCorrSolver->matrix->A_C[i];
			}
			else {
				for (size_t k = 0; k < 3; k++)
				{
					if (j == a[k]) {
						num2Print = _pressureCorrSolver->matrix->A_F[i][k];
						break;
					}
				}
			}
			printf("%lf, ", num2Print);
		}
		printf(" ]\n");
	}

	_pressureCorrSolver->solve();
	debug::printField(data.mesh, _pressureCorr);

*/