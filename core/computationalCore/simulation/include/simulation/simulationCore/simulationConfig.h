#pragma once

namespace simConfig {

	constexpr double G_CONSTANT = 9.8145; // [m/s^2]
	constexpr double DENSITY = 1.2041; // [kg/m^3]

	// THERMAL QUANTITIES --------------------------------------------------------------------------

	constexpr double THERM_CONDUCTIVITY = 0.02587; // [W / (m * K)]
	constexpr double SPECIFIC_HEAT = 1007; // [J / (kg * C)]
	constexpr double THERM_DIFFUSITY = THERM_CONDUCTIVITY / (SPECIFIC_HEAT * DENSITY); // [m^2 / s]
	constexpr double THERM_EXPANSION = 3.41e-3; // [1 / K]

	// MOMENTUM QUANTITIES -------------------------------------------------------------------------

	constexpr double VISCOSITY = 1.82e-5; // [Pa * s] add e-5
	constexpr double KINEMATIC_VISCOSITY = VISCOSITY / DENSITY; // [m^2 / s]

	// SIMULATION ----------------------------------------------------------------------------------

	constexpr double V_RELAXATION_FACTOR = 0.3;
	constexpr double P_RELAXATION_FACTOR = 0.3;
	constexpr double DT = 0.01;
}