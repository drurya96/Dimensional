#ifndef STATIC_DIMENSION_MASS_H
#define STATIC_DIMENSION_MASS_H

#include "BaseDimension.h"

namespace Dimension
{

   struct MassType {};
   struct Grams;

   template<typename MassUnit>
   concept IsMassUnit = std::is_same_v<typename MassUnit::Dim, MassType>;

   template<typename Unit>
   struct MassUnit : public BaseUnit<Unit>
   { 
   public: 
      using BaseUnit<Unit>::BaseUnit;

      using Dim = MassType;
      using Primary = Grams;
   };

   struct Grams : public MassUnit<Grams> { public: using MassUnit::MassUnit; };
   struct PoundMass : public MassUnit<PoundMass> { public: using MassUnit::MassUnit; };
   struct Ounces : public MassUnit<Ounces> { public: using MassUnit::MassUnit; };

   struct Slugs : public MassUnit<Slugs> { public: using MassUnit::MassUnit; };
   struct Grains : public MassUnit<Grains> { public: using MassUnit::MassUnit; };
   struct Stone : public MassUnit<Stone> { public: using MassUnit::MassUnit; };
   struct ShortTon : public MassUnit<ShortTon> { public: using MassUnit::MassUnit; };
   struct LongTon : public MassUnit<LongTon> { public: using MassUnit::MassUnit; };
   struct Tonne : public MassUnit<Tonne> { public: using MassUnit::MassUnit; };

   template<typename T>
   struct is_mass : std::is_convertible<T, BaseDimension<std::tuple<Grams>, std::tuple<>>> {};

   template<typename T>
   constexpr bool is_mass_v = is_mass<T>::value;

   template<typename T>
   concept mass_type = is_mass_v<T>;

   template<IsMassUnit T>
   PrecisionType getMass(mass_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   template<IsMassUnit Unit>
   class Mass : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Mass() : BaseDimension<std::tuple<Unit>, std::tuple<>>(0.0) {}

      Mass(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>(val) {}

      template<IsMassUnit T>
      Mass(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>(base.template GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<IsMassUnit T>
      [[deprecated("Use the free function getMass() instead.")]]
      double GetMass() const
      {
         return getMass<T>(*this);
      }
   };

   template<IsMassUnit T>
   Mass(T) -> Mass<T>;

   template<IsMassUnit MassUnit>
   Mass(BaseDimension<std::tuple<MassUnit>, std::tuple<>>) -> Mass<MassUnit>;

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
