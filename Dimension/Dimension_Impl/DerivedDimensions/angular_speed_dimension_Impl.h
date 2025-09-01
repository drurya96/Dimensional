#ifndef STATIC_DIMENSION_ANGULAR_SPEED_IMPL_H
#define STATIC_DIMENSION_ANGULAR_SPEED_IMPL_H

#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "../../Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

#include "../../dimensions/fundamental/angle_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_angular_speed_units =
         (
               is_angle_unit<T0> && 
               is_timespan_unit<T1>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_angle_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named angular_speed unit
   template<typename T>
   concept IsNamedangular_speedUnit =
      (std::tuple_size_v<unit_units_t<T>> == 2) &&
      is_angle_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_timespan_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      !is_fundamental_unit_v<T>;

   /// @brief Concept to verify a dimension can be treated as a angular_speed type
   template<typename T, typename Rep>
   concept is_angular_speed_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_angle, 1>, 
      unit_exponent<primary_timespan, -1>
   >;

   template<typename T>
   concept is_angular_speed = is_angular_speed_as<T, double>;

   /// @brief Retrieves the value of a angular_speed object with specific units
   /// @tparam angleUnit The angle unit used for all angle components of angular_speed
   /// @tparam timespanUnit The timespan unit used for all timespan components of angular_speed
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_angle_unit angleUnit,
      is_timespan_unit timespanUnit,
      is_angular_speed DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr typename DimType::rep get_angular_speed_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<angleUnit, 1>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named angular_speed object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedangular_speedUnit Named, is_angular_speed DimType>
   // TODO: Unit test this and remove suppression
   constexpr typename DimType::rep get_angular_speed_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class angular_speed;

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_angular_speed_units<
      T0,
      T1
   >
   class angular_speed<T0, T1, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<angleType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<angleType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_angular_speed<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr angular_speed(const T& base) : Base(base) {}
   };

   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_angular_speed_units<
      T0,
      T1
   >
   class angular_speed<Rep, T0, T1, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<angleType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<angleType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_angular_speed<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr angular_speed(const T& base) : Base(base) {}
   };

   /// @brief Template specialization for named angular_speed units
   /// @tparam Named The named unit this angular_speed type is in terms of
   template<IsNamedangular_speedUnit Named, is_coefficient... Cs>
   class angular_speed<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_angular_speed<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr angular_speed(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named angular_speed units
   /// @tparam Named The named unit this angular_speed type is in terms of
   template<rep_type Rep, IsNamedangular_speedUnit Named, is_coefficient... Cs>
   class angular_speed<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_angular_speed<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr angular_speed(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   // ───────────────────── factory helpers (make_*) ─────────────────────
   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_angular_speed_units<
      T0,
      T1
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_angular_speed(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return angular_speed<double, T0, T1, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_angular_speed_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_angular_speed(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return angular_speed<Rep, T0, T1, Cs...>(value);
   }

   /// @brief Template specialization for named angular_speed units
   /// @tparam Named The named unit this angular_speed type is in terms of
   template<IsNamedangular_speedUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_angular_speed(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return angular_speed<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named angular_speed units
   /// @tparam Named The named unit this angular_speed type is in terms of
   template<IsNamedangular_speedUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_angular_speed(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return angular_speed<Rep, Named, Cs...>(value);
   }

   // deduction guide
   template<is_angular_speed Dim>
   angular_speed(Dim) ->
   angular_speed<
      simplified_unit_filter<angleType, typename Dim::units>,
      simplified_unit_filter<timespanType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_ANGULAR_SPEED_IMPL_H