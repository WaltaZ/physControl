#pragma once

#include <simulation/simulationCore/simulationData.h>

class SIMPLE {
public:
	void nextStep();
	SIMPLE(const SimulationData& data);

private:
	SimulationData data;

	std::unique_ptr<LinearSolver<Vector<GeometryDim::D3>>> _velocitySolver;
	std::unique_ptr<LinearSolver<double>> _pressureCorrSolver;

	Field<double, Cell<MeshDim::D3>>* _pressureCorr;
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* _gradPressureCorr;
};