#ifndef STATIC_DIMENSION_FREQUENCY_IMPL_H
#define STATIC_DIMENSION_FREQUENCY_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"


namespace dimension
{

   template<
         typename T0
   >
   concept are_frequency_units =
         (
               is_timespan_unit<T0>
         )
   ;

   /// @brief Concept to verify a type can serve as a named frequency unit
   template<typename T>
   concept IsNamedfrequencyUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 1 &&
         is_timespan_unit<typename std::tuple_element_t<0, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a frequency type
   template<typename T>
   concept is_frequency = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_timespan, -1>
   >>;

   /// @brief Retrieves the value of a frequency object with specific units
   /// @tparam timespanUnit The timespan unit used for all timespan components of frequency
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_timespan_unit timespanUnit,
      is_frequency DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr PrecisionType get_frequency_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named frequency object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedfrequencyUnit Named, is_frequency DimType>
   // TODO: Unit test this and remove suppression
   constexpr PrecisionType get_frequency_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class frequency;




   template<
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_frequency_units<
      T0
   >
   class frequency<T0, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<timespanType, T0>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<timespanType, T0>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_frequency<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr frequency(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_frequency_units<
      T0
   >
   class frequency<Rep, T0, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<timespanType, T0>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<timespanType, T0>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_frequency<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr frequency(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named frequency units
   /// @tparam Named The named unit this frequency type is in terms of
   template<IsNamedfrequencyUnit Named, is_coefficient... Cs>
   class frequency<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_frequency<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr frequency(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named frequency units
   /// @tparam Named The named unit this frequency type is in terms of
   template<rep_type Rep, IsNamedfrequencyUnit Named, is_coefficient... Cs>
   class frequency<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_frequency<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr frequency(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_frequency_units<
      T0
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_frequency(Cs... coeffs)
   {
      return frequency<double, T0, Cs...>(1.0, coeffs...);
   }

   template<
      IsBasicUnitType T0,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_frequency_units<
      T0
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_frequency(Rep value, Cs... coeffs)
   {
      return frequency<Rep, T0, Cs...>(value, coeffs...);
   }

   /// @brief Template specialization for named frequency units
   /// @tparam Named The named unit this frequency type is in terms of
   template<IsNamedfrequencyUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_frequency(Cs... coeffs)
   {
      return frequency<double, Named, Cs...>(1.0, coeffs...);
   }

   /// @brief Template specialization for named frequency units
   /// @tparam Named The named unit this frequency type is in terms of
   template<IsNamedfrequencyUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_frequency(Rep value, Cs... coeffs)
   {
      return frequency<Rep, Named, Cs...>(value, coeffs...);
   }

   template<is_frequency Dim>
   frequency(Dim) -> 
   frequency<
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_FREQUENCY_IMPL_H