#ifndef STATIC_DIMENSION_SPECIFIC_VOLUME_IMPL_H
#define STATIC_DIMENSION_SPECIFIC_VOLUME_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"

namespace dimension
{
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

   /// @brief Represents the default specific_volume
   template<>
   class specific_volume<> : public base_dimension<
      unit_exponent<Primarylength, 3>,
      unit_exponent<Primarymass, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarylength, 3>,
         unit_exponent<Primarymass, -1>>;
      using Base::Base;

      explicit constexpr specific_volume(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isspecific_volume<Other>
      constexpr specific_volume(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named specific_volume units
   /// @tparam Named The named unit this specific_volume type is in terms of
   template<IsNamedspecific_volumeUnit Named>
   class specific_volume<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isspecific_volume<Other>
      constexpr specific_volume(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class specific_volume<Units...> : public base_dimension<
      unit_exponent<typename Extractor<lengthType, Units...>::type, 3>,
      unit_exponent<typename Extractor<massType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<lengthType, Units...>::type, 3>,
         unit_exponent<typename Extractor<massType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isspecific_volume<T>
      constexpr specific_volume(const T& base) : Base(base) {}
   };

   template<Isspecific_volume Dim>
   specific_volume(Dim) -> 
   specific_volume<
      DimExtractor<lengthType, Dim>,
      DimExtractor<massType, Dim>
   >;
}

#endif // STATIC_DIMENSION_SPECIFIC_VOLUME_IMPL_H