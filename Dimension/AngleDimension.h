#ifndef STATIC_DIMENSION_ANGLE_H
#define STATIC_DIMENSION_ANGLE_H

#include "BaseDimension.h"
#include <numbers>

namespace Dimension
{
   constexpr double pi = std::numbers::pi;

   struct AngleType {};
   struct Radians;

   template<typename AngleUnit>
   concept IsAngleUnit = std::is_same_v<typename AngleUnit::Dim, AngleType>;

   template<typename Unit>
   struct AngleUnit : public BaseUnit<Unit>
   { 
   public: 
      using BaseUnit<Unit>::BaseUnit;

      using Dim = AngleType;
      using Primary = Radians;
   };

   struct Radians : public AngleUnit<Radians> { public: using AngleUnit::AngleUnit; };
   struct Degrees : public AngleUnit<Degrees> { public: using AngleUnit::AngleUnit; };

   template<IsAngleUnit Unit>
   class Angle : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Angle() : BaseDimension<std::tuple<Unit>, std::tuple<>>(0.0) {}

      Angle(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>(val) {}

      template<IsAngleUnit T>
      Angle(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>(base.template GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<IsAngleUnit T>
      double GetAngle() const
      {
         return this->template GetVal<std::tuple<T>, std::tuple<>>();
      }
   };

   template<IsAngleUnit T>
   Angle(T) -> Angle<T>;

   template<IsAngleUnit AngleUnit>
   Angle(BaseDimension<std::tuple<AngleUnit>, std::tuple<>>) -> Angle<AngleUnit>;

   template<> struct Conversion<Radians, Degrees> { static constexpr PrecisionType slope = 180 / pi; };

   template<typename AngleUnit>
   PrecisionType cos(Angle<AngleUnit> angle) { return std::cos(angle.template GetAngle<Radians>()); }

   template<typename AngleUnit>
   PrecisionType sin(Angle<AngleUnit> angle) { return std::sin(angle.template GetAngle<Radians>()); }

   template<typename AngleUnit>
   PrecisionType tan(Angle<AngleUnit> angle) { return std::tan(angle.template GetAngle<Radians>()); }

   inline Angle<Radians> acos(double ratio) { return Angle<Radians>(std::acos(ratio)); }

   inline Angle<Radians> asin(double ratio) { return Angle<Radians>(std::asin(ratio)); }

   inline Angle<Radians> atan(double ratio) { return Angle<Radians>(std::atan(ratio)); }

   template<typename NumTuple, typename DenTuple>
   Angle<Radians> atan2(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2)
   {
      return Angle<Radians>(std::atan2(obj1.template GetVal<NumTuple, DenTuple>() , obj2.template GetVal<NumTuple, DenTuple>()));
   }

   template<typename T>
   struct is_angle : std::is_convertible<T, Angle<Radians>> {};

   template<typename T>
   constexpr bool is_angle_v = is_angle<T>::value;

   template<typename T>
   concept angle_type = is_angle_v<T>;

}

#endif //STATIC_DIMENSION_ANGLE_H
