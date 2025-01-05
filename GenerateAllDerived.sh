#!/bin/bash

python GenerateDerivedDimension.py Acceleration Length Time,Time
python GenerateDerivedDimension.py Energy Mass,Length,Length Time,Time
python GenerateDerivedDimension.py Force Mass,Length Time,Time
python GenerateDerivedDimension.py MolarMass Mass Amount
python GenerateDerivedDimension.py Pressure Mass Length,Time,Time
python GenerateDerivedDimension.py Speed Length Time
python GenerateDerivedDimension.py Volume Length,Length,Length []