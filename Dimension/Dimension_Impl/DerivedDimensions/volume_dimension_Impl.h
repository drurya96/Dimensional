#ifndef STATIC_DIMENSION_VOLUME_IMPL_H
#define STATIC_DIMENSION_VOLUME_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"

namespace dimension
{
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
   concept Isvolume = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarylength, 3>
   >>;

   /// @brief Retrieves the value of a volume object with specific units
   /// @tparam lengthUnit The length unit used for all length components of volume
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_length_unit lengthUnit,
      Isvolume DimType>
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
   template<IsNamedvolumeUnit Named, Isvolume DimType>
   constexpr PrecisionType get_volume_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class volume;

   /// @brief Represents the default volume
   template<>
   class volume<> : public base_dimension<
      unit_exponent<Primarylength, 3>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarylength, 3>>;
      using Base::Base;

      explicit constexpr volume(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isvolume<Other>
      constexpr volume(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named volume units
   /// @tparam Named The named unit this volume type is in terms of
   template<IsNamedvolumeUnit Named>
   class volume<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isvolume<Other>
      constexpr volume(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class volume<Units...> : public base_dimension<
      unit_exponent<typename Extractor<lengthType, Units...>::type, 3>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<lengthType, Units...>::type, 3>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isvolume<T>
      constexpr volume(const T& base) : Base(base) {}
   };

   template<Isvolume Dim>
   volume(Dim) -> 
   volume<
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_VOLUME_IMPL_H