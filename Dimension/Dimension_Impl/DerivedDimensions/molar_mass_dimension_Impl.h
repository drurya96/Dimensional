#ifndef STATIC_DIMENSION_MOLAR_MASS_IMPL_H
#define STATIC_DIMENSION_MOLAR_MASS_IMPL_H

#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "../../Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/amount_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_molar_mass_units =
         (
               is_mass_unit<T0> && 
               is_amount_unit<T1>
         ) ||
         (
               is_amount_unit<T0> && 
               is_mass_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named molar_mass unit
   template<typename T>
   concept IsNamedmolar_massUnit =
      (std::tuple_size_v<unit_units_t<T>> == 2) &&
      is_mass_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_amount_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      !is_fundamental_unit_v<T>;

   /// @brief Concept to verify a dimension can be treated as a molar_mass type
   template<typename T, typename Rep>
   concept is_molar_mass_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_amount, -1>
   >;

   template<typename T>
   concept is_molar_mass = is_molar_mass_as<T, double>;

   /// @brief Retrieves the value of a molar_mass object with specific units
   /// @tparam massUnit The mass unit used for all mass components of molar_mass
   /// @tparam amountUnit The amount unit used for all amount components of molar_mass
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_mass_unit massUnit,
      is_amount_unit amountUnit,
      is_molar_mass DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr typename DimType::rep get_molar_mass_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<amountUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named molar_mass object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedmolar_massUnit Named, is_molar_mass DimType>
   // TODO: Unit test this and remove suppression
   constexpr typename DimType::rep get_molar_mass_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class molar_mass;

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_molar_mass_units<
      T0,
      T1
   >
   class molar_mass<T0, T1, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<amountType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<amountType, T0, T1>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_molar_mass<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr molar_mass(const T& base) : Base(base) {}
   };

   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_molar_mass_units<
      T0,
      T1
   >
   class molar_mass<Rep, T0, T1, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<amountType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<amountType, T0, T1>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_molar_mass<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr molar_mass(const T& base) : Base(base) {}
   };

   /// @brief Template specialization for named molar_mass units
   /// @tparam Named The named unit this molar_mass type is in terms of
   template<IsNamedmolar_massUnit Named, is_coefficient... Cs>
   class molar_mass<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_molar_mass<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr molar_mass(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named molar_mass units
   /// @tparam Named The named unit this molar_mass type is in terms of
   template<rep_type Rep, IsNamedmolar_massUnit Named, is_coefficient... Cs>
   class molar_mass<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_molar_mass<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr molar_mass(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   // ───────────────────── factory helpers (make_*) ─────────────────────
   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_molar_mass_units<
      T0,
      T1
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_molar_mass(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return molar_mass<double, T0, T1, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_molar_mass_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_molar_mass(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return molar_mass<Rep, T0, T1, Cs...>(value);
   }

   /// @brief Template specialization for named molar_mass units
   /// @tparam Named The named unit this molar_mass type is in terms of
   template<IsNamedmolar_massUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_molar_mass(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return molar_mass<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named molar_mass units
   /// @tparam Named The named unit this molar_mass type is in terms of
   template<IsNamedmolar_massUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_molar_mass(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return molar_mass<Rep, Named, Cs...>(value);
   }

   // deduction guide
   template<is_molar_mass Dim>
   molar_mass(Dim) ->
   molar_mass<
      simplified_unit_filter<massType, typename Dim::units>,
      simplified_unit_filter<amountType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_MOLAR_MASS_IMPL_H