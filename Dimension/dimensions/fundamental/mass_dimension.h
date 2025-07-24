#ifndef STATIC_DIMENSION_MASS_H
#define STATIC_DIMENSION_MASS_H

#include "Dimension_Impl/FundamentalDimensions/mass_dimension_Impl.h"

namespace dimension
{
   struct grams : public massUnit<grams, "Grams", "g"> {};
   struct pound_mass : public massUnit<pound_mass, "Pound mass", "lb"> {};
   struct ounces : public massUnit<ounces, "Ounces", "oz"> {};
   struct slugs : public massUnit<slugs, "Slugs", "slug"> {};
   struct grains : public massUnit<grains, "Grains", "gr"> {};
   struct stone : public massUnit<stone, "Stone", "st"> {};
   struct short_ton : public massUnit<short_ton, "Short Ton", "ton"> {};
   struct long_ton : public massUnit<long_ton, "Long Ton", "LT"> {};
   struct tonne : public massUnit<tonne, "Tonne", "t"> {};

   template<> struct Conversion<grams, pound_mass> { static constexpr PrecisionType slope = (100000.0 / 45359237.0); };
   template<> struct Conversion<grams, ounces> { static constexpr PrecisionType slope = (1600000.0 / 45359237.0); };
   template<> struct Conversion<grams, slugs> { static constexpr PrecisionType slope = (609600000.0 / 8896443230521.0); };
   template<> struct Conversion<grams, grains> { static constexpr PrecisionType slope = (100000000.0 / 6479891.0); };
   template<> struct Conversion<grams, stone> { static constexpr PrecisionType slope = (50000.0 / 317514659.0); };
   template<> struct Conversion<grams, short_ton> { static constexpr PrecisionType slope = (50.0 / 45359237.0); };
   template<> struct Conversion<grams, long_ton> { static constexpr PrecisionType slope = (625.0 / 635029318.0); };
   template<> struct Conversion<grams, tonne> { static constexpr PrecisionType slope = 1e-06; };

   ALL_SI_PREFIXES(grams, "g", massUnit);

}




#endif //STATIC_DIMENSION_MASS_H