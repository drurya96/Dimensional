#ifndef STATIC_DIMENSION_AREA_IMPL_H
#define STATIC_DIMENSION_AREA_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named area unit
   template<typename T>
   concept IsNamedareaUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 1 &&
         is_length_unit<typename std::tuple_element_t<0, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a area type
   template<typename T>
   concept Isarea = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarylength, 2>
   >>;

   /// @brief Retrieves the value of a area object with specific units
   /// @tparam lengthUnit The length unit used for all length components of area
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_length_unit lengthUnit,
      Isarea DimType>
   constexpr PrecisionType get_area_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 2>
      >(obj);
   }

   /// @brief Retrieves the value of a named area object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedareaUnit Named, Isarea DimType>
   constexpr PrecisionType get_area_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class area;

   /// @brief Represents the default area
   template<>
   class area<> : public base_dimension<
      unit_exponent<Primarylength, 2>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarylength, 2>>;
      using Base::Base;

      explicit constexpr area(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isarea<Other>
      constexpr area(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named area units
   /// @tparam Named The named unit this area type is in terms of
   template<IsNamedareaUnit Named>
   class area<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isarea<Other>
      constexpr area(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class area<Units...> : public base_dimension<
      unit_exponent<typename Extractor<lengthType, Units...>::type, 2>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<lengthType, Units...>::type, 2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isarea<T>
      constexpr area(const T& base) : Base(base) {}
   };

   template<Isarea Dim>
   area(Dim) -> 
   area<
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_AREA_IMPL_H