#ifndef DIMENSION_ANGLE_EXTRAS_H
#define DIMENSION_ANGLE_EXTRAS_H

namespace Dimension
{
   template<typename AngleUnit>
   PrecisionType cos(Angle<AngleUnit> angle) { return std::cos(get_angle_as<Radians>(angle)); }

   template<typename AngleUnit>
   PrecisionType sin(Angle<AngleUnit> angle) { return std::sin(get_angle_as<Radians>(angle)); }

   template<typename AngleUnit>
   PrecisionType tan(Angle<AngleUnit> angle) { return std::tan(get_angle_as<Radians>(angle)); }

   inline Angle<Radians> acos(double ratio) { return Angle<Radians>(std::acos(ratio)); }

   inline Angle<Radians> asin(double ratio) { return Angle<Radians>(std::asin(ratio)); }

   inline Angle<Radians> atan(double ratio) { return Angle<Radians>(std::atan(ratio)); }

   template<typename... Units>
   Angle<Radians> atan2(const BaseDimension<Units...>& obj1, const BaseDimension<Units...>& obj2)
   {
      //return Angle<Radians>(std::atan2(obj1.template GetVal<NumTuple, DenTuple>() , obj2.template GetVal<NumTuple, DenTuple>()));
      return Angle<Radians>(std::atan2(get_dimension_as<Units...>(obj1) , get_dimension_as<Units...>(obj2)));
   }

}

#endif //DIMENSION_ANGLE_EXTRAS_H