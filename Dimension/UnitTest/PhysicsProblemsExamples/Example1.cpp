#include "Example1.h"

#include "AngleDimension.h"
#include "LengthDimension.h"
#include "TemperatureDimension.h"
#include "MassDimension.h"
#include "TimeDimension.h"

#include "EnergyDimension.h"
#include "SpeedDimension.h"
#include "AccelerationDimension.h"
#include "ForceDimension.h"
#include "VolumeDimension.h"

#include <ostream>

#include <iostream>

TEST_F(PhysicsProblemsExample1, SimpleHeatTransfer)
{
    using namespace Dimension;

    Quantity<Temperature<Celsius>> temp1{10.0};
    Quantity<Temperature<Fahrenheit>> temp2{80.0};
    
    auto deltaTemp = temp2 - temp1; // equivalent to 16.667 K
    ASSERT_NEAR(getTemperature<Kelvin>(deltaTemp), 16.667, TOLERANCE);

    BaseDimension<std::tuple<Joules>, std::tuple<KiloGrams, Kelvin>> specificHeat_water{4.184};

    Mass<Grams> mass{100};

    Energy<Joules> heat = mass * specificHeat_water * deltaTemp;

    ASSERT_NEAR((getEnergy<Joules>(heat)), 6.9734728, TOLERANCE);

}

TEST_F(PhysicsProblemsExample1, KinematicsDisplacement)
{
    using namespace Dimension;

    // Given values
    Speed<Meters, Seconds> initialVelocity{10.0}; // Initial speed = 10 m/s
    Acceleration<Meters, Seconds, Seconds> acceleration{2.0}; // Acceleration = 2 m/s²
    Time<Seconds> time{5.0}; // Time = 5 seconds

    // Calculate displacement using s = v0 * t + (1/2) * a * t^2
    Length<Meters> displacement1 = initialVelocity * time;
    Length<Meters> displacement2 = 0.5 * acceleration * time * time;

    Length<Meters> totalDisplacement = displacement1 + displacement2;

    // Expected result: s = (10 * 5) + (0.5 * 2 * 5^2) = 50 + 25 = 75 meters
    ASSERT_NEAR(getLength<Meters>(totalDisplacement), 75.0, TOLERANCE);
}

TEST_F(PhysicsProblemsExample1, WorkDoneByForce)
{
    using namespace Dimension;

    // Given values
    Force<Newtons> force{50.0}; // Force = 50 N (kg·m/s²)
    Length<Meters> displacement{10.0};                      // Displacement = 10 m
    Angle<Radians> angle{Dimension::pi / 3};                         // Angle = 60 degrees (in radians)

    // Calculate work done: W = F * d * cos(theta)
    Energy<Joules> workDone = force * displacement * Dimension::cos(angle);

    // Expected result: W = 50 * 10 * cos(60°) = 50 * 10 * 0.5 = 250 J
    ASSERT_NEAR((getEnergy<Joules>(workDone)), 250.0, TOLERANCE);
}

TEST_F(PhysicsProblemsExample1, ProjectileMotion)
{
    using namespace Dimension;

    // Given values
    Speed<Meters, Seconds> initialVelocity{50.0};  // Initial velocity = 50 m/s
    Angle<Radians> launchAngle{Dimension::pi / 4};          // Launch angle = 45 degrees (in radians)
    Acceleration<Meters, Seconds, Seconds> gravity{9.81}; // Gravitational acceleration = 9.81 m/s²

    // Calculate Time of Flight: T = (2 * v0 * sin(angle)) / g
    Time<Seconds> timeOfFlight = (2.0 * initialVelocity * sin(launchAngle)) / gravity;

    // Calculate Horizontal Range: R = v0 * cos(angle) * T
    Length<Meters> horizontalRange = initialVelocity * cos(launchAngle) * timeOfFlight;

    // Expected results
    double expectedTimeOfFlight = (2.0 * 50.0 * std::sin(Dimension::pi / 4)) / 9.81; // ~ 7.208 s
    double expectedRange = 50.0 * std::cos(Dimension::pi / 4) * expectedTimeOfFlight; // ~ 255.1 m

    // Assertions
    ASSERT_NEAR(getTime<Seconds>(timeOfFlight), expectedTimeOfFlight, TOLERANCE);
    ASSERT_NEAR(getLength<Meters>(horizontalRange), expectedRange, TOLERANCE);
}

TEST_F(PhysicsProblemsExample1, PowerFromWorkAndTime)
{
    using namespace Dimension;

    // Given values
    Energy<Joules> work{500.0}; // Work = 500 J
    Time<Seconds> time{10.0};                                        // Time = 10 s

    /// @todo Update this with a Work dimension when available
    // Calculate Power: P = W / t
    BaseDimension<std::tuple<Joules>, std::tuple<Seconds>> power = work / time;

    // Expected result
    double expectedPower = 500.0 / 10.0; // 50.0 W

    // Assertions
    ASSERT_NEAR((power.GetVal<std::tuple<KiloGrams, Meters, Meters>, std::tuple<Seconds, Seconds, Seconds>>()), expectedPower, TOLERANCE);

}

TEST_F(PhysicsProblemsExample1, DensityCalculation)
{
    using namespace Dimension;

    // Given values
    Mass<KiloGrams> mass{500.0};                      // Mass = 500 kg
    Length<Meters> length{2.0};                       // Length = 2 m
    Length<Meters> width{1.0};                        // Width = 1 m
    Length<Meters> height{0.5};                       // Height = 0.5 m

    // Calculate Volume: V = length * width * height
    Volume<Meters, Meters, Meters> volume = length * width * height;

    // Calculate Density: ρ = mass / volume
    BaseDimension<std::tuple<KiloGrams>, std::tuple<Meters, Meters, Meters>> density = mass / volume;

    // Expected results
    double expectedVolume = 2.0 * 1.0 * 0.5; // 1.0 m³
    double expectedDensity = 500.0 / expectedVolume; // 500.0 kg/m³

    // Assertions
    ASSERT_NEAR((getVolume<Meters, Meters, Meters>(volume)), expectedVolume, TOLERANCE);
    ASSERT_NEAR((density.GetVal<std::tuple<KiloGrams>, std::tuple<Meters, Meters, Meters>>()), expectedDensity, TOLERANCE);
}

TEST_F(PhysicsProblemsExample1, PressureFromForceAndVolume)
{
    using namespace Dimension;

    // Given values
    Force<Newtons> force{500.0};                   // Force = 500 N
    Length<Meters> length{4.0};                    // Length = 4 m
    Length<Meters> width{2.0};                     // Width = 2 m
    Length<Meters> height{0.5};                    // Height = 0.5 m

    // Step 1: Calculate Volume
    Volume<Meters, Meters, Meters> volume = length * width * height;

    // Step 2: Calculate Area (cross-section): A = V / h
    BaseDimension<std::tuple<Meters, Meters>, std::tuple<>> area = volume / height;

    // Step 3: Calculate Pressure: P = F / A
    //BaseDimension<std::tuple<Newtons>, std::tuple<Meters, Meters>> pressure = force / area;
    BaseDimension<std::tuple<KiloGrams>, std::tuple<Meters, Seconds, Seconds>> pressure = force / area;
    //auto pressure = force / area;
    //BaseDimension<std::tuple<KiloGrams>, std::tuple<Meters, Seconds, Seconds>> pressure1{4000.0};

    // Expected results
    double expectedVolume = 4.0 * 2.0 * 0.5;       // 4 m * 2 m * 0.5 m = 4.0 m³
    double expectedArea = expectedVolume / 0.5;   // Cross-section: V / h = 8.0 m²
    double expectedPressure = 500.0 / expectedArea; // P = F / A = 500 / 8.0 = 62.5 N/m²

    // Assertions
    ASSERT_NEAR((getVolume<Meters, Meters, Meters>(volume)), expectedVolume, TOLERANCE);
    ASSERT_NEAR((area.GetVal<std::tuple<Meters, Meters>, std::tuple<>>()), expectedArea, TOLERANCE);
    //ASSERT_NEAR((pressure.GetVal<std::tuple<Newtons>, std::tuple<Meters, Meters>>()), expectedPressure, TOLERANCE);
    //ASSERT_NEAR((pressure.GetVal<std::tuple<KiloGrams>, std::tuple<Meters, Seconds, Seconds>>()), expectedPressure, TOLERANCE);

    std::cout << pressure << expectedPressure << std::endl;

    //BaseDimension<std::tuple<Meters, Seconds>, std::tuple<>> testObj(10.0);
    //std::cout << testObj.GetVal<std::tuple<Seconds, Meters>, std::tuple<>>() << std::endl;

    //std::cout << testObj << std::endl;
}
