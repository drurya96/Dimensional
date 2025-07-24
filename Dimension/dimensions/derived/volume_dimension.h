#ifndef STATIC_DIMENSION_VOLUME_H
#define STATIC_DIMENSION_VOLUME_H

#include "Dimension_Impl/DerivedDimensions/volume_dimension_Impl.h"

namespace dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct fluid_ounce_length : public lengthUnit<fluid_ounce_length, "FluidOuncelength", "FluidOuncelength"> {};
   template<> struct Conversion<fluid_ounce_length, meters> { static constexpr PrecisionType slope = 0.03092440948; };

   struct cup_length : public lengthUnit<cup_length, "Cuplength", "Cuplength"> {};
   template<> struct Conversion<cup_length, meters> { static constexpr PrecisionType slope = 0.061848767433875; };

   struct pint_length : public lengthUnit<pint_length, "Pintlength", "Pintlength"> {};
   template<> struct Conversion<pint_length, meters> { static constexpr PrecisionType slope = 0.077924564; };

   struct quart_length : public lengthUnit<quart_length, "Quartlength", "Quartlength"> {};
   template<> struct Conversion<quart_length, meters> { static constexpr PrecisionType slope = 0.09817886355045986; };

   struct gallon_length : public lengthUnit<gallon_length, "Gallonlength", "Gallonlength"> {};
   template<> struct Conversion<gallon_length, meters> { static constexpr PrecisionType slope = 0.155849128; };

   struct teaspoon_length : public lengthUnit<teaspoon_length, "Teaspoonlength", "Teaspoonlength"> {};
   template<> struct Conversion<teaspoon_length, meters> { static constexpr PrecisionType slope = 0.017018344209166245; };

   struct tablespoon_length : public lengthUnit<tablespoon_length, "Tablespoonlength", "Tablespoonlength"> {};
   template<> struct Conversion<tablespoon_length, meters> { static constexpr PrecisionType slope = 0.02454469962137835; };


   struct liters
   {
      using units = std::tuple<
         unit_exponent<deci_meters, 3, 1>
      >;
   };

   struct milli_liters
   {
      using units = std::tuple<
         unit_exponent<centi_meters, 3, 1>
      >;
   };

   struct fluid_ounces
   {
      using units = std::tuple<
         unit_exponent<fluid_ounce_length, 3, 1>
      >;
   };

   struct cups
   {
      using units = std::tuple<
         unit_exponent<cup_length, 3, 1>
      >;
   };

   struct pints
   {
      using units = std::tuple<
         unit_exponent<pint_length, 3, 1>
      >;
   };

   struct quarts
   {
      using units = std::tuple<
         unit_exponent<quart_length, 3, 1>
      >;
   };

   struct gallons
   {
      using units = std::tuple<
         unit_exponent<gallon_length, 3, 1>
      >;
   };

   struct teaspoons
   {
      using units = std::tuple<
         unit_exponent<teaspoon_length, 3, 1>
      >;
   };

   struct tablespoons
   {
      using units = std::tuple<
         unit_exponent<tablespoon_length, 3, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_VOLUME_H