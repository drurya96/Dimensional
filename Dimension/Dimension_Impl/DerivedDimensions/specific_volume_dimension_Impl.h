#ifndef STATIC_DIMENSION_SPECIFIC_VOLUME_IMPL_H
#define STATIC_DIMENSION_SPECIFIC_VOLUME_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_specific_volume_units =
         (
               is_length_unit<T0> && 
               is_mass_unit<T1>
         ) ||
         (
               is_mass_unit<T0> && 
               is_length_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named specific_volume unit
   template<typename T>
   concept IsNamedspecific_volumeUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_length_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_mass_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a specific_volume type
   template<typename T>
   concept Isspecific_volume = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarylength, 3>, 
      unit_exponent<Primarymass, -1>
   >>;

   /// @brief Retrieves the value of a specific_volume object with specific units
   /// @tparam lengthUnit The length unit used for all length components of specific_volume
   /// @tparam massUnit The mass unit used for all mass components of specific_volume
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_length_unit lengthUnit,
      is_mass_unit massUnit,
      Isspecific_volume DimType>
   constexpr PrecisionType get_specific_volume_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 3>,
         unit_exponent<massUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named specific_volume object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedspecific_volumeUnit Named, Isspecific_volume DimType>
   constexpr PrecisionType get_specific_volume_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class specific_volume;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_specific_volume_units<
      T0,
      T1
   >
   class specific_volume<T0, T1, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<lengthType, T0, T1>::type, 3>,
      unit_exponent<typename Extractor<massType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<lengthType, T0, T1>::type, 3>,
         unit_exponent<typename Extractor<massType, T0, T1>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isspecific_volume<T>
      constexpr specific_volume(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_specific_volume_units<
      T0,
      T1
   >
   class specific_volume<Rep, T0, T1, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<lengthType, T0, T1>::type, 3>,
      unit_exponent<typename Extractor<massType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<lengthType, T0, T1>::type, 3>,
         unit_exponent<typename Extractor<massType, T0, T1>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isspecific_volume<T>
      constexpr specific_volume(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named specific_volume units
   /// @tparam Named The named unit this specific_volume type is in terms of
   template<IsNamedspecific_volumeUnit Named, is_coefficient... Cs>
   class specific_volume<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires Isspecific_volume<Other>
      constexpr specific_volume(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named specific_volume units
   /// @tparam Named The named unit this specific_volume type is in terms of
   template<rep_type Rep, IsNamedspecific_volumeUnit Named, is_coefficient... Cs>
   class specific_volume<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires Isspecific_volume<Other>
      constexpr specific_volume(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_specific_volume_units<
      T0,
      T1
   >
   constexpr auto make_specific_volume(Cs... coeffs)
   {
      return specific_volume<double, T0, T1, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_specific_volume_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   constexpr auto make_specific_volume(Rep value, Cs... coeffs)
   {
      return specific_volume<Rep, T0, T1, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named specific_volume units
   /// @tparam Named The named unit this specific_volume type is in terms of
   template<IsNamedspecific_volumeUnit Named, is_coefficient... Cs>
   constexpr auto make_specific_volume(Cs... coeffs)
   {
      return specific_volume<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named specific_volume units
   /// @tparam Named The named unit this specific_volume type is in terms of
   template<IsNamedspecific_volumeUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_specific_volume(Rep value, Cs... coeffs)
   {
      return specific_volume<Rep, Named, Cs...>(value, coeffs...);
   }









   template<Isspecific_volume Dim>
   specific_volume(Dim) -> 
   specific_volume<
      DimExtractor<lengthType, Dim>,
      DimExtractor<massType, Dim>
   >;
}

#endif // STATIC_DIMENSION_SPECIFIC_VOLUME_IMPL_H