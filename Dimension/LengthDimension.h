#ifndef STATIC_DIMENSION_LENGTH_H
#define STATIC_DIMENSION_LENGTH_H

#include "Dimension_Impl/FundamentalDimensions/LengthDimension_Impl.h"

namespace Dimension
{

   struct Meters            : public LengthUnit<Meters, "Meters", "m"> {};
   struct Feet              : public LengthUnit<Feet, "Feet", "ft"> {};
   struct Inches            : public LengthUnit<Inches, "Inches", "in"> {};
   struct AstronomicalUnits : public LengthUnit<AstronomicalUnits, "AstronomicalUnits", "au"> {};
   struct DataMiles         : public LengthUnit<DataMiles, "DataMiles", "DataMiles"> {};
   struct NauticalMiles     : public LengthUnit<NauticalMiles, "NauticalMiles", "nmi"> {};
   struct Miles             : public LengthUnit<Miles, "Miles", "mi"> {};
   struct Leagues           : public LengthUnit<Leagues, "Leagues", "lea"> {};
   struct Fathoms           : public LengthUnit<Fathoms, "Fathoms", "ftm"> {};
   struct Furlong           : public LengthUnit<Furlong, "Furlong", "fur"> {};
   struct Yards             : public LengthUnit<Yards, "Yards", "yd"> {};
   struct US_Survey_Feet    : public LengthUnit<US_Survey_Feet, "US_Survey_Feet", "ftUS"> {}; // Obsolete as of 2022


   template<> struct Conversion<Feet,              Meters> { static constexpr PrecisionType slope = 0.3048; }; // NIST
   template<> struct Conversion<Inches,            Meters> { static constexpr PrecisionType slope = 0.0254; }; // NIST
   template<> struct Conversion<AstronomicalUnits, Meters> { static constexpr PrecisionType slope = 1.495979e11; }; // NIST
   template<> struct Conversion<DataMiles,         Meters> { static constexpr PrecisionType slope = 1828.80; };
   template<> struct Conversion<NauticalMiles,     Meters> { static constexpr PrecisionType slope = 1852; }; // NIST
   template<> struct Conversion<Miles,             Meters> { static constexpr PrecisionType slope = 1609.344; }; // NIST
   template<> struct Conversion<Leagues,           Meters> { static constexpr PrecisionType slope = 4828.032; }; // NIST
   template<> struct Conversion<Fathoms,           Meters> { static constexpr PrecisionType slope = 1.8288; }; // NIST
   template<> struct Conversion<Furlong,           Meters> { static constexpr PrecisionType slope = 201.168; }; // NIST
   template<> struct Conversion<Yards,             Meters> { static constexpr PrecisionType slope = 0.9144; }; // NIST
   template<> struct Conversion<US_Survey_Feet,    Meters> { static constexpr PrecisionType slope = 0.304800609601; }; // NIST

   ALL_SI_PREFIXES(Meters, "m", LengthUnit);

}

#endif //STATIC_DIMENSION_LENGTH_H
