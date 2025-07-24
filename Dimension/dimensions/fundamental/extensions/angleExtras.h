#ifndef DIMENSION_ANGLE_EXTRAS_H
#define DIMENSION_ANGLE_EXTRAS_H

namespace dimension
{
   template<typename angleUnit>
   PrecisionType cos(angle<angleUnit> angle) { return std::cos(get_angle_as<radians>(angle)); }

   template<typename angleUnit>
   PrecisionType sin(angle<angleUnit> angle) { return std::sin(get_angle_as<radians>(angle)); }

   template<typename angleUnit>
   PrecisionType tan(angle<angleUnit> angle) { return std::tan(get_angle_as<radians>(angle)); }

   static_assert( is_angle_unit<radians>, "radians must satisfy is_angle_unit" ); // Passes
   using __force_instantiation = angle<radians>;
   static_assert(sizeof(angle<radians>) > 0, "instantiation test"); // Fails
   static_assert(sizeof(angle<double, radians>) > 0, "instantiation test"); // Passes

   inline angle<radians> acos(double ratio) { return angle<radians>{std::acos(ratio)}; }

   inline angle<radians> asin(double ratio) { return angle<radians>{std::asin(ratio)}; }

   inline angle<radians> atan(double ratio) { return angle<radians>{std::atan(ratio)}; }

   template<typename... Units>
   angle<radians> atan2(const base_dimension_impl<double, Units...>& obj1, const base_dimension_impl<double, Units...>& obj2)
   {
      //return angle<radians>(std::atan2(obj1.template GetVal<NumTuple, DenTuple>() , obj2.template GetVal<NumTuple, DenTuple>()));
      return angle<radians>(std::atan2(get_dimension_as<Units...>(obj1) , get_dimension_as<Units...>(obj2)));
   }

}

#endif //DIMENSION_ANGLE_EXTRAS_H