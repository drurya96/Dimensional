#ifndef STATIC_DIMENSION_DENSITY_IMPL_H
#define STATIC_DIMENSION_DENSITY_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_density_units =
         (
               is_mass_unit<T0> && 
               is_length_unit<T1>
         ) ||
         (
               is_length_unit<T0> && 
               is_mass_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named density unit
   template<typename T>
   concept IsNameddensityUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a density type
   template<typename T>
   concept is_density = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, -3>
   >>;

   /// @brief Retrieves the value of a density object with specific units
   /// @tparam massUnit The mass unit used for all mass components of density
   /// @tparam lengthUnit The length unit used for all length components of density
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_density DimType>
   constexpr PrecisionType get_density_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, -3>
      >(obj);
   }

   /// @brief Retrieves the value of a named density object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNameddensityUnit Named, is_density DimType>
   constexpr PrecisionType get_density_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class density;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_density_units<
      T0,
      T1
   >
   class density<T0, T1, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1>::type, -3>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1>::type, -3>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_density<T>
      constexpr density(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_density_units<
      T0,
      T1
   >
   class density<Rep, T0, T1, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1>::type, -3>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1>::type, -3>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_density<T>
      constexpr density(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named density units
   /// @tparam Named The named unit this density type is in terms of
   template<IsNameddensityUnit Named, is_coefficient... Cs>
   class density<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_density<Other>
      constexpr density(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named density units
   /// @tparam Named The named unit this density type is in terms of
   template<rep_type Rep, IsNameddensityUnit Named, is_coefficient... Cs>
   class density<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_density<Other>
      constexpr density(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_density_units<
      T0,
      T1
   >
   constexpr auto make_density(Cs... coeffs)
   {
      return density<double, T0, T1, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_density_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   constexpr auto make_density(Rep value, Cs... coeffs)
   {
      return density<Rep, T0, T1, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named density units
   /// @tparam Named The named unit this density type is in terms of
   template<IsNameddensityUnit Named, is_coefficient... Cs>
   constexpr auto make_density(Cs... coeffs)
   {
      return density<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named density units
   /// @tparam Named The named unit this density type is in terms of
   template<IsNameddensityUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_density(Rep value, Cs... coeffs)
   {
      return density<Rep, Named, Cs...>(value, coeffs...);
   }









   template<is_density Dim>
   density(Dim) -> 
   density<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_DENSITY_IMPL_H