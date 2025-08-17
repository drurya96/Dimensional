#ifndef STATIC_DIMENSION_MOMENT_OF_INERTIA_IMPL_H
#define STATIC_DIMENSION_MOMENT_OF_INERTIA_IMPL_H

#include "../../base_unit.h"
#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"

#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


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
   concept IsNamedmoment_of_inertiaUnit =
      (std::tuple_size_v<unit_units_t<T>> == 2) &&
      is_mass_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_length_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      (!std::is_base_of_v<FundamentalUnitTag, T>);

   /// @brief Concept to verify a dimension can be treated as a moment_of_inertia type
   template<typename T, typename Rep>
   concept is_moment_of_inertia_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, 2>
   >;

   template<typename T>
   concept is_moment_of_inertia = is_moment_of_inertia_as<T, double>;

   /// @brief Retrieves the value of a moment_of_inertia object with specific units
   /// @tparam massUnit The mass unit used for all mass components of moment_of_inertia
   /// @tparam lengthUnit The length unit used for all length components of moment_of_inertia
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_moment_of_inertia DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr DimType::rep get_moment_of_inertia_as(const DimType& obj)
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
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedmoment_of_inertiaUnit Named, is_moment_of_inertia DimType>
   // TODO: Unit test this and remove suppression
   constexpr DimType::rep get_moment_of_inertia_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
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
   class moment_of_inertia<T0, T1, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_moment_of_inertia<T>
      // cppcheck-suppress noExplicitConstructor
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
   class moment_of_inertia<Rep, T0, T1, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_moment_of_inertia<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr moment_of_inertia(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named moment_of_inertia units
   /// @tparam Named The named unit this moment_of_inertia type is in terms of
   template<IsNamedmoment_of_inertiaUnit Named, is_coefficient... Cs>
   class moment_of_inertia<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_moment_of_inertia<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr moment_of_inertia(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named moment_of_inertia units
   /// @tparam Named The named unit this moment_of_inertia type is in terms of
   template<rep_type Rep, IsNamedmoment_of_inertiaUnit Named, is_coefficient... Cs>
   class moment_of_inertia<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_moment_of_inertia<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr moment_of_inertia(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
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
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_moment_of_inertia(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return moment_of_inertia<double, T0, T1, Cs...>(1.0);
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
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_moment_of_inertia(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return moment_of_inertia<Rep, T0, T1, Cs...>(value);
   }

   /// @brief Template specialization for named moment_of_inertia units
   /// @tparam Named The named unit this moment_of_inertia type is in terms of
   template<IsNamedmoment_of_inertiaUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_moment_of_inertia(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return moment_of_inertia<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named moment_of_inertia units
   /// @tparam Named The named unit this moment_of_inertia type is in terms of
   template<IsNamedmoment_of_inertiaUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_moment_of_inertia(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return moment_of_inertia<Rep, Named, Cs...>(value);
   }

   template<is_moment_of_inertia Dim>
   moment_of_inertia(Dim) -> 
   moment_of_inertia<
      simplified_unit_filter<massType, typename Dim::units>,
      simplified_unit_filter<lengthType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_MOMENT_OF_INERTIA_IMPL_H