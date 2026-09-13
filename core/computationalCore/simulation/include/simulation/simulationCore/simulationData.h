#pragma once

#include <problem/problem.h>
#include "simulationMethods.h"

struct SimulationData {
	HeatTransferProblemD3& problem;
	const Mesh<MeshDim::D3>* mesh;
	HeatTransferSimulationMethods* methods;
};