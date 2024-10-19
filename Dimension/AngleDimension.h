#ifndef STATIC_DIMENSION_ANGLE_H
#define STATIC_DIMENSION_ANGLE_H

#include "BaseDimension.h"
#include <numbers>

namespace Dimension
{

   struct AngleType {};
   struct Radian;

   template<typename Unit>
   struct AngleUnit : public BaseUnit
   { 
   public: 
      using BaseUnit::BaseUnit;

      using Dim = AngleType;
      using Primary = Radian;
   };

   struct Radian : public AngleUnit<Radian> { public: using AngleUnit::AngleUnit; };
   struct Degree : public AngleUnit<Degree> { public: using AngleUnit::AngleUnit; };

   template<typename Unit>
   class Angle : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      static_assert(std::is_same_v<typename Unit::Dim, typename Radian::Dim>, "Unit provided does not derive from AngleUnit");
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Angle() : BaseDimension<std::tuple<Unit>, std::tuple<>>(0.0) {}

      Angle(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>(val) {}

      template<typename T>
      Angle(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>(base.template GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<typename T>
      double GetAngle() const
      {
         return this->template GetVal<std::tuple<T>, std::tuple<>>();
      }
   };

   template<typename T>
   Angle(T) -> Angle<T>;

   template<typename AngleUnit>
   Angle(BaseDimension<std::tuple<AngleUnit>, std::tuple<>>) -> Angle<AngleUnit>;

   template<> struct Conversion<Radian, Degree> { static constexpr PrecisionType slope = 180 / std::numbers::pi; };
   template<> struct Conversion<Degree, Radian> { static constexpr PrecisionType slope = std::numbers::pi / 180; };

   template<typename AngleUnit>
   PrecisionType cos(Angle<AngleUnit> angle) { return std::cos(angle.template GetAngle<Radian>()); }

   template<typename AngleUnit>
   PrecisionType sin(Angle<AngleUnit> angle) { return std::sin(angle.template GetAngle<Radian>()); }

   template<typename AngleUnit>
   PrecisionType tan(Angle<AngleUnit> angle) { return std::tan(angle.template GetAngle<Radian>()); }

   inline Angle<Radian> acos(double ratio) { return Angle<Radian>(std::acos(ratio)); }

   inline Angle<Radian> asin(double ratio) { return Angle<Radian>(std::asin(ratio)); }

   inline Angle<Radian> atan(double ratio) { return Angle<Radian>(std::atan(ratio)); }

   template<typename NumTuple, typename DenTuple>
   Angle<Radian> atan2(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2)
   {
      return Angle<Radian>(std::atan2(obj1.template GetVal<NumTuple, DenTuple>() , obj2.template GetVal<NumTuple, DenTuple>()));
   }
}

#endif //STATIC_DIMENSION_ANGLE_H
