#ifndef STATIC_DIMENSION_MASS_H
#define STATIC_DIMENSION_MASS_H

#include "Dimension_Impl/FundamentalDimensions/MassDimension_Impl.h"

namespace Dimension
{

   struct Grams     : public MassUnit<Grams, "Grams", "g"> { public: using MassUnit::MassUnit; };
   struct PoundMass : public MassUnit<PoundMass, "PoundMass", "lb"> { public: using MassUnit::MassUnit; };
   struct Ounces    : public MassUnit<Ounces, "Ounces", "oz"> { public: using MassUnit::MassUnit; };
   struct Slugs     : public MassUnit<Slugs, "Slugs", "slug"> { public: using MassUnit::MassUnit; };
   struct Grains    : public MassUnit<Grains, "Grains", "gr"> { public: using MassUnit::MassUnit; };
   struct Stone     : public MassUnit<Stone, "Stone", "st"> { public: using MassUnit::MassUnit; };
   struct ShortTon  : public MassUnit<ShortTon, "ShortTon", "ton"> { public: using MassUnit::MassUnit; };
   struct LongTon   : public MassUnit<LongTon, "LongTon", "LT"> { public: using MassUnit::MassUnit; };
   struct Tonne     : public MassUnit<Tonne, "Tonne", "t"> { public: using MassUnit::MassUnit; };


   template<> struct Conversion<PoundMass, Grams> { static constexpr PrecisionType slope = 453.5924; }; // NIST
   template<> struct Conversion<Ounces,    Grams> { static constexpr PrecisionType slope = 28.34952; }; // NIST
   template<> struct Conversion<Slugs,     Grams> { static constexpr PrecisionType slope = 14593.90; }; // NIST
   template<> struct Conversion<Grains,    Grams> { static constexpr PrecisionType slope = 0.06479891; }; // NIST
   template<> struct Conversion<Stone,     Grams> { static constexpr PrecisionType slope = 6350.29318; };
   template<> struct Conversion<ShortTon,  Grams> { static constexpr PrecisionType slope = 907184.7; }; // NIST
   template<> struct Conversion<LongTon,   Grams> { static constexpr PrecisionType slope = 1016047.0; }; // NIST
   template<> struct Conversion<Tonne,     Grams> { static constexpr PrecisionType slope = 1000000.0; }; // NIST

   ALL_SI_PREFIXES(Grams, "g", MassUnit);

}

#endif //STATIC_DIMENSION_MASS_H
