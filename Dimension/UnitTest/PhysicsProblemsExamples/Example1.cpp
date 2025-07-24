#include "PhysicsProblemsBase.h"

#include <ostream>
#include <iostream>

template<typename T>
using temperature_point = dimension::point<T, dimension::temperatureType>;


TEST(PhysicsProblemsExample1, HeatTransfer)
{
    using namespace dimension;

    // Given values
    temperature_point<kelvin> startingTemp{100};
    temperature_point<celsius> endingTemp{26.85};

    temperature deltaTemp = endingTemp - startingTemp;

    mass<kilo_grams> mass(2.0);

    energy<joules> heat = mass * constants::specific_heat_water * deltaTemp;

    double expected_heat = 1673600;

    ASSERT_NEAR(get_energy_as<joules>(heat), expected_heat, TOLERANCE);
}


TEST(PhysicsProblemsExample1, KinematicsDisplacement)
{
    using namespace dimension;

    // Given values
    speed<meters, seconds> initialVelocity{10.0}; // Initial speed = 10 m/s
    acceleration<meters, seconds> acceleration{2.0}; // acceleration = 2 m/s²
    timespan<seconds> time{5.0}; // timespan = 5 seconds

    // Calculate displacement using s = v0 * t + (1/2) * a * t^2
    length<meters> displacement1 = initialVelocity * time;
    length<meters> displacement2 = 0.5 * acceleration * time * time;

    length<meters> totalDisplacement = displacement1 + displacement2;

    // Expected result: s = (10 * 5) + (0.5 * 2 * 5^2) = 50 + 25 = 75 meters
    ASSERT_NEAR(get_length_as<meters>(totalDisplacement), 75.0, TOLERANCE);
}

TEST(PhysicsProblemsExample1, WorkDoneByforce)
{
    using namespace dimension;

    // Given values
    force<newtons> force{50.0}; // force = 50 N (kg·m/s²)
    length<meters> displacement{10.0};                      // Displacement = 10 m
    angle<radians> angle{std::numbers::pi / 3};                         // angle = 60 degrees (in radians)

    // Calculate work done: W = F * d * cos(theta)
    energy<joules> workDone = force * displacement * dimension::cos(angle);

    // Expected result: W = 50 * 10 * cos(60°) = 50 * 10 * 0.5 = 250 J
    ASSERT_NEAR((get_energy_as<joules>(workDone)), 250.0, TOLERANCE);
}

TEST(PhysicsProblemsExample1, ProjectileMotion)
{
    using namespace dimension;

    // Given values
    speed<meters, seconds> initialVelocity{50.0};  // Initial velocity = 50 m/s
    angle<radians> launchangle{std::numbers::pi / 4};          // Launch angle = 45 degrees (in radians)
    acceleration<meters, seconds> gravity{9.81}; // Gravitational acceleration = 9.81 m/s²

    // Calculate timespan of Flight: T = (2 * v0 * sin(angle)) / g
    timespan<seconds> timeOfFlight = (2.0 * initialVelocity * sin(launchangle)) / gravity;

    // Calculate Horizontal Range: R = v0 * cos(angle) * T
    length<meters> horizontalRange = initialVelocity * cos(launchangle) * timeOfFlight;

    // Expected results
    double expectedtimespanOfFlight = (2.0 * 50.0 * std::sin(std::numbers::pi / 4)) / 9.81; // ~ 7.208 s
    double expectedRange = 50.0 * std::cos(std::numbers::pi / 4) * expectedtimespanOfFlight; // ~ 255.1 m

    // Assertions
    ASSERT_NEAR(get_timespan_as<seconds>(timeOfFlight), expectedtimespanOfFlight, TOLERANCE);
    ASSERT_NEAR(get_length_as<meters>(horizontalRange), expectedRange, TOLERANCE);
}

TEST(PhysicsProblemsExample1, powerFromWorkAndtimespan)
{
    using namespace dimension;

    // Given values
    energy<joules> work{500.0}; // Work = 500 J
    timespan<seconds> time{10.0};                                        // timespan = 10 s

    /// @todo Update this with a Work dimension when available
    // Calculate power: P = W / t
    power power = work / time;

    // Expected result
    double expectedpower = 500.0 / 10.0; // 50.0 W

    // Assertions
    ASSERT_NEAR((get_power_as<kilo_grams, meters, seconds>(power)), expectedpower, TOLERANCE);

}

TEST(PhysicsProblemsExample1, densityCalculation)
{
    using namespace dimension;

    // Given values
    mass<kilo_grams> mass{500.0};                      // mass = 500 kg
    length<meters> len{2.0};                       // length = 2 m
    length<meters> width{1.0};                        // Width = 1 m
    length<meters> height{0.5};                       // Height = 0.5 m

    // Calculate volume: V = length * width * height
    volume<meters> volume = len * width * height;

    // Calculate density: ρ = mass / volume
    density<kilo_grams, meters> density = mass / volume;

    // Expected results
    double expectedvolume = 2.0 * 1.0 * 0.5; // 1.0 m³
    double expecteddensity = 500.0 / expectedvolume; // 500.0 kg/m³

    // Assertions
    ASSERT_NEAR((get_volume_as<meters>(volume)), expectedvolume, TOLERANCE);
    ASSERT_NEAR((get_density_as<kilo_grams, meters>(density)), expecteddensity, TOLERANCE);
}

TEST(PhysicsProblemsExample1, pressureFromforceAndvolume)
{
    using namespace dimension;

    // Given values
    force<newtons> force{500.0};                   // force = 500 N
    length<meters> len{4.0};                    // length = 4 m
    length<meters> width{2.0};                     // Width = 2 m
    length<meters> height{0.5};                    // Height = 0.5 m

    // Step 1: Calculate volume
    volume<meters> volume = len * width * height;

    // Step 2: Calculate area (cross-section): A = V / h
    area<meters> area = volume / height;

    // Step 3: Calculate pressure: P = F / A
    pressure<kilo_grams, meters, seconds> pressure = force / area;

    // Expected results
    double expectedvolume = 4.0 * 2.0 * 0.5;       // 4 m * 2 m * 0.5 m = 4.0 m³
    double expectedarea = expectedvolume / 0.5;   // Cross-section: V / h = 8.0 m²
    double expectedpressure = 500.0 / expectedarea; // P = F / A = 500 / 8.0 = 62.5 N/m²

    // Assertions
    ASSERT_NEAR((get_volume_as<meters>(volume)), expectedvolume, TOLERANCE);
    ASSERT_NEAR((get_area_as<meters>(area)), expectedarea, TOLERANCE);
    ASSERT_NEAR((get_pressure_as<kilo_grams, meters, seconds>(pressure)), expectedpressure, TOLERANCE);

}

TEST(PhysicsProblemsExample1, WaterBoilingtemperatureChange)
{
    using namespace dimension;

    // Given values
    temperature_point<celsius> initialTemp{25.0}; // Initial temperature in celsius
    temperature_point<celsius> boilingPoint{100.0}; // Boiling point of water in celsius

    // Calculate the temperature difference
    temperature deltaTemp = boilingPoint - initialTemp;

    // Validate in celsius
    double expectedDeltaTempC = 75.0; // 100 - 25
    ASSERT_NEAR(get_temperature_as<kelvin>(deltaTemp), expectedDeltaTempC, TOLERANCE);

    // Convert delta to Fahrenheit and validate
    double expectedDeltaTempF = 75.0 * 9.0 / 5.0;  // = 135.0
    ASSERT_NEAR(get_temperature_as<rankine>(deltaTemp), expectedDeltaTempF, TOLERANCE);
}

TEST(PhysicsProblemsExample1, energyRequiredToHeatWater)
{
    using namespace dimension;

    // Given values
    volume<liters> volumeOfWater{2.0};                     // volume in liters
    //base_dimension<unit_exponent<kilo_grams>, unit_exponent<liters, -1>> waterdensity(1.0);
    density<kilo_grams, deci_meters> waterdensity(1.0); // TODO: Not an ideal signature for density... I'd like to use density<kilo_grams, liters>
    mass<kilo_grams> mass = volumeOfWater * waterdensity;            // density of water = 1 kg/L
    temperature_point<celsius> initialTemp{20.0};      // Initial temperature in celsius
    temperature_point<celsius> finalTemp{80.0};        // Final temperature in celsius
    
    // Calculate the temperature difference
    temperature deltaTemp = finalTemp - initialTemp;

    // Calculate the energy required
    energy energy = mass * constants::specific_heat_water * deltaTemp;

    // Validate the result in joules
    double expectedenergyJ = 2.0 * 1000.0 * 4.184 * 60.0; // 2 kg * 1000 g/kg * 4.184 J/g°C * 60°C
    ASSERT_NEAR(get_energy_as<joules>(energy), expectedenergyJ, TOLERANCE);

    // Convert to calories and validate
    double expectedenergyCal = expectedenergyJ / 4.184; // ~286.47 calories
    ASSERT_NEAR(get_energy_as<calories>(energy), expectedenergyCal, TOLERANCE);
    
}

TEST(PhysicsProblemsExample1, PVEqualsnRT) {
    using namespace dimension;

    // Define the quantities
    volume<meters> V{0.02241404394303833}; // Value calculated manually
    amount<moles> n{1.0};

    // Compute nRT
    auto nRT = n * constants::ideal_gas_constant * constants::standard_temperature;

    // Compute PV
    auto PV = constants::standard_atmospheric_pressure * V;

    // Compare PV and nRT
    ASSERT_NEAR(get_energy_as<joules>(PV), get_energy_as<joules>(nRT), TOLERANCE);
}

TEST(PhysicsProblemsExample1, PVEqualsnRT_temperatureIncelsius) {
    using namespace dimension;
    // Define the quantities
    pressure<pascals> P{202650};
    volume<meters> V{0.02446548490432685}; // Calculated volume for PV = nRT with n=2 mol, T=25°C
    amount<moles> n{2.0}; // 2 moles
    temperature_point<celsius> T{25.0}; // 25°C, should convert to 298.15 K

    // Compute nRT
    auto nRT = n * constants::ideal_gas_constant * T; // Should be approximately 2 * 8.314 * 298.15 ≈ 4,957.6382 J

    // Compute PV
    auto PV = P * V; // Should be approximately 2 * 101325 * 0.0244640424377005 ≈ 4,957.6382 J

    // Compare PV and nRT
    ASSERT_NEAR(get_energy_as<joules>(PV), get_energy_as<joules>(nRT), 1e-1);
}

TEST(PhysicsProblemsExample1, VerifyFaraday) {
    using namespace dimension;

    auto chargePerMole = charge<elementary_charges>(1.0) * constants::avogadro_number;

    double val_chargePerMole = get_dimension_as<unit_exponent<coulombs>, unit_exponent<moles, -1>>(chargePerMole);
    double val_faraday = get_dimension_as<unit_exponent<coulombs>, unit_exponent<moles, -1>>(constants::faraday);

    ASSERT_NEAR(val_chargePerMole, val_faraday, 1e-1);

}

TEST(PhysicsProblemsExample1, areaCalculation)
{
    using namespace dimension;

    length<meters> len{5.0};
    length<meters> width{3.0};

    area<meters> area = len * width;

    ASSERT_NEAR((get_area_as<meters>(area)), 15.0, TOLERANCE);
}

TEST(PhysicsProblemsExample1, frequencyFromPeriod)
{
    using namespace dimension;

    timespan<seconds> period{2.0}; // 2 seconds
    frequency<hertz> frequency = 1.0 / period;

    ASSERT_NEAR((get_frequency_as<hertz>(frequency)), 0.5, TOLERANCE); // 0.5 Hz
}

TEST(PhysicsProblemsExample1, momentumCalculation)
{
    using namespace dimension;

    mass<kilo_grams> mass{2.0};
    speed<meters, seconds> velocity{3.0};

    momentum<kilo_grams, meters, seconds> momentum = mass * velocity;

    ASSERT_NEAR((get_momentum_as<kilo_grams, meters, seconds>(momentum)), 6.0, TOLERANCE);
}

TEST(PhysicsProblemsExample1, powerFromWorkAndtimespan2)
{
    using namespace dimension;

    energy<joules> work{100.0};
    timespan<seconds> time{5.0};

    power<Watts> power = work / time;

    ASSERT_NEAR((get_power_as<Watts>(power)), 20.0, TOLERANCE); // 100 J / 5 s = 20 W
}

TEST(PhysicsProblemsExample1, currentCalculation)
{
    using namespace dimension;

    charge<coulombs> charge{10.0};
    timespan<seconds> time{2.0};

    current<amperes> current = charge / time;

    ASSERT_NEAR((get_current_as<amperes>(current)), 5.0, TOLERANCE); // 10 C / 2 s = 5 A
}

TEST(PhysicsProblemsExample1, electric_potentialFromWorkAndcharge)
{
    using namespace dimension;

    energy<joules> work{50.0};
    charge<coulombs> charge{2.0};

    electric_potential<volts> voltage = work / charge;

    ASSERT_NEAR((get_electric_potential_as<volts>(voltage)), 25.0, TOLERANCE); // 50 J / 2 C = 25 V
}

TEST(PhysicsProblemsExample1, resistanceFromVoltageAndcurrent)
{
    using namespace dimension;

    electric_potential<volts> voltage{120.0};
    current<amperes> current{10.0};

    resistance<ohms> resistance = voltage / current;
    //resistance<ohms> resistance(12.0);
    

    ASSERT_NEAR((get_resistance_as<ohms>(resistance)), 12.0, TOLERANCE); // 120 V / 10 A = 12 ohms
}


TEST(PhysicsProblemsExample1, capacitanceCalculation)
{
    using namespace dimension;

    charge<coulombs> charge{5.0};
    electric_potential<volts> voltage{10.0};

    //capacitance<farads> capacitance = charge / voltage;
    auto capacitance = charge / voltage;
    ASSERT_NEAR((get_capacitance_as<farads>(capacitance)), 0.5, TOLERANCE); // 5 C / 10 V = 0.5 F
}


TEST(PhysicsProblemsExample1, inductanceCalculation)
{
    using namespace dimension;

    magnetic_flux<webers> flux{2.0};
    current<amperes> current{4.0};

    inductance<henrys> inductance = flux / current;

    ASSERT_NEAR((get_inductance_as<henrys>(inductance)), 0.5, TOLERANCE); // 2 Wb / 4 A = 0.5 H
}

TEST(PhysicsProblemsExample1, magnetic_fluxCalculation)
{
    using namespace dimension;

    electric_potential<volts> voltage{10.0};
    timespan<seconds> time{2.0};

    magnetic_flux<webers> flux = voltage * time;

    ASSERT_NEAR((get_magnetic_flux_as<webers>(flux)), 20.0, TOLERANCE); // 10 V * 2 s = 20 Wb
}

TEST(PhysicsProblemsExample1, magnetic_fieldCalculation)
{
    using namespace dimension;

    magnetic_flux<webers> flux{5.0};
    area<meters> area{2.0};

    magnetic_field<teslas> field = flux / area;

    ASSERT_NEAR((get_magnetic_field_as<teslas>(field)), 2.5, TOLERANCE); // 5 Wb / 2 m² = 2.5 T
}

TEST(PhysicsProblemsExample1, electric_fieldCalculation)
{
    using namespace dimension;

    electric_potential<volts> voltage{100.0};
    length<meters> distance{2.0};

    electric_field<kilo_grams, meters, seconds, coulombs> field = voltage / distance;

    ASSERT_NEAR((get_electric_field_as<kilo_grams, meters, seconds, coulombs>(field)), 50.0, TOLERANCE); // 100 V / 2 m = 50 N/C
}

TEST(PhysicsProblemsExample1, conductanceCalculation)
{
    using namespace dimension;

    resistance<ohms> resistance{10.0};

    conductance<siemens> conductance = 1.0 / resistance;

    ASSERT_NEAR((get_conductance_as<siemens>(conductance)), 0.1, TOLERANCE); // 1 / 10 ohms = 0.1 S
}

TEST(PhysicsProblemsExample1, dynamic_viscosityCalculation)
{
    using namespace dimension;

    force<newtons> force{10.0};
    area<meters> area{2.0};
    speed<meters, seconds> velocity{4.0};
    length<meters> distance{1.0};

    dynamic_viscosity viscosity = (force / area) * (distance / velocity);

    ASSERT_NEAR((get_dynamic_viscosity_as<kilo_grams, seconds, meters>(viscosity)), 1.25, TOLERANCE);
}

TEST(PhysicsProblemsExample1, densityCalculation2)
{
    using namespace dimension;

    mass<kilo_grams> mass{10.0};
    volume<meters> volume{2.0};

    density<kilo_grams, meters> density = mass / volume;

    ASSERT_NEAR((get_density_as<kilo_grams, meters>(density)), 5.0, TOLERANCE); // 10 kg / 2 m³ = 5 kg/m³
}

TEST(PhysicsProblemsExample1, volumetricFlowRateCalculation)
{
    using namespace dimension;

    volume<meters> volume{1.0};
    timespan<seconds> time{2.0};

    volumetric_flow_rate<meters, seconds> flowRate = volume / time;

    ASSERT_NEAR((get_volumetric_flow_rate_as<meters, seconds>(flowRate)), 0.5, TOLERANCE); // 1 m³ / 2 s = 0.5 m³/s
}

TEST(PhysicsProblemsExample1, massFlowRateCalculation)
{
    using namespace dimension;

    mass<kilo_grams> mass{20.0};
    timespan<seconds> time{4.0};

    mass_flow_rate<kilo_grams, seconds> flowRate = mass / time;

    ASSERT_NEAR((get_mass_flow_rate_as<kilo_grams, seconds>(flowRate)), 5.0, TOLERANCE); // 20 kg / 4 s = 5 kg/s
}

TEST(PhysicsProblemsExample1, SpecificvolumeCalculation)
{
    using namespace dimension;

    volume<meters> volume{1.0};
    mass<kilo_grams> mass{4.0};

    specific_volume<meters, kilo_grams> specificvolume = volume / mass;

    ASSERT_NEAR((get_specific_volume_as<meters, kilo_grams>(specificvolume)), 0.25, TOLERANCE); // 1 m³ / 4 kg = 0.25 m³/kg
}

TEST(PhysicsProblemsExample1, MomentOfInertiaCalculation)
{
    using namespace dimension;

    mass<kilo_grams> mass{10.0};
    length<meters> radius{2.0};

    moment_of_inertia<kilo_grams, meters> inertia = mass * radius * radius;

    ASSERT_NEAR((get_moment_of_inertia_as<kilo_grams, meters>(inertia)), 40.0, TOLERANCE); // 10 kg * 2 m * 2 m = 40 kg·m²
}

TEST(PhysicsProblemsExample1, angular_speedCalculation)
{
    using namespace dimension;

    angle<radians> angle{std::numbers::pi}; // Half a rotation (180 degrees)
    timespan<seconds> time{2.0};

    angular_speed<radians, seconds> angularspeed = angle / time;

    ASSERT_NEAR((get_angular_speed_as<radians, seconds>(angularspeed)), std::numbers::pi / 2.0, TOLERANCE); // π radians / 2 s = π/2 rad/s
}

TEST(PhysicsProblemsExample1, AngularaccelerationCalculation)
{
    using namespace dimension;

    angular_speed<radians, seconds> initialspeed{2.0};
    angular_speed<radians, seconds> finalspeed{6.0};
    timespan<seconds> time{2.0};

    angular_acceleration<radians, seconds> angularacceleration = (finalspeed - initialspeed) / time;

    ASSERT_NEAR((get_angular_acceleration_as<radians, seconds>(angularacceleration)), 2.0, TOLERANCE); // (6 rad/s - 2 rad/s) / 2 s = 2 rad/s²
}


TEST(PhysicsProblemsExample1, AtmosphereCalculation)
{
    using namespace dimension;

    pressure<pascals> obj(1.0);

    ASSERT_NEAR((get_pressure_as<atmospheres>(obj)), 9.86923e-6, 1e-7);
    ASSERT_NEAR((get_pressure_as<bar>(obj)), 1e-5, 1e-7);
    ASSERT_NEAR((get_pressure_as<torr>(obj)), 0.00750062, 1e-7);
    ASSERT_NEAR((get_pressure_as<mm_hg>(obj)), 0.00750062, 1e-7);
    ASSERT_NEAR((get_pressure_as<in_hg>(obj)), 0.0002953, 1e-7);
    ASSERT_NEAR((get_pressure_as<pascals>(obj)), 1.0, 1e-7);
}

TEST(PhysicsProblemsExample1, volumeCaclulation)
{
    using namespace dimension;

    volume<gallons> obj(1.0);

    ASSERT_NEAR((get_volume_as<fluid_ounces>(obj)), 128.0, 1e-3);
    ASSERT_NEAR((get_volume_as<cups>(obj)), 16.0, 1e-3);
    ASSERT_NEAR((get_volume_as<pints>(obj)), 8.0, 1e-3);
    ASSERT_NEAR((get_volume_as<quarts>(obj)), 4.0, 1e-3);
    ASSERT_NEAR((get_volume_as<gallons>(obj)), 1.0, 1e-3);
    ASSERT_NEAR((get_volume_as<teaspoons>(obj)), 768.0, 1e-3);
    ASSERT_NEAR((get_volume_as<tablespoons>(obj)), 256.0, 1e-3);


    //base_dimension<unit_exponent<seconds, -1>, unit_exponent<meters>, unit_exponent<Grams, -1>, unit_exponent<kilo_grams>> a(10.0);
    base_dimension<unit_exponent<seconds, -1>, unit_exponent<meters>, unit_exponent<miles, -1>, unit_exponent<kilo_meters>> a(10.0);
    //base_dimension<unit_exponent<seconds, -1>, unit_exponent<meters>> a(10.0);
    //speed<seconds, meters> b = a;
    //speed<Knots> a(10.0);
    
    speed b = a;

    static_assert(std::is_base_of_v<base_dimension<unit_exponent<meters>, unit_exponent<seconds, -1>>, decltype(b)>);
    static_assert(std::is_same_v<speed<meters, seconds>, decltype(b)>);

    ASSERT_NEAR((get_speed_as<meters, seconds>(b)), 6.21371, 0.001);
}
