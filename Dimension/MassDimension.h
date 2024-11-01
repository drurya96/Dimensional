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
      double GetMass() const
      {
         return this->template GetVal<std::tuple<T>, std::tuple<>>();
      }
   };

   template<IsMassUnit T>
   Mass(T) -> Mass<T>;

   template<IsMassUnit MassUnit>
   Mass(BaseDimension<std::tuple<MassUnit>, std::tuple<>>) -> Mass<MassUnit>;

   template<> struct Conversion<Grams, PoundMass> { static constexpr PrecisionType slope = 0.0022046226; };
   template<> struct Conversion<PoundMass, Grams> { static constexpr PrecisionType slope = 453.5923745; };

   template<> struct Conversion<Grams, Ounces> { static constexpr PrecisionType slope = 0.0352739619; };
   template<> struct Conversion<Ounces, Grams> { static constexpr PrecisionType slope = 28.349523165; };


   template<> struct Conversion<Grams, Slugs> { static constexpr PrecisionType slope = 0.00006852; };
   template<> struct Conversion<Slugs, Grams> { static constexpr PrecisionType slope = (1.0 / 0.00006852); };

   template<> struct Conversion<Grams, Grains> { static constexpr PrecisionType slope = 15.4324; };
   template<> struct Conversion<Grains, Grams> { static constexpr PrecisionType slope = (1.0 / 15.4324); };

   template<> struct Conversion<Grams, Stone> { static constexpr PrecisionType slope = 0.000157473; };
   template<> struct Conversion<Stone, Grams> { static constexpr PrecisionType slope = (1.0 / 0.000157473); };

   template<> struct Conversion<Grams, ShortTon> { static constexpr PrecisionType slope = 0.00000110231; };
   template<> struct Conversion<ShortTon, Grams> { static constexpr PrecisionType slope = (1.0 / 0.00000110231); };

   template<> struct Conversion<Grams, LongTon> { static constexpr PrecisionType slope = 0.000000984207; };
   template<> struct Conversion<LongTon, Grams> { static constexpr PrecisionType slope = (1.0 / 0.000000984207); };

   template<> struct Conversion<Grams, Tonne> { static constexpr PrecisionType slope = 0.000001; };
   template<> struct Conversion<Tonne, Grams> { static constexpr PrecisionType slope = (1.0 / 0.000001); };

   ALL_SI_PREFIXES(Grams, MassUnit);

   template<typename T>
   struct is_mass : std::is_convertible<T, Mass<Grams>> {};

   template<typename T>
   constexpr bool is_mass_v = is_mass<T>::value;

   template<typename T>
   concept mass_type = is_mass_v<T>;

}

#endif //STATIC_DIMENSION_MASS_H
