#ifndef STATIC_DIMENSION_AREA_IMPL_H
#define STATIC_DIMENSION_AREA_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"


namespace dimension
{

   template<
         typename T0
   >
   concept are_area_units =
         (
               is_length_unit<T0>
         )
   ;

   /// @brief Concept to verify a type can serve as a named area unit
   template<typename T>
   concept IsNamedareaUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 1 &&
         is_length_unit<typename std::tuple_element_t<0, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a area type
   template<typename T>
   concept is_area = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_length, 2>
   >>;

   /// @brief Retrieves the value of a area object with specific units
   /// @tparam lengthUnit The length unit used for all length components of area
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_length_unit lengthUnit,
      is_area DimType>
   constexpr PrecisionType get_area_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 2>
      >(obj);
   }

   /// @brief Retrieves the value of a named area object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedareaUnit Named, is_area DimType>
   constexpr PrecisionType get_area_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class area;




   template<
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_area_units<
      T0
   >
   class area<T0, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<lengthType, T0>::type, 2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<lengthType, T0>::type, 2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_area<T>
      constexpr area(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_area_units<
      T0
   >
   class area<Rep, T0, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<lengthType, T0>::type, 2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<lengthType, T0>::type, 2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_area<T>
      constexpr area(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named area units
   /// @tparam Named The named unit this area type is in terms of
   template<IsNamedareaUnit Named, is_coefficient... Cs>
   class area<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_area<Other>
      constexpr area(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named area units
   /// @tparam Named The named unit this area type is in terms of
   template<rep_type Rep, IsNamedareaUnit Named, is_coefficient... Cs>
   class area<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_area<Other>
      constexpr area(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_area_units<
      T0
   >
   constexpr auto make_area(Cs... coeffs)
   {
      return area<double, T0, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_area_units<
      T0
   > && (!is_coefficient<Rep>)
   constexpr auto make_area(Rep value, Cs... coeffs)
   {
      return area<Rep, T0, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named area units
   /// @tparam Named The named unit this area type is in terms of
   template<IsNamedareaUnit Named, is_coefficient... Cs>
   constexpr auto make_area(Cs... coeffs)
   {
      return area<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named area units
   /// @tparam Named The named unit this area type is in terms of
   template<IsNamedareaUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_area(Rep value, Cs... coeffs)
   {
      return area<Rep, Named, Cs...>(value, coeffs...);
   }









   template<is_area Dim>
   area(Dim) -> 
   area<
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_AREA_IMPL_H