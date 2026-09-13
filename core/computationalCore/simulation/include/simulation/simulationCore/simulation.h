#pragma once

#include <mesh/meshElements/meshElements.h>
#include <problem/problem.h>
#include <geometry/geometryUtils.h>
#include <utility/cudaUtils.h>

#include "simulationData.h"

#include "SIMPLE/SIMPLE.h"

class HeatTransferSimulationD3 {
public:
	SimulationData data;

	HeatTransferSimulationD3(
		HeatTransferProblemD3& problem,
		const Mesh<MeshDim::D3>* mesh,
		HeatTransferSimulationMethods&& methods
	);

	void nextStep();

private:
	SIMPLE _simple;
};