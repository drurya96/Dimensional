#!/bin/bash

# Existing
python GenerateDerivedDimension.py Acceleration Length Time,Time
python GenerateDerivedDimension.py Energy Mass,Length,Length Time,Time # Same as Work and Torque, see documentation
python GenerateDerivedDimension.py Force Mass,Length Time,Time
python GenerateDerivedDimension.py MolarMass Mass Amount
python GenerateDerivedDimension.py Pressure Mass Length,Time,Time
python GenerateDerivedDimension.py Speed Length Time
python GenerateDerivedDimension.py Volume Length,Length,Length []
python GenerateDerivedDimension.py Area Length,Length []
python GenerateDerivedDimension.py Frequency [] Time
python GenerateDerivedDimension.py Momentum Mass,Length Time # Same as Impulse, see documentation
python GenerateDerivedDimension.py Power Mass,Length,Length Time,Time,Time
python GenerateDerivedDimension.py Current Charge Time
python GenerateDerivedDimension.py ElectricPotential Mass,Length,Length Time,Time,Charge
python GenerateDerivedDimension.py Resistance Mass,Length,Length Time,Charge,Charge
python GenerateDerivedDimension.py Capacitance Charge,Charge,Time,Time Mass,Length,Length
python GenerateDerivedDimension.py Inductance Mass,Length,Length Charge,Charge
python GenerateDerivedDimension.py MagneticFlux Mass,Length,Length Time,Charge
python GenerateDerivedDimension.py MagneticField Mass Time,Charge
python GenerateDerivedDimension.py ElectricField Mass,Length Time,Time,Charge
python GenerateDerivedDimension.py Conductance Time,Charge,Charge Mass,Length,Length
python GenerateDerivedDimension.py Entropy Mass,Length,Length Time,Time,Temperature
python GenerateDerivedDimension.py HeatFlux Mass Time,Time,Time
python GenerateDerivedDimension.py SpecificHeatCapacity Length,Length Time,Time,Temperature
python GenerateDerivedDimension.py DiffusionCoefficient Length,Length Time # Same units as KinematicViscosity, see documentation
python GenerateDerivedDimension.py DynamicViscosity Mass Time,Length
python GenerateDerivedDimension.py Density Mass Length,Length,Length
python GenerateDerivedDimension.py VolumetricFlowRate Length,Length,Length Time
python GenerateDerivedDimension.py MassFlowRate Mass Time
python GenerateDerivedDimension.py SpecificVolume Length,Length,Length Mass
python GenerateDerivedDimension.py MomentOfInertia Mass,Length,Length []
python GenerateDerivedDimension.py AngularSpeed Angle Time
python GenerateDerivedDimension.py AngularAcceleration Angle Time,Time
python GenerateDerivedDimension.py Torque Mass,Length,Length Time,Time,Angle
