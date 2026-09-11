#pragma once

#include <mesh/meshElements/meshElements.h>
#include <problem/problem.h>
#include <geometry/geometryUtils.h>
#include <utility/cudaUtils.h>

#include "simulationMethods.h"

class HeatTransferSimulationD3 {
public:
	HeatTransferProblemD3& problem;
	const Mesh<MeshDim::D3>* mesh;
	HeatTransferSimulationMethods* methods;

	HeatTransferSimulationD3(
		HeatTransferProblemD3& problem,
		const Mesh<MeshDim::D3>* mesh,
		HeatTransferSimulationMethods&& methods
	);

	void nextStep();
};