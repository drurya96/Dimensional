#ifndef STATIC_DIMENSION_VOLUME_H
#define STATIC_DIMENSION_VOLUME_H

#include "Dimension_Impl/DerivedDimensions/VolumeDimension_Impl.h"

namespace dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct FluidOuncelength : public lengthUnit<FluidOuncelength, "FluidOuncelength", "FluidOuncelength"> {};
   template<> struct Conversion<FluidOuncelength, meters> { static constexpr PrecisionType slope = 0.03092440948; };

   struct Cuplength : public lengthUnit<Cuplength, "Cuplength", "Cuplength"> {};
   template<> struct Conversion<Cuplength, meters> { static constexpr PrecisionType slope = 0.061848767433875; };

   struct Pintlength : public lengthUnit<Pintlength, "Pintlength", "Pintlength"> {};
   template<> struct Conversion<Pintlength, meters> { static constexpr PrecisionType slope = 0.077924564; };

   struct Quartlength : public lengthUnit<Quartlength, "Quartlength", "Quartlength"> {};
   template<> struct Conversion<Quartlength, meters> { static constexpr PrecisionType slope = 0.09817886355045986; };

   struct Gallonlength : public lengthUnit<Gallonlength, "Gallonlength", "Gallonlength"> {};
   template<> struct Conversion<Gallonlength, meters> { static constexpr PrecisionType slope = 0.155849128; };

   struct Teaspoonlength : public lengthUnit<Teaspoonlength, "Teaspoonlength", "Teaspoonlength"> {};
   template<> struct Conversion<Teaspoonlength, meters> { static constexpr PrecisionType slope = 0.017018344209166245; };

   struct Tablespoonlength : public lengthUnit<Tablespoonlength, "Tablespoonlength", "Tablespoonlength"> {};
   template<> struct Conversion<Tablespoonlength, meters> { static constexpr PrecisionType slope = 0.02454469962137835; };


   struct Liters
   {
      using units = std::tuple<
         unit_exponent<Decimeters, 3, 1>,
      >;
   };
   
   struct MilliLiters
   {
      using units = std::tuple<
         unit_exponent<Centimeters, 3, 1>,
      >;
   };
   
   struct FluidOunces
   {
      using units = std::tuple<
         unit_exponent<FluidOuncelength, 3, 1>,
      >;
   };
   
   struct Cups
   {
      using units = std::tuple<
         unit_exponent<Cuplength, 3, 1>,
      >;
   };
   
   struct Pints
   {
      using units = std::tuple<
         unit_exponent<Pintlength, 3, 1>,
      >;
   };
   
   struct Quarts
   {
      using units = std::tuple<
         unit_exponent<Quartlength, 3, 1>,
      >;
   };
   
   struct Gallons
   {
      using units = std::tuple<
         unit_exponent<Gallonlength, 3, 1>,
      >;
   };
   
   struct Teaspoons
   {
      using units = std::tuple<
         unit_exponent<Teaspoonlength, 3, 1>,
      >;
   };
   
   struct Tablespoons
   {
      using units = std::tuple<
         unit_exponent<Tablespoonlength, 3, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_VOLUME_H