#ifndef STATIC_DIMENSION_SPEED_IMPL_H
#define STATIC_DIMENSION_SPEED_IMPL_H

#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "../../Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_speed_units =
         (
               is_length_unit<T0> && 
               is_timespan_unit<T1>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_length_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named speed unit
   template<typename T>
   concept IsNamedspeedUnit =
      (std::tuple_size_v<unit_units_t<T>> == 2) &&
      is_length_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_timespan_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      !is_fundamental_unit_v<T>;

   /// @brief Concept to verify a dimension can be treated as a speed type
   template<typename T, typename Rep>
   concept is_speed_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_length, 1>, 
      unit_exponent<primary_timespan, -1>
   >;

   template<typename T>
   concept is_speed = is_speed_as<T, double>;

   /// @brief Retrieves the value of a speed object with specific units
   /// @tparam lengthUnit The length unit used for all length components of speed
   /// @tparam timespanUnit The timespan unit used for all timespan components of speed
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_speed DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr typename DimType::rep get_speed_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 1>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named speed object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedspeedUnit Named, is_speed DimType>
   // TODO: Unit test this and remove suppression
   constexpr typename DimType::rep get_speed_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class speed;

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_speed_units<
      T0,
      T1
   >
   class speed<T0, T1, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_speed<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr speed(const T& base) : Base(base) {}
   };

   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_speed_units<
      T0,
      T1
   >
   class speed<Rep, T0, T1, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_speed<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr speed(const T& base) : Base(base) {}
   };

   /// @brief Template specialization for named speed units
   /// @tparam Named The named unit this speed type is in terms of
   template<IsNamedspeedUnit Named, is_coefficient... Cs>
   class speed<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_speed<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr speed(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named speed units
   /// @tparam Named The named unit this speed type is in terms of
   template<rep_type Rep, IsNamedspeedUnit Named, is_coefficient... Cs>
   class speed<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_speed<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr speed(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   // ───────────────────── factory helpers (make_*) ─────────────────────
   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_speed_units<
      T0,
      T1
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_speed(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return speed<double, T0, T1, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_speed_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_speed(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return speed<Rep, T0, T1, Cs...>(value);
   }

   /// @brief Template specialization for named speed units
   /// @tparam Named The named unit this speed type is in terms of
   template<IsNamedspeedUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_speed(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return speed<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named speed units
   /// @tparam Named The named unit this speed type is in terms of
   template<IsNamedspeedUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_speed(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return speed<Rep, Named, Cs...>(value);
   }

   // deduction guide
   template<is_speed Dim>
   speed(Dim) ->
   speed<
      simplified_unit_filter<lengthType, typename Dim::units>,
      simplified_unit_filter<timespanType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_SPEED_IMPL_H