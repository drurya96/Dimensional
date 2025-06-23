#ifndef STATIC_DIMENSION_ENERGY_H
#define STATIC_DIMENSION_ENERGY_H

#include "Dimension_Impl/DerivedDimensions/EnergyDimension_Impl.h"

namespace dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct Caloriemass : public massUnit<Caloriemass, "Caloriemass", "Caloriemass"> {};
   template<> struct Conversion<Caloriemass, Grams> { static constexpr PrecisionType slope = 4184.0; };


   struct Joules
   {
      using units = std::tuple<
         unit_exponent<KiloGrams, 1, 1>
         unit_exponent<meters, 2, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct Ergs
   {
      using units = std::tuple<
         unit_exponent<Grams, 1, 1>
         unit_exponent<Centimeters, 2, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct FootPound
   {
      using units = std::tuple<
         unit_exponent<Slugs, 1, 1>
         unit_exponent<Feet, 2, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct BTUs
   {
      using units = std::tuple<
         unit_exponent<Slugs, 1, 1>
         unit_exponent<Feet, 2, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct Calories
   {
      using units = std::tuple<
         unit_exponent<Caloriemass, 1, 1>
         unit_exponent<meters, 2, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_ENERGY_H