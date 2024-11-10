#ifndef STATIC_DIMENSION_ANGLE_H
#define STATIC_DIMENSION_ANGLE_H

#include "Dimension_Impl/FundamentalDimensions/AngleDimension_Impl.h"

namespace Dimension
{
   struct Radians : public AngleUnit<Radians, "Radians", "rad"> { public: using AngleUnit::AngleUnit; };
   struct Degrees : public AngleUnit<Degrees, "Degrees", "deg"> { public: using AngleUnit::AngleUnit; };

   template<> struct Conversion<Radians, Degrees> { static constexpr PrecisionType slope = 180 / pi; };

   template<typename AngleUnit>
   PrecisionType cos(Angle<AngleUnit> angle) { return std::cos(getAngle<Radians>(angle)); }

   template<typename AngleUnit>
   PrecisionType sin(Angle<AngleUnit> angle) { return std::sin(getAngle<Radians>(angle)); }

   template<typename AngleUnit>
   PrecisionType tan(Angle<AngleUnit> angle) { return std::tan(getAngle<Radians>(angle)); }

   inline Angle<Radians> acos(double ratio) { return Angle<Radians>(std::acos(ratio)); }

   inline Angle<Radians> asin(double ratio) { return Angle<Radians>(std::asin(ratio)); }

   inline Angle<Radians> atan(double ratio) { return Angle<Radians>(std::atan(ratio)); }

   template<typename NumTuple, typename DenTuple>
   Angle<Radians> atan2(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2)
   {
      return Angle<Radians>(std::atan2(obj1.template GetVal<NumTuple, DenTuple>() , obj2.template GetVal<NumTuple, DenTuple>()));
   }

}

#endif //STATIC_DIMENSION_ANGLE_H
