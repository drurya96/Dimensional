#ifndef STATIC_DIMENSION_ANGLE_IMPL_H
#define STATIC_DIMENSION_ANGLE_IMPL_H

#include "../../BaseDimension.h"
#include <numbers>

namespace Dimension
{
   constexpr double pi = std::numbers::pi;

   struct AngleType {};
   struct Radians;

   template<typename T>
   concept IsAngleUnit = IsNonQuantityUnitDimension<T, AngleType> || IsQuantityUnitDimension<T, AngleType>;

   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation>
   struct AngleUnit : public BaseUnit<Unit, Name, Abbreviation, "Angle">
   { 
   public: 
      using Dim = AngleType;
      using Primary = Radians;
   };

   template<typename T>
   struct is_angle : std::false_type {};

   template<typename T>
   struct is_angle<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsAngleUnit<T>> {};

   template<typename T>
   constexpr bool is_angle_v = is_angle<T>::value;

   template<typename T>
   concept angle_type = is_angle_v<T>;

   template<IsAngleUnit T>
   constexpr PrecisionType getAngle(angle_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   template<IsAngleUnit Unit>
   class Angle : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      constexpr Angle() : BaseDimension<std::tuple<Unit>, std::tuple<>>(0.0) {}

      constexpr Angle(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>(val) {}

      template<IsAngleUnit T>
      constexpr Angle(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>(base.template GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<IsAngleUnit T>
      [[deprecated("Use the free function getAngle() instead.")]]
      double GetAngle() const
      {
         return getAngle<T>(*this);
      }
   };

   template<IsAngleUnit T>
   Angle(T) -> Angle<T>;

   template<IsAngleUnit AngleUnit>
   Angle(BaseDimension<std::tuple<AngleUnit>, std::tuple<>>) -> Angle<AngleUnit>;

   template<typename T>
   struct is_angle<Angle<T>> : std::bool_constant<IsAngleUnit<T>> {};

}

#endif //STATIC_DIMENSION_ANGLE_IMPL_H
