#ifndef STATIC_DIMENSION_LENGTH_IMPL_H
#define STATIC_DIMENSION_LENGTH_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   struct LengthType {};
   struct Meters;

   template<typename LengthUnit>
   concept IsLengthUnit = std::is_same_v<typename LengthUnit::Dim, LengthType>;
   
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation>
   struct LengthUnit : public BaseUnit<Unit, Name, Abbreviation>
   { 
   public: 
      using Dim = LengthType;
      using Primary = Meters;
   };

   template<typename T>
   struct is_length : std::false_type {};

   template<typename T>
   struct is_length<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsLengthUnit<T>> {};

   template<typename T>
   constexpr bool is_length_v = is_length<T>::value;

   template<typename T>
   concept length_type = is_length_v<T>;

   template<IsLengthUnit T>
   PrecisionType getLength(length_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   template<IsLengthUnit Unit>
   class Length : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Length() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      Length(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      template<IsLengthUnit T>
      Length(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()){}

      template<IsLengthUnit T>
      [[deprecated("Use the free function getLength() instead.")]]
      double GetLength() const
      {
         return getLength<T>(*this);
      }
   };

   template<IsLengthUnit T>
   Length(T) -> Length<T>;

   template<IsLengthUnit LengthUnit>
   Length(BaseDimension<std::tuple<LengthUnit>, std::tuple<>>) -> Length<LengthUnit>;

   template<typename T>
   struct is_length<Length<T>> : std::bool_constant<IsLengthUnit<T>> {};

}

#endif //STATIC_DIMENSION_LENGTH_IMPL_H
