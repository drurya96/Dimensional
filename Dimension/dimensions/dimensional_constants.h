#ifndef STATIC_DIMENSION_CONSTANTS_H
#define STATIC_DIMENSION_CONSTANTS_H

/*
// Fundamental Dimensions
#include "amount_dimension.h"
#include "angle_dimension.h"
#include "charge_dimension.h"
#include "length_dimension.h"
#include "mass_dimension.h"
#include "temperature_dimension.h"
#include "timespan_dimension.h"

// Derived Dimensions
#include "acceleration_dimension.h"
#include "pressure_dimension.h"
#include "speed_dimension.h"
#include "energy_dimension.h"
*/

#include "dimensions.h"

namespace dimension::constants
{

    // Reminder: The units chosen for these constants are based on typical usage.
    // However, the same constant will work in other systems of measurement without adjustement.
    // Unit conversions are implicit, so even if these units don't match the use-case needed, they will still behave as expected.

    // Constant values have **NOT YET BEEN THOROUGHLY VERIFIED!**
    // If a constant is found to be incorrect, please open either an issue or an MR fixing the value

    static constexpr base_dimension<unit_exponent<joules>, unit_exponent<seconds>> plancks_constant(6.62607015e-34);

    static constexpr base_dimension<unit_exponent<meters, 3>, unit_exponent<kilo_grams, -1>, unit_exponent<seconds, -2>> gravitational_constant(6.67430e-11);

    static constexpr base_dimension<unit_exponent<moles, -1>> avogadro_number(6.02214076e23);

    static constexpr base_dimension<unit_exponent<joules>, unit_exponent<kelvin, -1>> boltzmann_constant(1.380649e-23);

    static constexpr speed<meters, seconds> speed_of_light(299792458.0);

    static constexpr base_dimension<unit_exponent<joules>,unit_exponent<kelvin, -1>, unit_exponent<moles, -1>> ideal_gas_constant(8.31446261815324);

    static constexpr base_dimension<unit_exponent<joules>, unit_exponent<seconds, -1>, unit_exponent<meters, -2>, unit_exponent<kelvin, -4>> stefan_boltzmann_constant(5.670374419e-8);

    static constexpr length<meters> earth_mean_radius(6371008.8);

    static constexpr acceleration<meters, seconds> earth_gravity(9.80665);

    static constexpr pressure<pascals> standard_atmospheric_pressure(101325.0);

    static constexpr temperature<kelvin> standard_temperature(273.15);

    static constexpr base_dimension<unit_exponent<meters, 3>, unit_exponent<seconds, -2>> universal_gravitational_parameter(3.986004418e14);

    static constexpr base_dimension<unit_exponent<joules>, unit_exponent<kelvin, -1>, unit_exponent<grams, -1>> specific_heat_water(4.184);

    static constexpr base_dimension<unit_exponent<joules>, unit_exponent<kilo_grams, -1>> latent_heat_vaporization_water(2.26e6);

    static constexpr base_dimension<unit_exponent<newtons>, unit_exponent<meters>> surface_tension_water(0.0728);

    static constexpr base_dimension<unit_exponent<meters, 3>, unit_exponent<moles, -1>> molar_volume_stp(0.022414);

    static constexpr base_dimension<unit_exponent<joules>, unit_exponent<kilo_grams, -1>, unit_exponent<kelvin, -1>> entropy_vaporization_water(1000.0);

    static constexpr base_dimension<unit_exponent<meters, -1>> rydberg_constant(1.0973731568160e7);

    static constexpr base_dimension<unit_exponent<coulombs>, unit_exponent<moles, -1>> faraday(9.64853321233100184e4);
    
    static constexpr base_dimension<unit_exponent<coulombs, 2>, unit_exponent<newtons, -1>, unit_exponent<meters, -2>> permittivity_vacuum(8.854187817e-12);

    static constexpr base_dimension<unit_exponent<newtons>, unit_exponent<coulombs, -2>> permeability_vacuum(4 * std::numbers::pi * 1e-7);

    static constexpr base_dimension<unit_exponent<newtons>, unit_exponent<seconds>, unit_exponent<coulombs, -2>> vacuum_impedance(376.730313668);

    static constexpr base_dimension<unit_exponent<newtons>, unit_exponent<meters, 2>, unit_exponent<coulombs, -2>> coulombs_constant(8.9875517923e9);

    static constexpr base_dimension<unit_exponent<joules>, unit_exponent<newtons, -1>, unit_exponent<meters, -1>, unit_exponent<coulombs, -1>> bohr_magneton(9.2740100783e-24);

    static constexpr mass<pico_grams> electron_mass(9.1093837015e-16);

    static constexpr mass<pico_grams> proton_mass(1.67262192369e-12);

    static constexpr energy<joules> electron_volt(1.602176634e-19);

}

#endif // STATIC_DIMENSION_CONSTANTS_H
