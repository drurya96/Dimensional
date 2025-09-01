#ifndef STATIC_DIMENSION_ENERGY_IMPL_H
#define STATIC_DIMENSION_ENERGY_IMPL_H

#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "../../Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1,
         typename T2
   >
   concept are_energy_units =
         (
               is_mass_unit<T0> && 
               is_length_unit<T1> && 
               is_timespan_unit<T2>
         ) ||
         (
               is_mass_unit<T0> && 
               is_timespan_unit<T1> && 
               is_length_unit<T2>
         ) ||
         (
               is_length_unit<T0> && 
               is_mass_unit<T1> && 
               is_timespan_unit<T2>
         ) ||
         (
               is_length_unit<T0> && 
               is_timespan_unit<T1> && 
               is_mass_unit<T2>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_mass_unit<T1> && 
               is_length_unit<T2>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_length_unit<T1> && 
               is_mass_unit<T2>
         )
   ;

   /// @brief Concept to verify a type can serve as a named energy unit
   template<typename T>
   concept IsNamedenergyUnit =
      (std::tuple_size_v<unit_units_t<T>> == 3) &&
      is_mass_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_length_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      is_timespan_unit<typename std::tuple_element_t<2, unit_units_t<T>>::unit> &&
      !is_fundamental_unit_v<T>;

   /// @brief Concept to verify a dimension can be treated as a energy type
   template<typename T, typename Rep>
   concept is_energy_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, 2>, 
      unit_exponent<primary_timespan, -2>
   >;

   template<typename T>
   concept is_energy = is_energy_as<T, double>;

   /// @brief Retrieves the value of a energy object with specific units
   /// @tparam massUnit The mass unit used for all mass components of energy
   /// @tparam lengthUnit The length unit used for all length components of energy
   /// @tparam timespanUnit The timespan unit used for all timespan components of energy
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_energy DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr typename DimType::rep get_energy_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named energy object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedenergyUnit Named, is_energy DimType>
   // TODO: Unit test this and remove suppression
   constexpr typename DimType::rep get_energy_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class energy;

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_energy_units<
      T0,
      T1,
      T2
   >
   class energy<T0, T1, T2, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -2>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_energy<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr energy(const T& base) : Base(base) {}
   };

   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_energy_units<
      T0,
      T1,
      T2
   >
   class energy<Rep, T0, T1, T2, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -2>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_energy<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr energy(const T& base) : Base(base) {}
   };

   /// @brief Template specialization for named energy units
   /// @tparam Named The named unit this energy type is in terms of
   template<IsNamedenergyUnit Named, is_coefficient... Cs>
   class energy<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_energy<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr energy(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named energy units
   /// @tparam Named The named unit this energy type is in terms of
   template<rep_type Rep, IsNamedenergyUnit Named, is_coefficient... Cs>
   class energy<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_energy<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr energy(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   // ───────────────────── factory helpers (make_*) ─────────────────────
   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_energy_units<
      T0,
      T1,
      T2
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_energy(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return energy<double, T0, T1, T2, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_energy_units<
      T0,
      T1,
      T2
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_energy(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return energy<Rep, T0, T1, T2, Cs...>(value);
   }

   /// @brief Template specialization for named energy units
   /// @tparam Named The named unit this energy type is in terms of
   template<IsNamedenergyUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_energy(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return energy<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named energy units
   /// @tparam Named The named unit this energy type is in terms of
   template<IsNamedenergyUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_energy(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return energy<Rep, Named, Cs...>(value);
   }

   // deduction guide
   template<is_energy Dim>
   energy(Dim) ->
   energy<
      simplified_unit_filter<massType, typename Dim::units>,
      simplified_unit_filter<lengthType, typename Dim::units>,
      simplified_unit_filter<timespanType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_ENERGY_IMPL_H