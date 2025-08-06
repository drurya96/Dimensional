#ifndef STATIC_DIMENSION_CURRENT_IMPL_H
#define STATIC_DIMENSION_CURRENT_IMPL_H

#include "../../base_unit.h"
#include "../../base_dimension_impl.h"
#include "../../dimensions/fundamental/charge_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_current_units =
         (
               is_charge_unit<T0> && 
               is_timespan_unit<T1>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_charge_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named current unit
   template<typename T>
   concept IsNamedcurrentUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_charge_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a current type
   template<typename T, typename Rep>
   concept is_current_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_charge, 1>, 
      unit_exponent<primary_timespan, -1>
   >;

   template<typename T>
   concept is_current = is_current_as<T, double>;

   /// @brief Retrieves the value of a current object with specific units
   /// @tparam chargeUnit The charge unit used for all charge components of current
   /// @tparam timespanUnit The timespan unit used for all timespan components of current
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_charge_unit chargeUnit,
      is_timespan_unit timespanUnit,
      is_current DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr DimType::rep get_current_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<chargeUnit, 1>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named current object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedcurrentUnit Named, is_current DimType>
   // TODO: Unit test this and remove suppression
   constexpr DimType::rep get_current_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class current;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_current_units<
      T0,
      T1
   >
   class current<T0, T1, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<chargeType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<chargeType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_current<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr current(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_current_units<
      T0,
      T1
   >
   class current<Rep, T0, T1, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<chargeType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<chargeType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_current<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr current(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named current units
   /// @tparam Named The named unit this current type is in terms of
   template<IsNamedcurrentUnit Named, is_coefficient... Cs>
   class current<Named, Cs...> : public base_dimension_from_tuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_current<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr current(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named current units
   /// @tparam Named The named unit this current type is in terms of
   template<rep_type Rep, IsNamedcurrentUnit Named, is_coefficient... Cs>
   class current<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_current<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr current(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_current_units<
      T0,
      T1
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_current(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return current<double, T0, T1, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_current_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_current(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return current<Rep, T0, T1, Cs...>(value);
   }

   /// @brief Template specialization for named current units
   /// @tparam Named The named unit this current type is in terms of
   template<IsNamedcurrentUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_current(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return current<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named current units
   /// @tparam Named The named unit this current type is in terms of
   template<IsNamedcurrentUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_current(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return current<Rep, Named, Cs...>(value);
   }

   template<is_current Dim>
   current(Dim) -> 
   current<
      simplified_unit_filter<chargeType, typename Dim::units>,
      simplified_unit_filter<timespanType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_CURRENT_IMPL_H