#!/bin/bash

# Existing
python GenerateDerivedDimension.py acceleration length Time,Time
python GenerateDerivedDimension.py Energy mass,length,length Time,Time # Same as Work and Torque, see documentation
python GenerateDerivedDimension.py force mass,length Time,Time
python GenerateDerivedDimension.py Molarmass mass Amount
python GenerateDerivedDimension.py Pressure mass length,Time,Time
python GenerateDerivedDimension.py speed length Time
python GenerateDerivedDimension.py Volume length,length,length []
python GenerateDerivedDimension.py Area length,length []
python GenerateDerivedDimension.py Frequency [] Time
python GenerateDerivedDimension.py Momentum mass,length Time # Same as Impulse, see documentation
python GenerateDerivedDimension.py Power mass,length,length Time,Time,Time
python GenerateDerivedDimension.py Current charge Time
python GenerateDerivedDimension.py ElectricPotential mass,length,length Time,Time,charge
python GenerateDerivedDimension.py Resistance mass,length,length Time,charge,charge
python GenerateDerivedDimension.py Capacitance charge,charge,Time,Time mass,length,length
python GenerateDerivedDimension.py Inductance mass,length,length charge,charge
python GenerateDerivedDimension.py MagneticFlux mass,length,length Time,charge
python GenerateDerivedDimension.py MagneticField mass Time,charge
python GenerateDerivedDimension.py ElectricField mass,length Time,Time,charge
python GenerateDerivedDimension.py Conductance Time,charge,charge mass,length,length
python GenerateDerivedDimension.py Entropy mass,length,length Time,Time,temperature
python GenerateDerivedDimension.py HeatFlux mass Time,Time,Time
python GenerateDerivedDimension.py SpecificHeatCapacity length,length Time,Time,temperature
python GenerateDerivedDimension.py DiffusionCoefficient length,length Time # Same units as KinematicViscosity, see documentation
python GenerateDerivedDimension.py DynamicViscosity mass Time,length
python GenerateDerivedDimension.py Density mass length,length,length
python GenerateDerivedDimension.py VolumetricFlowRate length,length,length Time
python GenerateDerivedDimension.py massFlowRate mass Time
python GenerateDerivedDimension.py SpecificVolume length,length,length mass
python GenerateDerivedDimension.py MomentOfInertia mass,length,length []
python GenerateDerivedDimension.py Angularspeed angle Time
python GenerateDerivedDimension.py Angularacceleration angle Time,Time
python GenerateDerivedDimension.py Torque mass,length,length Time,Time,angle
