#ifndef STATIC_DIMENSION_LENGTH_H
#define STATIC_DIMENSION_LENGTH_H

#include "BaseDimension.h"

namespace Dimension
{
   struct LengthType {};
   struct Meters;

   template<typename Unit>
   struct LengthUnit : public BaseUnit<Unit>
   { 
   public: 
      using BaseUnit<Unit>::BaseUnit;

      using Dim = LengthType;
      using Primary = Meters;
   };

   struct Feet : public LengthUnit<Feet> { public: using LengthUnit::LengthUnit; };
   struct Inches : public LengthUnit<Inches> { public: using LengthUnit::LengthUnit; };
   struct Meters : public LengthUnit<Meters> { public: using LengthUnit::LengthUnit; };

   struct AstronomicalUnits : public LengthUnit<AstronomicalUnits> { public: using LengthUnit::LengthUnit; };
   struct DataMiles : public LengthUnit<DataMiles> { public: using LengthUnit::LengthUnit; };
   struct NauticalMiles : public LengthUnit<NauticalMiles> { public: using LengthUnit::LengthUnit; };
   struct Miles : public LengthUnit<Miles> { public: using LengthUnit::LengthUnit; };
   struct Leagues : public LengthUnit<Leagues> { public: using LengthUnit::LengthUnit; };
   struct Fathoms : public LengthUnit<Fathoms> { public: using LengthUnit::LengthUnit; };
   struct Furlong : public LengthUnit<Furlong> { public: using LengthUnit::LengthUnit; };
   struct Yards : public LengthUnit<Yards> { public: using LengthUnit::LengthUnit; };
   struct US_Survey_Feet : public LengthUnit<US_Survey_Feet> { public: using LengthUnit::LengthUnit; };

   template<typename Unit>
   class Length : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      static_assert(std::is_same_v<typename Unit::Dim, typename Meters::Dim>, "Unit provided does not derive from LengthUnit");
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Length() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      Length(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      template<typename T>
      Length(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()){}


      template<typename T>
      double GetLength() const
      {
         return this->template GetVal<std::tuple<T>, std::tuple<>>();
      }
   };

   template<typename T>
   Length(T) -> Length<T>;

   template<typename LengthUnit>
   Length(BaseDimension<std::tuple<LengthUnit>, std::tuple<>>) -> Length<LengthUnit>;

   template<> struct Conversion<Meters, Feet> { static constexpr PrecisionType slope = 3.280839895; };
   template<> struct Conversion<Feet, Meters> { static constexpr PrecisionType slope = 0.304800000; };
   template<> struct Conversion<Meters, Inches> { static constexpr PrecisionType slope = 39.37; };
   template<> struct Conversion<Inches, Meters> { static constexpr PrecisionType slope = 0.0254000508; };

   template<> struct Conversion<Meters, AstronomicalUnits> { static constexpr PrecisionType slope = 6.68459e-12; };
   template<> struct Conversion<AstronomicalUnits, Meters> { static constexpr PrecisionType slope = 1.0 / 6.68459e-12; };

   template<> struct Conversion<Meters, DataMiles> { static constexpr PrecisionType slope = 0.000621371; };
   template<> struct Conversion<DataMiles, Meters> { static constexpr PrecisionType slope = 1.0 / 0.000621371; };

   template<> struct Conversion<Meters, NauticalMiles> { static constexpr PrecisionType slope = 0.000539957; };
   template<> struct Conversion<NauticalMiles, Meters> { static constexpr PrecisionType slope = 1.0 / 0.000539957; };

   template<> struct Conversion<Meters, Miles> { static constexpr PrecisionType slope = 0.000621371; };
   template<> struct Conversion<Miles, Meters> { static constexpr PrecisionType slope = 1.0 / 0.000621371; };

   template<> struct Conversion<Meters, Leagues> { static constexpr PrecisionType slope = 0.000207124; };
   template<> struct Conversion<Leagues, Meters> { static constexpr PrecisionType slope = 1.0 / 0.000207124; };

   template<> struct Conversion<Meters, Fathoms> { static constexpr PrecisionType slope = 0.546807; };
   template<> struct Conversion<Fathoms, Meters> { static constexpr PrecisionType slope = 1.0 / 0.546807; };

   template<> struct Conversion<Meters, Furlong> { static constexpr PrecisionType slope = 0.00497097; };
   template<> struct Conversion<Furlong, Meters> { static constexpr PrecisionType slope = 1.0 / 0.00497097; };

   template<> struct Conversion<Meters, Yards> { static constexpr PrecisionType slope = 1.09361; };
   template<> struct Conversion<Yards, Meters> { static constexpr PrecisionType slope = 1.0 / 1.09361; };

   template<> struct Conversion<Meters, US_Survey_Feet> { static constexpr PrecisionType slope = 3.28084; };
   template<> struct Conversion<US_Survey_Feet, Meters> { static constexpr PrecisionType slope = 1.0 / 3.28084; };

   ALL_SI_PREFIXES(Meters, LengthUnit);

   // Type trait for C++17 and older
   template<typename T>
   struct is_length : std::is_convertible<T, Length<Meters>> {};

   template<typename T>
   constexpr bool is_length_v = is_length<T>::value;

   // Concept for C++20 and newer
   template<typename T>
   concept length_type = is_length_v<T>;
}

#endif //STATIC_DIMENSION_LENGTH_H
