#ifndef STATIC_DIMENSION_INDUCTANCE_IMPL_H
#define STATIC_DIMENSION_INDUCTANCE_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"


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
   concept IsNamedinductanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_charge_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a inductance type
   template<typename T>
   concept is_inductance = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, 2>, 
      unit_exponent<primary_charge, -2>
   >>;

   /// @brief Retrieves the value of a inductance object with specific units
   /// @tparam massUnit The mass unit used for all mass components of inductance
   /// @tparam lengthUnit The length unit used for all length components of inductance
   /// @tparam chargeUnit The charge unit used for all charge components of inductance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_charge_unit chargeUnit,
      is_inductance DimType>
   constexpr PrecisionType get_inductance_as(const DimType& obj)
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
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedinductanceUnit Named, is_inductance DimType>
   constexpr PrecisionType get_inductance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
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
   class inductance<T0, T1, T2, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename Extractor<chargeType, T0, T1, T2>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename Extractor<chargeType, T0, T1, T2>::type, -2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_inductance<T>
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
   class inductance<Rep, T0, T1, T2, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename Extractor<chargeType, T0, T1, T2>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename Extractor<chargeType, T0, T1, T2>::type, -2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_inductance<T>
      constexpr inductance(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named inductance units
   /// @tparam Named The named unit this inductance type is in terms of
   template<IsNamedinductanceUnit Named, is_coefficient... Cs>
   class inductance<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_inductance<Other>
      constexpr inductance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named inductance units
   /// @tparam Named The named unit this inductance type is in terms of
   template<rep_type Rep, IsNamedinductanceUnit Named, is_coefficient... Cs>
   class inductance<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_inductance<Other>
      constexpr inductance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




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
   constexpr auto make_inductance(Cs... coeffs)
   {
      return inductance<double, T0, T1, T2, Cs...>(1.0, coeffs...);
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
   constexpr auto make_inductance(Rep value, Cs... coeffs)
   {
      return inductance<Rep, T0, T1, T2, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named inductance units
   /// @tparam Named The named unit this inductance type is in terms of
   template<IsNamedinductanceUnit Named, is_coefficient... Cs>
   constexpr auto make_inductance(Cs... coeffs)
   {
      return inductance<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named inductance units
   /// @tparam Named The named unit this inductance type is in terms of
   template<IsNamedinductanceUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_inductance(Rep value, Cs... coeffs)
   {
      return inductance<Rep, Named, Cs...>(value, coeffs...);
   }









   template<is_inductance Dim>
   inductance(Dim) -> 
   inductance<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<chargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_INDUCTANCE_IMPL_H