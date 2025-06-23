#ifndef STATIC_DIMENSION_MOMENT_OF_INERTIA_IMPL_H
#define STATIC_DIMENSION_MOMENT_OF_INERTIA_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named moment_of_inertia unit
   template<typename T>
   concept IsNamedmoment_of_inertiaUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a moment_of_inertia type
   template<typename T>
   concept Ismoment_of_inertia = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarylength, 2>
   >>;

   /// @brief Retrieves the value of a moment_of_inertia object with specific units
   /// @tparam massUnit The mass unit used for all mass components of moment_of_inertia
   /// @tparam lengthUnit The length unit used for all length components of moment_of_inertia
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      Ismoment_of_inertia DimType>
   constexpr PrecisionType get_moment_of_inertia_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>
      >(obj);
   }

   /// @brief Retrieves the value of a named moment_of_inertia object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedmoment_of_inertiaUnit Named, Ismoment_of_inertia DimType>
   constexpr PrecisionType get_moment_of_inertia_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class moment_of_inertia;

   /// @brief Represents the default moment_of_inertia
   template<>
   class moment_of_inertia<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primarylength, 2>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primarylength, 2>>;
      using Base::Base;

      explicit constexpr moment_of_inertia(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Ismoment_of_inertia<Other>
      constexpr moment_of_inertia(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named moment_of_inertia units
   /// @tparam Named The named unit this moment_of_inertia type is in terms of
   template<IsNamedmoment_of_inertiaUnit Named>
   class moment_of_inertia<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Ismoment_of_inertia<Other>
      constexpr moment_of_inertia(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class moment_of_inertia<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<lengthType, Units...>::type, 2>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<lengthType, Units...>::type, 2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Ismoment_of_inertia<T>
      constexpr moment_of_inertia(const T& base) : Base(base) {}
   };

   template<Ismoment_of_inertia Dim>
   moment_of_inertia(Dim) -> 
   moment_of_inertia<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MOMENT_OF_INERTIA_IMPL_H