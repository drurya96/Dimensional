#ifndef STATIC_DIMENSION_CONSTANTS_H
#define STATIC_DIMENSION_CONSTANTS_H

// Fundamental Dimensions
#include "AmountDimension.h"
#include "AngleDimension.h"
#include "LengthDimension.h"
#include "MassDimension.h"
#include "TemperatureDimension.h"
#include "TimeDimension.h"

// Derived Dimensions
#include "AccelerationDimension.h"
#include "EnergyDimension.h"
#include "ForceDimension.h"
#include "PressureDimension.h"
#include "SpeedDimension.h"
#include "VolumeDimension.h"

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

}

#endif // STATIC_DIMENSION_CONSTANTS_H
