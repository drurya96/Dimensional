#ifndef STATIC_DIMENSION_MOMENT_OF_INERTIA_IMPL_H
#define STATIC_DIMENSION_MOMENT_OF_INERTIA_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_moment_of_inertia_units =
         (
               is_mass_unit<T0> && 
               is_length_unit<T1>
         ) ||
         (
               is_length_unit<T0> && 
               is_mass_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named moment_of_inertia unit
   template<typename T>
   concept IsNamedmoment_of_inertiaUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a moment_of_inertia type
   template<typename T>
   concept is_moment_of_inertia = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, 2>
   >>;

   /// @brief Retrieves the value of a moment_of_inertia object with specific units
   /// @tparam massUnit The mass unit used for all mass components of moment_of_inertia
   /// @tparam lengthUnit The length unit used for all length components of moment_of_inertia
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_moment_of_inertia DimType>
   constexpr PrecisionType get_moment_of_inertia_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>
      >(obj);
   }

   /// @brief Retrieves the value of a named moment_of_inertia object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedmoment_of_inertiaUnit Named, is_moment_of_inertia DimType>
   constexpr PrecisionType get_moment_of_inertia_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class moment_of_inertia;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_moment_of_inertia_units<
      T0,
      T1
   >
   class moment_of_inertia<T0, T1, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1>::type, 2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1>::type, 2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_moment_of_inertia<T>
      constexpr moment_of_inertia(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_moment_of_inertia_units<
      T0,
      T1
   >
   class moment_of_inertia<Rep, T0, T1, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1>::type, 2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1>::type, 2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_moment_of_inertia<T>
      constexpr moment_of_inertia(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named moment_of_inertia units
   /// @tparam Named The named unit this moment_of_inertia type is in terms of
   template<IsNamedmoment_of_inertiaUnit Named, is_coefficient... Cs>
   class moment_of_inertia<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_moment_of_inertia<Other>
      constexpr moment_of_inertia(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named moment_of_inertia units
   /// @tparam Named The named unit this moment_of_inertia type is in terms of
   template<rep_type Rep, IsNamedmoment_of_inertiaUnit Named, is_coefficient... Cs>
   class moment_of_inertia<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_moment_of_inertia<Other>
      constexpr moment_of_inertia(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_moment_of_inertia_units<
      T0,
      T1
   >
   constexpr auto make_moment_of_inertia(Cs... coeffs)
   {
      return moment_of_inertia<double, T0, T1, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_moment_of_inertia_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   constexpr auto make_moment_of_inertia(Rep value, Cs... coeffs)
   {
      return moment_of_inertia<Rep, T0, T1, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named moment_of_inertia units
   /// @tparam Named The named unit this moment_of_inertia type is in terms of
   template<IsNamedmoment_of_inertiaUnit Named, is_coefficient... Cs>
   constexpr auto make_moment_of_inertia(Cs... coeffs)
   {
      return moment_of_inertia<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named moment_of_inertia units
   /// @tparam Named The named unit this moment_of_inertia type is in terms of
   template<IsNamedmoment_of_inertiaUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_moment_of_inertia(Rep value, Cs... coeffs)
   {
      return moment_of_inertia<Rep, Named, Cs...>(value, coeffs...);
   }









   template<is_moment_of_inertia Dim>
   moment_of_inertia(Dim) -> 
   moment_of_inertia<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MOMENT_OF_INERTIA_IMPL_H