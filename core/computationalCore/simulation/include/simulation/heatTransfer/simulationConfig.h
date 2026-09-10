#pragma once

namespace simConfig {

	constexpr double G_CONSTANT = 9.8145; // [m/s^2]
	constexpr double DENSITY = 1.2041; // [kg/m^3]

	// THERMAL QUANTITIES --------------------------------------------------------------------------

	constexpr double THERM_CONDUCTIVITY = 0.02587; // [W / (m * K)]
	constexpr double SPECIFIC_HEAT = 1007; // [J / (kg * C)]
	constexpr double THERM_DIFFUSITY = THERM_CONDUCTIVITY / (SPECIFIC_HEAT * DENSITY); // [m^2 / s]

	// MOMENTUM QUANTITIES -------------------------------------------------------------------------

	constexpr double VISCOSITY = 1.82e-5; // [Pa * s]
	constexpr double KINEMATIC_VISCOSITY = VISCOSITY / DENSITY; // [m^2 / s]
}