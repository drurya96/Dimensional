#ifndef STATIC_DIMENSION_DENSITY_IMPL_H
#define STATIC_DIMENSION_DENSITY_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"

namespace dimension
{
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
   concept Isdensity = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarylength, -3>
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
      Isdensity DimType>
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
   template<IsNameddensityUnit Named, Isdensity DimType>
   constexpr PrecisionType get_density_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class density;

   /// @brief Represents the default density
   template<>
   class density<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primarylength, -3>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primarylength, -3>>;
      using Base::Base;

      explicit constexpr density(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isdensity<Other>
      constexpr density(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named density units
   /// @tparam Named The named unit this density type is in terms of
   template<IsNameddensityUnit Named>
   class density<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isdensity<Other>
      constexpr density(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class density<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<lengthType, Units...>::type, -3>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<lengthType, Units...>::type, -3>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isdensity<T>
      constexpr density(const T& base) : Base(base) {}
   };

   template<Isdensity Dim>
   density(Dim) -> 
   density<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_DENSITY_IMPL_H