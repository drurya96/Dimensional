#ifndef STATIC_DIMENSION_INDUCTANCE_IMPL_H
#define STATIC_DIMENSION_INDUCTANCE_IMPL_H

#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "../../Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1,
         typename T2
   >
   concept are_inductance_units =
         (
               is_mass_unit<T0> && 
               is_length_unit<T1> && 
               is_charge_unit<T2>
         ) ||
         (
               is_mass_unit<T0> && 
               is_charge_unit<T1> && 
               is_length_unit<T2>
         ) ||
         (
               is_length_unit<T0> && 
               is_mass_unit<T1> && 
               is_charge_unit<T2>
         ) ||
         (
               is_length_unit<T0> && 
               is_charge_unit<T1> && 
               is_mass_unit<T2>
         ) ||
         (
               is_charge_unit<T0> && 
               is_mass_unit<T1> && 
               is_length_unit<T2>
         ) ||
         (
               is_charge_unit<T0> && 
               is_length_unit<T1> && 
               is_mass_unit<T2>
         )
   ;

   /// @brief Concept to verify a type can serve as a named inductance unit
   template<typename T>
   concept IsNamedinductanceUnit =
      (std::tuple_size_v<unit_units_t<T>> == 3) &&
      is_mass_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_length_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      is_charge_unit<typename std::tuple_element_t<2, unit_units_t<T>>::unit> &&
      !is_fundamental_unit_v<T>;

   /// @brief Concept to verify a dimension can be treated as a inductance type
   template<typename T, typename Rep>
   concept is_inductance_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, 2>, 
      unit_exponent<primary_charge, -2>
   >;

   template<typename T>
   concept is_inductance = is_inductance_as<T, double>;

   /// @brief Retrieves the value of a inductance object with specific units
   /// @tparam massUnit The mass unit used for all mass components of inductance
   /// @tparam lengthUnit The length unit used for all length components of inductance
   /// @tparam chargeUnit The charge unit used for all charge components of inductance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_charge_unit chargeUnit,
      is_inductance DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr typename DimType::rep get_inductance_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<chargeUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named inductance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedinductanceUnit Named, is_inductance DimType>
   // TODO: Unit test this and remove suppression
   constexpr typename DimType::rep get_inductance_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class inductance;

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_inductance_units<
      T0,
      T1,
      T2
   >
   class inductance<T0, T1, T2, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename unit_filter<chargeType, T0, T1, T2>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename unit_filter<chargeType, T0, T1, T2>::type, -2>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_inductance<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr inductance(const T& base) : Base(base) {}
   };

   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_inductance_units<
      T0,
      T1,
      T2
   >
   class inductance<Rep, T0, T1, T2, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename unit_filter<chargeType, T0, T1, T2>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename unit_filter<chargeType, T0, T1, T2>::type, -2>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_inductance<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr inductance(const T& base) : Base(base) {}
   };

   /// @brief Template specialization for named inductance units
   /// @tparam Named The named unit this inductance type is in terms of
   template<IsNamedinductanceUnit Named, is_coefficient... Cs>
   class inductance<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_inductance<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr inductance(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named inductance units
   /// @tparam Named The named unit this inductance type is in terms of
   template<rep_type Rep, IsNamedinductanceUnit Named, is_coefficient... Cs>
   class inductance<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_inductance<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr inductance(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   // ───────────────────── factory helpers (make_*) ─────────────────────
   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_inductance_units<
      T0,
      T1,
      T2
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_inductance(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return inductance<double, T0, T1, T2, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_inductance_units<
      T0,
      T1,
      T2
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_inductance(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return inductance<Rep, T0, T1, T2, Cs...>(value);
   }

   /// @brief Template specialization for named inductance units
   /// @tparam Named The named unit this inductance type is in terms of
   template<IsNamedinductanceUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_inductance(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return inductance<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named inductance units
   /// @tparam Named The named unit this inductance type is in terms of
   template<IsNamedinductanceUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_inductance(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return inductance<Rep, Named, Cs...>(value);
   }

   // deduction guide
   template<is_inductance Dim>
   inductance(Dim) ->
   inductance<
      simplified_unit_filter<massType, typename Dim::units>,
      simplified_unit_filter<lengthType, typename Dim::units>,
      simplified_unit_filter<chargeType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_INDUCTANCE_IMPL_H