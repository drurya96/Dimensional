#include "PhysicsProblemsBase.h"

#include "AmountDimension.h"
#include "AngleDimension.h"
#include "LengthDimension.h"
#include "TemperatureDimension.h"
#include "MassDimension.h"
#include "TimeDimension.h"

#include "EnergyDimension.h"
#include "SpeedDimension.h"
#include "AccelerationDimension.h"
#include "ForceDimension.h"
#include "PressureDimension.h"
#include "VolumeDimension.h"

#include "DimensionalConstants.h"
#include "DimensionalMolarMasses.h"

#include <ostream>

#include <iostream>

template<typename T>
using temperature_point = Dimension::point<T, Dimension::TemperatureType>;


TEST(PhysicsProblemsExample1, HeatTransfer)
{
    using namespace Dimension;

    // Given values
    temperature_point<Kelvin> startingTemp{100};
    temperature_point<Celsius> endingTemp{26.85};

    Temperature deltaTemp = endingTemp - startingTemp;

    Mass<KiloGrams> mass(2.0);

    Energy<Joules> heat = mass * Constants::specific_heat_water * deltaTemp;

    double expected_heat = 1673600;

    ASSERT_NEAR(get_energy_as<Joules>(heat), expected_heat, TOLERANCE);
}


TEST(PhysicsProblemsExample1, KinematicsDisplacement)
{
    using namespace Dimension;

    // Given values
    Speed<Meters, Seconds> initialVelocity{10.0}; // Initial speed = 10 m/s
    Acceleration<Meters, Seconds> acceleration{2.0}; // Acceleration = 2 m/s²
    Time<Seconds> time{5.0}; // Time = 5 seconds

    // Calculate displacement using s = v0 * t + (1/2) * a * t^2
    Length<Meters> displacement1 = initialVelocity * time;
    Length<Meters> displacement2 = 0.5 * acceleration * time * time;

    Length<Meters> totalDisplacement = displacement1 + displacement2;

    // Expected result: s = (10 * 5) + (0.5 * 2 * 5^2) = 50 + 25 = 75 Meters
    ASSERT_NEAR(get_length_as<Meters>(totalDisplacement), 75.0, TOLERANCE);
}

TEST(PhysicsProblemsExample1, WorkDoneByForce)
{
    using namespace Dimension;

    // Given values
    Force<Newtons> force{50.0}; // Force = 50 N (kg·m/s²)
    Length<Meters> displacement{10.0};                      // Displacement = 10 m
    Angle<Radians> angle{std::numbers::pi / 3};                         // Angle = 60 degrees (in radians)

    // Calculate work done: W = F * d * cos(theta)
    Energy<Joules> workDone = force * displacement * Dimension::cos(angle);

    // Expected result: W = 50 * 10 * cos(60°) = 50 * 10 * 0.5 = 250 J
    ASSERT_NEAR((get_energy_as<Joules>(workDone)), 250.0, TOLERANCE);
}

TEST(PhysicsProblemsExample1, ProjectileMotion)
{
    using namespace Dimension;

    // Given values
    Speed<Meters, Seconds> initialVelocity{50.0};  // Initial velocity = 50 m/s
    Angle<Radians> launchAngle{std::numbers::pi / 4};          // Launch angle = 45 degrees (in radians)
    Acceleration<Meters, Seconds> gravity{9.81}; // Gravitational acceleration = 9.81 m/s²

    // Calculate Time of Flight: T = (2 * v0 * sin(angle)) / g
    Time<Seconds> timeOfFlight = (2.0 * initialVelocity * sin(launchAngle)) / gravity;

    // Calculate Horizontal Range: R = v0 * cos(angle) * T
    Length<Meters> horizontalRange = initialVelocity * cos(launchAngle) * timeOfFlight;

    // Expected results
    double expectedTimeOfFlight = (2.0 * 50.0 * std::sin(std::numbers::pi / 4)) / 9.81; // ~ 7.208 s
    double expectedRange = 50.0 * std::cos(std::numbers::pi / 4) * expectedTimeOfFlight; // ~ 255.1 m

    // Assertions
    ASSERT_NEAR(get_time_as<Seconds>(timeOfFlight), expectedTimeOfFlight, TOLERANCE);
    ASSERT_NEAR(get_length_as<Meters>(horizontalRange), expectedRange, TOLERANCE);
}

TEST(PhysicsProblemsExample1, PowerFromWorkAndTime)
{
    using namespace Dimension;

    // Given values
    Energy<Joules> work{500.0}; // Work = 500 J
    Time<Seconds> time{10.0};                                        // Time = 10 s

    /// @todo Update this with a Work dimension when available
    // Calculate Power: P = W / t
    Power power = work / time;

    // Expected result
    double expectedPower = 500.0 / 10.0; // 50.0 W

    // Assertions
    ASSERT_NEAR((get_power_as<KiloGrams, Meters, Seconds>(power)), expectedPower, TOLERANCE);

}

TEST(PhysicsProblemsExample1, DensityCalculation)
{
    using namespace Dimension;

    // Given values
    Mass<KiloGrams> mass{500.0};                      // Mass = 500 kg
    Length<Meters> length{2.0};                       // Length = 2 m
    Length<Meters> width{1.0};                        // Width = 1 m
    Length<Meters> height{0.5};                       // Height = 0.5 m

    // Calculate Volume: V = length * width * height
    Volume<Meters> volume = length * width * height;

    // Calculate Density: ρ = mass / volume
    Density<KiloGrams, Meters> density = mass / volume;

    // Expected results
    double expectedVolume = 2.0 * 1.0 * 0.5; // 1.0 m³
    double expectedDensity = 500.0 / expectedVolume; // 500.0 kg/m³

    // Assertions
    ASSERT_NEAR((get_volume_as<Meters>(volume)), expectedVolume, TOLERANCE);
    ASSERT_NEAR((get_density_as<KiloGrams, Meters>(density)), expectedDensity, TOLERANCE);
}

TEST(PhysicsProblemsExample1, PressureFromForceAndVolume)
{
    using namespace Dimension;

    // Given values
    Force<Newtons> force{500.0};                   // Force = 500 N
    Length<Meters> length{4.0};                    // Length = 4 m
    Length<Meters> width{2.0};                     // Width = 2 m
    Length<Meters> height{0.5};                    // Height = 0.5 m

    // Step 1: Calculate Volume
    Volume<Meters> volume = length * width * height;

    // Step 2: Calculate Area (cross-section): A = V / h
    Area<Meters> area = volume / height;

    // Step 3: Calculate Pressure: P = F / A
    Pressure<KiloGrams, Meters, Seconds> pressure = force / area;

    // Expected results
    double expectedVolume = 4.0 * 2.0 * 0.5;       // 4 m * 2 m * 0.5 m = 4.0 m³
    double expectedArea = expectedVolume / 0.5;   // Cross-section: V / h = 8.0 m²
    double expectedPressure = 500.0 / expectedArea; // P = F / A = 500 / 8.0 = 62.5 N/m²

    // Assertions
    ASSERT_NEAR((get_volume_as<Meters>(volume)), expectedVolume, TOLERANCE);
    ASSERT_NEAR((get_area_as<Meters>(area)), expectedArea, TOLERANCE);
    ASSERT_NEAR((get_pressure_as<KiloGrams, Meters, Seconds>(pressure)), expectedPressure, TOLERANCE);

}

TEST(PhysicsProblemsExample1, WaterBoilingTemperatureChange)
{
    using namespace Dimension;

    // Given values
    temperature_point<Celsius> initialTemp{25.0}; // Initial temperature in Celsius
    temperature_point<Celsius> boilingPoint{100.0}; // Boiling point of water in Celsius

    // Calculate the temperature difference
    Temperature deltaTemp = boilingPoint - initialTemp;

    // Validate in Celsius
    double expectedDeltaTempC = 75.0; // 100 - 25
    ASSERT_NEAR(get_temperature_as<Kelvin>(deltaTemp), expectedDeltaTempC, TOLERANCE);

    // Convert delta to Fahrenheit and validate
    double expectedDeltaTempF = 75.0 * 9.0 / 5.0;  // = 135.0
    ASSERT_NEAR(get_temperature_as<Rankine>(deltaTemp), expectedDeltaTempF, TOLERANCE);
}

TEST(PhysicsProblemsExample1, EnergyRequiredToHeatWater)
{
    using namespace Dimension;

    // Given values
    Volume<Liters> volumeOfWater{2.0};                     // Volume in liters
    //BaseDimension<UnitExponent<KiloGrams>, UnitExponent<Liters, -1>> waterDensity(1.0);
    Density<KiloGrams, DeciMeters> waterDensity(1.0); // TODO: Not an ideal signature for Density... I'd like to use Density<KiloGrams, Liters>
    Mass<KiloGrams> mass = volumeOfWater * waterDensity;            // Density of water = 1 kg/L
    temperature_point<Celsius> initialTemp{20.0};      // Initial temperature in Celsius
    temperature_point<Celsius> finalTemp{80.0};        // Final temperature in Celsius
    
    // Calculate the temperature difference
    Temperature deltaTemp = finalTemp - initialTemp;

    // Calculate the energy required
    Energy energy = mass * Constants::specific_heat_water * deltaTemp;

    // Validate the result in Joules
    double expectedEnergyJ = 2.0 * 1000.0 * 4.184 * 60.0; // 2 kg * 1000 g/kg * 4.184 J/g°C * 60°C
    ASSERT_NEAR(get_energy_as<Joules>(energy), expectedEnergyJ, TOLERANCE);

    // Convert to Calories and validate
    double expectedEnergyCal = expectedEnergyJ / 4.184; // ~286.47 Calories
    ASSERT_NEAR(get_energy_as<Calories>(energy), expectedEnergyCal, TOLERANCE);
    
}

TEST(PhysicsProblemsExample1, PVEqualsnRT) {
    using namespace Dimension;

    // Define the quantities
    Volume<Meters> V{0.02241404394303833}; // Value calculated manually
    Amount<Moles> n{1.0};

    // Compute nRT
    auto nRT = n * Constants::ideal_gas_constant * Constants::standard_temperature;

    // Compute PV
    auto PV = Constants::standard_atmospheric_pressure * V;

    // Compare PV and nRT
    ASSERT_NEAR(get_energy_as<Joules>(PV), get_energy_as<Joules>(nRT), TOLERANCE);
}

TEST(PhysicsProblemsExample1, PVEqualsnRT_TemperatureInCelsius) {
    using namespace Dimension;
    // Define the quantities
    Pressure<Pascals> P{202650};
    Volume<Meters> V{0.02446548490432685}; // Calculated volume for PV = nRT with n=2 mol, T=25°C
    Amount<Moles> n{2.0}; // 2 moles
    temperature_point<Celsius> T{25.0}; // 25°C, should convert to 298.15 K

    // Compute nRT
    auto nRT = n * Constants::ideal_gas_constant * T; // Should be approximately 2 * 8.314 * 298.15 ≈ 4,957.6382 J

    // Compute PV
    auto PV = P * V; // Should be approximately 2 * 101325 * 0.0244640424377005 ≈ 4,957.6382 J

    // Compare PV and nRT
    ASSERT_NEAR(get_energy_as<Joules>(PV), get_energy_as<Joules>(nRT), 1e-1);
}

TEST(PhysicsProblemsExample1, VerifyFaraday) {
    using namespace Dimension;

    auto chargePerMole = Charge<ElementaryCharges>(1.0) * Constants::avogadro_number;

    double val_chargePerMole = get_dimension_as<UnitExponent<Coulombs>, UnitExponent<Moles, -1>>(chargePerMole);
    double val_faraday = get_dimension_as<UnitExponent<Coulombs>, UnitExponent<Moles, -1>>(Constants::faraday);

    ASSERT_NEAR(val_chargePerMole, val_faraday, 1e-1);

}

TEST(PhysicsProblemsExample1, AreaCalculation)
{
    using namespace Dimension;

    Length<Meters> length{5.0};
    Length<Meters> width{3.0};

    Area<Meters> area = length * width;

    ASSERT_NEAR((get_area_as<Meters>(area)), 15.0, TOLERANCE);
}

TEST(PhysicsProblemsExample1, FrequencyFromPeriod)
{
    using namespace Dimension;

    Time<Seconds> period{2.0}; // 2 seconds
    Frequency<Hertz> frequency = 1.0 / period;

    ASSERT_NEAR((get_frequency_as<Hertz>(frequency)), 0.5, TOLERANCE); // 0.5 Hz
}

TEST(PhysicsProblemsExample1, MomentumCalculation)
{
    using namespace Dimension;

    Mass<KiloGrams> mass{2.0};
    Speed<Meters, Seconds> velocity{3.0};

    Momentum<KiloGrams, Meters, Seconds> momentum = mass * velocity;

    ASSERT_NEAR((get_momentum_as<KiloGrams, Meters, Seconds>(momentum)), 6.0, TOLERANCE);
}

TEST(PhysicsProblemsExample1, PowerFromWorkAndTime2)
{
    using namespace Dimension;

    Energy<Joules> work{100.0};
    Time<Seconds> time{5.0};

    Power<Watts> power = work / time;

    ASSERT_NEAR((get_power_as<Watts>(power)), 20.0, TOLERANCE); // 100 J / 5 s = 20 W
}

TEST(PhysicsProblemsExample1, CurrentCalculation)
{
    using namespace Dimension;

    Charge<Coulombs> charge{10.0};
    Time<Seconds> time{2.0};

    Current<Amperes> current = charge / time;

    ASSERT_NEAR((get_current_as<Amperes>(current)), 5.0, TOLERANCE); // 10 C / 2 s = 5 A
}

TEST(PhysicsProblemsExample1, ElectricPotentialFromWorkAndCharge)
{
    using namespace Dimension;

    Energy<Joules> work{50.0};
    Charge<Coulombs> charge{2.0};

    ElectricPotential<Volts> voltage = work / charge;

    ASSERT_NEAR((get_electricpotential_as<Volts>(voltage)), 25.0, TOLERANCE); // 50 J / 2 C = 25 V
}

TEST(PhysicsProblemsExample1, ResistanceFromVoltageAndCurrent)
{
    using namespace Dimension;

    ElectricPotential<Volts> voltage{120.0};
    Current<Amperes> current{10.0};

    Resistance<Ohms> resistance = voltage / current;
    //Resistance<Ohms> resistance(12.0);
    

    ASSERT_NEAR((get_resistance_as<Ohms>(resistance)), 12.0, TOLERANCE); // 120 V / 10 A = 12 Ohms
}


TEST(PhysicsProblemsExample1, CapacitanceCalculation)
{
    using namespace Dimension;

    Charge<Coulombs> charge{5.0};
    ElectricPotential<Volts> voltage{10.0};

    Capacitance<Farads> capacitance = charge / voltage;
    ASSERT_NEAR((get_capacitance_as<Farads>(capacitance)), 0.5, TOLERANCE); // 5 C / 10 V = 0.5 F
}


TEST(PhysicsProblemsExample1, InductanceCalculation)
{
    using namespace Dimension;

    MagneticFlux<Webers> flux{2.0};
    Current<Amperes> current{4.0};

    Inductance<Henrys> inductance = flux / current;

    ASSERT_NEAR((get_inductance_as<Henrys>(inductance)), 0.5, TOLERANCE); // 2 Wb / 4 A = 0.5 H
}

TEST(PhysicsProblemsExample1, MagneticFluxCalculation)
{
    using namespace Dimension;

    ElectricPotential<Volts> voltage{10.0};
    Time<Seconds> time{2.0};

    MagneticFlux<Webers> flux = voltage * time;

    ASSERT_NEAR((get_magneticflux_as<Webers>(flux)), 20.0, TOLERANCE); // 10 V * 2 s = 20 Wb
}

TEST(PhysicsProblemsExample1, MagneticFieldCalculation)
{
    using namespace Dimension;

    MagneticFlux<Webers> flux{5.0};
    Area<Meters> area{2.0};

    MagneticField<Teslas> field = flux / area;

    ASSERT_NEAR((get_magneticfield_as<Teslas>(field)), 2.5, TOLERANCE); // 5 Wb / 2 m² = 2.5 T
}

TEST(PhysicsProblemsExample1, ElectricFieldCalculation)
{
    using namespace Dimension;

    ElectricPotential<Volts> voltage{100.0};
    Length<Meters> distance{2.0};

    ElectricField<KiloGrams, Meters, Seconds, Coulombs> field = voltage / distance;

    ASSERT_NEAR((get_electricfield_as<KiloGrams, Meters, Seconds, Coulombs>(field)), 50.0, TOLERANCE); // 100 V / 2 m = 50 N/C
}

TEST(PhysicsProblemsExample1, ConductanceCalculation)
{
    using namespace Dimension;

    Resistance<Ohms> resistance{10.0};

    Conductance<Siemens> conductance = 1.0 / resistance;

    ASSERT_NEAR((get_conductance_as<Siemens>(conductance)), 0.1, TOLERANCE); // 1 / 10 Ohms = 0.1 S
}

TEST(PhysicsProblemsExample1, DynamicViscosityCalculation)
{
    using namespace Dimension;

    Force<Newtons> force{10.0};
    Area<Meters> area{2.0};
    Speed<Meters, Seconds> velocity{4.0};
    Length<Meters> distance{1.0};

    DynamicViscosity<Poises> viscosity = (force / area) * (distance / velocity);

    ASSERT_NEAR(get_dynamicviscosity_as<Poises>(viscosity), 12.5, TOLERANCE);
}

TEST(PhysicsProblemsExample1, DensityCalculation2)
{
    using namespace Dimension;

    Mass<KiloGrams> mass{10.0};
    Volume<Meters> volume{2.0};

    Density<KiloGrams, Meters> density = mass / volume;

    ASSERT_NEAR((get_density_as<KiloGrams, Meters>(density)), 5.0, TOLERANCE); // 10 kg / 2 m³ = 5 kg/m³
}

TEST(PhysicsProblemsExample1, VolumetricFlowRateCalculation)
{
    using namespace Dimension;

    Volume<Meters> volume{1.0};
    Time<Seconds> time{2.0};

    VolumetricFlowRate<Meters, Seconds> flowRate = volume / time;

    ASSERT_NEAR((get_volumetricflowrate_as<Meters, Seconds>(flowRate)), 0.5, TOLERANCE); // 1 m³ / 2 s = 0.5 m³/s
}

TEST(PhysicsProblemsExample1, MassFlowRateCalculation)
{
    using namespace Dimension;

    Mass<KiloGrams> mass{20.0};
    Time<Seconds> time{4.0};

    MassFlowRate<KiloGrams, Seconds> flowRate = mass / time;

    ASSERT_NEAR((get_massflowrate_as<KiloGrams, Seconds>(flowRate)), 5.0, TOLERANCE); // 20 kg / 4 s = 5 kg/s
}

TEST(PhysicsProblemsExample1, SpecificVolumeCalculation)
{
    using namespace Dimension;

    Volume<Meters> volume{1.0};
    Mass<KiloGrams> mass{4.0};

    SpecificVolume<Meters, KiloGrams> specificVolume = volume / mass;

    ASSERT_NEAR((get_specificvolume_as<Meters, KiloGrams>(specificVolume)), 0.25, TOLERANCE); // 1 m³ / 4 kg = 0.25 m³/kg
}

TEST(PhysicsProblemsExample1, MomentOfInertiaCalculation)
{
    using namespace Dimension;

    Mass<KiloGrams> mass{10.0};
    Length<Meters> radius{2.0};

    MomentOfInertia<KiloGrams, Meters> inertia = mass * radius * radius;

    ASSERT_NEAR((get_momentofinertia_as<KiloGrams, Meters>(inertia)), 40.0, TOLERANCE); // 10 kg * 2 m * 2 m = 40 kg·m²
}

TEST(PhysicsProblemsExample1, AngularSpeedCalculation)
{
    using namespace Dimension;

    Angle<Radians> angle{std::numbers::pi}; // Half a rotation (180 degrees)
    Time<Seconds> time{2.0};

    AngularSpeed<Radians, Seconds> angularSpeed = angle / time;

    ASSERT_NEAR((get_angularspeed_as<Radians, Seconds>(angularSpeed)), std::numbers::pi / 2.0, TOLERANCE); // π radians / 2 s = π/2 rad/s
}

TEST(PhysicsProblemsExample1, AngularAccelerationCalculation)
{
    using namespace Dimension;

    AngularSpeed<Radians, Seconds> initialSpeed{2.0};
    AngularSpeed<Radians, Seconds> finalSpeed{6.0};
    Time<Seconds> time{2.0};

    AngularAcceleration<Radians, Seconds> angularAcceleration = (finalSpeed - initialSpeed) / time;

    ASSERT_NEAR((get_angularacceleration_as<Radians, Seconds>(angularAcceleration)), 2.0, TOLERANCE); // (6 rad/s - 2 rad/s) / 2 s = 2 rad/s²
}


TEST(PhysicsProblemsExample1, AtmosphereCalculation)
{
    using namespace Dimension;

    Pressure<Pascals> obj(1.0);

    ASSERT_NEAR((get_pressure_as<Atmospheres>(obj)), 9.86923e-6, 1e-7);
    ASSERT_NEAR((get_pressure_as<Bar>(obj)), 1e-5, 1e-7);
    ASSERT_NEAR((get_pressure_as<Torr>(obj)), 0.00750062, 1e-7);
    ASSERT_NEAR((get_pressure_as<mmHg>(obj)), 0.00750062, 1e-7);
    ASSERT_NEAR((get_pressure_as<inHg>(obj)), 0.0002953, 1e-7);
    ASSERT_NEAR((get_pressure_as<Pascals>(obj)), 1.0, 1e-7);
}

TEST(PhysicsProblemsExample1, VolumeCaclulation)
{
    using namespace Dimension;

    Volume<Gallons> obj(1.0);

    ASSERT_NEAR((get_volume_as<FluidOunces>(obj)), 128.0, 1e-3);
    ASSERT_NEAR((get_volume_as<Cups>(obj)), 16.0, 1e-3);
    ASSERT_NEAR((get_volume_as<Pints>(obj)), 8.0, 1e-3);
    ASSERT_NEAR((get_volume_as<Quarts>(obj)), 4.0, 1e-3);
    ASSERT_NEAR((get_volume_as<Gallons>(obj)), 1.0, 1e-3);
    ASSERT_NEAR((get_volume_as<Teaspoons>(obj)), 768.0, 1e-3);
    ASSERT_NEAR((get_volume_as<Tablespoons>(obj)), 256.0, 1e-3);


    //BaseDimension<UnitExponent<Seconds, -1>, UnitExponent<Meters>, UnitExponent<Grams, -1>, UnitExponent<KiloGrams>> a(10.0);
    BaseDimension<UnitExponent<Seconds, -1>, UnitExponent<Meters>, UnitExponent<Miles, -1>, UnitExponent<KiloMeters>> a(10.0);
    //BaseDimension<UnitExponent<Seconds, -1>, UnitExponent<Meters>> a(10.0);
    //Speed<Seconds, Meters> b = a;
    //Speed<Knots> a(10.0);
    
    Speed b = a;

    static_assert(std::is_base_of_v<BaseDimension<UnitExponent<Meters>, UnitExponent<Seconds, -1>>, decltype(b)>);
    static_assert(std::is_same_v<Speed<Meters, Seconds>, decltype(b)>);

    ASSERT_NEAR((get_speed_as<Meters, Seconds>(b)), 6.21371, 0.001);
}
