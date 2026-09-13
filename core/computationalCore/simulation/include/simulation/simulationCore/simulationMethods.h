#pragma once

#include <simulation/discretization/diffusion/diffusion.h>
#include <simulation/discretization/gradient/gradient.h>
#include <simulation/discretization/convection/convection.h>
#include <simulation/discretization/unsteady/unsteady.h>
#include <simulation/discretization/source/sourceGravity.h>
#include <simulation/discretization/source/sourceBoussinesq.h>
#include <simulation/linearSolver/linearSolver.h>


struct HeatTransferSimulationMethods {
	GradientBase* gradient;
	DiffusionBase* diffusion;
	ConvectionBase* convection;
	UnsteadyBase* unsteady;
	SourceGravity* sourceGravity;
	SourceBoussinesq* sourceBoussinesq;

	LinearSolverFactory* solverFactory;

	HeatTransferSimulationMethods(
		GradientBase* gradient,
		DiffusionBase* diffusion,
		ConvectionBase* convection,
		UnsteadyBase* unsteady,
		LinearSolverFactory* solverFactory
	);

	~HeatTransferSimulationMethods();

	HeatTransferSimulationMethods(
		HeatTransferSimulationMethods&& other);
};