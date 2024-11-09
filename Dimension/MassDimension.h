#ifndef STATIC_DIMENSION_MASS_H
#define STATIC_DIMENSION_MASS_H

#include "Dimension_Impl/FundamentalDimensions/MassDimension_Impl.h"

namespace Dimension
{

   struct Grams : public MassUnit<Grams> { public: using MassUnit::MassUnit; };
   struct PoundMass : public MassUnit<PoundMass> { public: using MassUnit::MassUnit; };
   struct Ounces : public MassUnit<Ounces> { public: using MassUnit::MassUnit; };

   struct Slugs : public MassUnit<Slugs> { public: using MassUnit::MassUnit; };
   struct Grains : public MassUnit<Grains> { public: using MassUnit::MassUnit; };
   struct Stone : public MassUnit<Stone> { public: using MassUnit::MassUnit; };
   struct ShortTon : public MassUnit<ShortTon> { public: using MassUnit::MassUnit; };
   struct LongTon : public MassUnit<LongTon> { public: using MassUnit::MassUnit; };
   struct Tonne : public MassUnit<Tonne> { public: using MassUnit::MassUnit; };


   template<> struct Conversion<PoundMass, Grams> { static constexpr PrecisionType slope = 453.5924; }; // NIST
   template<> struct Conversion<Ounces,    Grams> { static constexpr PrecisionType slope = 28.34952; }; // NIST
   template<> struct Conversion<Slugs,     Grams> { static constexpr PrecisionType slope = 14593.90; }; // NIST
   template<> struct Conversion<Grains,    Grams> { static constexpr PrecisionType slope = 0.06479891; }; // NIST
   template<> struct Conversion<Stone,     Grams> { static constexpr PrecisionType slope = 6350.29318; };
   template<> struct Conversion<ShortTon,  Grams> { static constexpr PrecisionType slope = 907184.7; }; // NIST
   template<> struct Conversion<LongTon,   Grams> { static constexpr PrecisionType slope = 1016047.0; }; // NIST
   template<> struct Conversion<Tonne,     Grams> { static constexpr PrecisionType slope = 1000000.0; }; // NIST

   ALL_SI_PREFIXES(Grams, MassUnit);

}

#endif //STATIC_DIMENSION_MASS_H
