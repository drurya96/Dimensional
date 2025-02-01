#ifndef STATIC_DIMENSION_PRESSURE_H
#define STATIC_DIMENSION_PRESSURE_H

#include "Dimension_Impl/DerivedDimensions/PressureDimension_Impl.h" 

namespace Dimension
{

   struct Pascals
   {
      using NumTuple = std::tuple<KiloGrams>;
      using DenTuple = std::tuple<Meters, Seconds, Seconds>;
   };

   struct AtmosphereMass : public MassUnit<AtmosphereMass, "AtmosphereMass", "AtmosphereMass"> {};

   template<> struct Conversion<AtmosphereMass, Grams> { static constexpr PrecisionType slope = 1.0 / 9.8692326671601e-9; };

   struct Atmospheres
   {
      using NumTuple = std::tuple<AtmosphereMass>;
      using DenTuple = std::tuple<Meters, Seconds, Seconds>;
   };

   struct BarMass : public MassUnit<BarMass, "BarMass", "BarMass"> {};

   template<> struct Conversion<BarMass, Grams> { static constexpr PrecisionType slope = 1e8; };

   struct Bar
   {
      using NumTuple = std::tuple<BarMass>;
      using DenTuple = std::tuple<Meters, Seconds, Seconds>;
   };

   struct TorrMass : public MassUnit<TorrMass, "TorrMass", "TorrMass"> {};

   template<> struct Conversion<TorrMass, Grams> { static constexpr PrecisionType slope = 1.0 / 7.50062e-6; };

   struct Torr
   {
      using NumTuple = std::tuple<TorrMass>;
      using DenTuple = std::tuple<Meters, Seconds, Seconds>;
   };

   using mmHg = Torr;

   struct inHgMass : public MassUnit<inHgMass, "inHgMass", "inHgMass"> {};

   template<> struct Conversion<inHgMass, Grams> { static constexpr PrecisionType slope = 1.0 / 2.953e-7; };

   struct inHg
   {
      using NumTuple = std::tuple<inHgMass>;
      using DenTuple = std::tuple<Meters, Seconds, Seconds>;
   };


   
}

#endif //STATIC_DIMENSION_PRESSURE_H
