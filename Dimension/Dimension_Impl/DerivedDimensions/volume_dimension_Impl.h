#ifndef STATIC_DIMENSION_VOLUME_IMPL_H
#define STATIC_DIMENSION_VOLUME_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"


namespace dimension
{

   template<
         typename T0
   >
   concept are_volume_units =
         (
               is_length_unit<T0>
         )
   ;

   /// @brief Concept to verify a type can serve as a named volume unit
   template<typename T>
   concept IsNamedvolumeUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 1 &&
         is_length_unit<typename std::tuple_element_t<0, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a volume type
   template<typename T>
   concept is_volume = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_length, 3>
   >>;

   /// @brief Retrieves the value of a volume object with specific units
   /// @tparam lengthUnit The length unit used for all length components of volume
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_length_unit lengthUnit,
      is_volume DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr PrecisionType get_volume_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 3>
      >(obj);
   }

   /// @brief Retrieves the value of a named volume object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedvolumeUnit Named, is_volume DimType>
   // TODO: Unit test this and remove suppression
   constexpr PrecisionType get_volume_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class volume;




   template<
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_volume_units<
      T0
   >
   class volume<T0, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<lengthType, T0>::type, 3>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<lengthType, T0>::type, 3>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_volume<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr volume(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_volume_units<
      T0
   >
   class volume<Rep, T0, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<lengthType, T0>::type, 3>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<lengthType, T0>::type, 3>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_volume<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr volume(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named volume units
   /// @tparam Named The named unit this volume type is in terms of
   template<IsNamedvolumeUnit Named, is_coefficient... Cs>
   class volume<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_volume<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr volume(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named volume units
   /// @tparam Named The named unit this volume type is in terms of
   template<rep_type Rep, IsNamedvolumeUnit Named, is_coefficient... Cs>
   class volume<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_volume<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr volume(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      is_coefficient... Cs
   >
   requires are_volume_units<
      T0
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_volume(Cs... coeffs)
   {
      return volume<double, T0, Cs...>(1.0, coeffs...);
   }

   template<
      IsBasicUnitType T0,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_volume_units<
      T0
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_volume(Rep value, Cs... coeffs)
   {
      return volume<Rep, T0, Cs...>(value, coeffs...);
   }

   /// @brief Template specialization for named volume units
   /// @tparam Named The named unit this volume type is in terms of
   template<IsNamedvolumeUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_volume(Cs... coeffs)
   {
      return volume<double, Named, Cs...>(1.0, coeffs...);
   }

   /// @brief Template specialization for named volume units
   /// @tparam Named The named unit this volume type is in terms of
   template<IsNamedvolumeUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_volume(Rep value, Cs... coeffs)
   {
      return volume<Rep, Named, Cs...>(value, coeffs...);
   }

   template<is_volume Dim>
   volume(Dim) -> 
   volume<
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_VOLUME_IMPL_H