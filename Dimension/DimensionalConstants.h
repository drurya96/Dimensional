#ifndef STATIC_DIMENSION_CONSTANTS_H
#define STATIC_DIMENSION_CONSTANTS_H

// Fundamental Dimensions
#include "AmountDimension.h"
#include "AngleDimension.h"
#include "ChargeDimension.h"
#include "LengthDimension.h"
#include "MassDimension.h"
#include "TemperatureDimension.h"
#include "TimeDimension.h"

// Derived Dimensions
#include "AccelerationDimension.h"
#include "AreaDimension.h"
#include "CapacitanceDimension.h"
#include "ConductanceDimension.h"
#include "CurrentDimension.h"
#include "ElectricFieldDimension.h"
#include "EnergyDimension.h"
#include "ForceDimension.h"
#include "FrequencyDimension.h"
#include "InductanceDimension.h"
#include "MagneticFieldDimension.h"
#include "MagneticFluxDimension.h"
#include "MomentumDimension.h"
#include "PowerDimension.h"
#include "PressureDimension.h"
#include "ResistanceDimension.h"
#include "SpeedDimension.h"
#include "ElectricPotentialDimension.h"
#include "VolumeDimension.h"

#include "EntropyDimension.h"
#include "HeatFluxDimension.h"
#include "SpecificHeatCapacityDimension.h"
#include "DiffusionCoefficientDimension.h"
#include "DynamicViscosityDimension.h"
#include "DensityDimension.h"
#include "VolumetricFlowRateDimension.h"
#include "MassFlowRateDimension.h"
#include "SpecificVolumeDimension.h"
#include "MomentOfInertiaDimension.h"
#include "AngularSpeedDimension.h"
#include "AngularAccelerationDimension.h"

namespace Dimension::Constants
{

    // Reminder: The units chosen for these constants are based on typical usage.
    // However, the same constant will work in other systems of measurement without adjustement.
    // Unit conversions are implicit, so even if these units don't match the use-case needed, they will still behave as expected.

    // Constant values have **NOT YET BEEN THOROUGHLY VERIFIED!**
    // If a constant is found to be incorrect, please open either an issue or an MR fixing the value

    static constexpr BaseDimension<std::tuple<Joules, Seconds>, std::tuple<>> plancks_constant(6.62607015e-34);

    static constexpr BaseDimension<std::tuple<Meters, Meters, Meters>, std::tuple<KiloGrams, Seconds, Seconds>> gravitational_constant(6.67430e-11);

    static constexpr BaseDimension<std::tuple<>, std::tuple<Moles>> avogadro_number(6.02214076e23);

    static constexpr BaseDimension<std::tuple<Joules>, std::tuple<Quantity<Kelvin>>> boltzmann_constant(1.380649e-23);

    static constexpr BaseDimension<std::tuple<Meters>, std::tuple<Seconds>> speed_of_light(299792458.0);

    static constexpr BaseDimension<std::tuple<Joules>, std::tuple<Quantity<Kelvin>, Moles>> ideal_gas_constant(8.31446261815324);

    static constexpr BaseDimension<std::tuple<Joules>, std::tuple<Seconds, Meters, Meters, Quantity<Kelvin>, Quantity<Kelvin>, Quantity<Kelvin>, Quantity<Kelvin>>> stefan_boltzmann_constant(5.670374419e-8);

    static constexpr BaseDimension<std::tuple<Meters>, std::tuple<>> earth_mean_radius(6371008.8);

    static constexpr BaseDimension<std::tuple<Meters>, std::tuple<Seconds, Seconds>> earth_gravity(9.80665);

    static constexpr BaseDimension<std::tuple<Pascals>, std::tuple<>> standard_atmospheric_pressure(101325.0);

    static constexpr BaseDimension<std::tuple<Quantity<Kelvin>>, std::tuple<>> standard_temperature(273.15);

    static constexpr BaseDimension<std::tuple<Meters, Meters, Meters>, std::tuple<Seconds, Seconds>> universal_gravitational_parameter(3.986004418e14);

    static constexpr BaseDimension<std::tuple<Joules>, std::tuple<Kelvin, Grams>> specific_heat_water(4.184);

    static constexpr BaseDimension<std::tuple<Joules>, std::tuple<KiloGrams>> latent_heat_vaporization_water(2.26e6);

    static constexpr BaseDimension<std::tuple<Newtons, Meters>, std::tuple<>> surface_tension_water(0.0728);

    static constexpr BaseDimension<std::tuple<Meters, Meters, Meters>, std::tuple<Moles>> molar_volume_stp(0.022414);

    static constexpr BaseDimension<std::tuple<Joules>, std::tuple<KiloGrams, Quantity<Kelvin>>> entropy_vaporization_water(1000.0);

    static constexpr BaseDimension<std::tuple<>, std::tuple<Meters>> rydberg_constant(1.0973731568160e7);

    static constexpr BaseDimension<std::tuple<Coulombs>, std::tuple<Moles>> faraday(9.64853321233100184e4);
    
    static constexpr BaseDimension<std::tuple<Coulombs, Coulombs>, std::tuple<Newtons, Meters, Meters>> permittivity_vacuum(8.854187817e-12);

    static constexpr BaseDimension<std::tuple<Newtons>, std::tuple<Coulombs, Coulombs>> permeability_vacuum(4 * std::numbers::pi * 1e-7);

    static constexpr BaseDimension<std::tuple<Newtons, Seconds>, std::tuple<Coulombs, Coulombs>> vacuum_impedance(376.730313668);

    static constexpr BaseDimension<std::tuple<Newtons, Meters, Meters>, std::tuple<Coulombs, Coulombs>> coulombs_constant(8.9875517923e9);

    static constexpr BaseDimension<std::tuple<Joules>, std::tuple<Newtons, Meters, Coulombs>> bohr_magneton(9.2740100783e-24);

    static constexpr BaseDimension<std::tuple<PicoGrams>, std::tuple<>> electron_mass(9.1093837015e-16);

    static constexpr BaseDimension<std::tuple<PicoGrams>, std::tuple<>> proton_mass(1.67262192369e-12);

    static constexpr BaseDimension<std::tuple<Joules>, std::tuple<>> electron_volt(1.602176634e-19);

}

#endif // STATIC_DIMENSION_CONSTANTS_H
