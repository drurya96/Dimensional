#ifndef STATIC_DIMENSION_LENGTH_H
#define STATIC_DIMENSION_LENGTH_H

#include "Dimension_Impl/FundamentalDimensions/LengthDimension_Impl.h"

namespace Dimension
{

   struct Meters : public LengthUnit<Meters> { public: using LengthUnit::LengthUnit; };
   struct Feet : public LengthUnit<Feet> { public: using LengthUnit::LengthUnit; };
   struct Inches : public LengthUnit<Inches> { public: using LengthUnit::LengthUnit; };

   struct AstronomicalUnits : public LengthUnit<AstronomicalUnits> { public: using LengthUnit::LengthUnit; };
   struct DataMiles : public LengthUnit<DataMiles> { public: using LengthUnit::LengthUnit; };
   struct NauticalMiles : public LengthUnit<NauticalMiles> { public: using LengthUnit::LengthUnit; };
   struct Miles : public LengthUnit<Miles> { public: using LengthUnit::LengthUnit; };
   struct Leagues : public LengthUnit<Leagues> { public: using LengthUnit::LengthUnit; };
   struct Fathoms : public LengthUnit<Fathoms> { public: using LengthUnit::LengthUnit; };
   struct Furlong : public LengthUnit<Furlong> { public: using LengthUnit::LengthUnit; };
   struct Yards : public LengthUnit<Yards> { public: using LengthUnit::LengthUnit; };
   struct US_Survey_Feet : public LengthUnit<US_Survey_Feet> { public: using LengthUnit::LengthUnit; }; // Obsolete as of 2022


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

   ALL_SI_PREFIXES(Meters, LengthUnit);

}

#endif //STATIC_DIMENSION_LENGTH_H
