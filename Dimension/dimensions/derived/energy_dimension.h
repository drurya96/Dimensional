#ifndef STATIC_DIMENSION_ENERGY_H
#define STATIC_DIMENSION_ENERGY_H

#include "Dimension_Impl/DerivedDimensions/energy_dimension_Impl.h"

namespace dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct calorie_mass : public massUnit<calorie_mass, "Caloriemass", "Caloriemass"> {};
   template<> struct Conversion<calorie_mass, grams> { static constexpr PrecisionType slope = 4184.0; };


   struct joules
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   struct foot_pounds
   {
      using units = std::tuple<
         unit_exponent<slugs, 1, 1>,
         unit_exponent<feet, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   struct btus
   {
      using units = std::tuple<
         unit_exponent<slugs, 1, 1>,
         unit_exponent<feet, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   struct calories
   {
      using units = std::tuple<
         unit_exponent<calorie_mass, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_ENERGY_H