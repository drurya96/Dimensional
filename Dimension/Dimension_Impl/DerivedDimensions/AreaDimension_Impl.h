#ifndef STATIC_DIMENSION_AREA_IMPL_H
#define STATIC_DIMENSION_AREA_IMPL_H

#include "../../BaseDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Area unit
   template<typename T>
   concept IsNamedAreaUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 1 &&
         IsLengthUnit<typename std::tuple_element_t<0, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Area type
   template<typename T>
   concept IsArea = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryLength, 2>
   >>;

   /// @brief Retrieves the value of a Area object with specific units
   /// @tparam LengthUnit The Length unit used for all Length components of Area
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsLengthUnit LengthUnit,
      IsArea DimType>
   constexpr PrecisionType get_area_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<LengthUnit, 2>
      >(obj);
   }

   /// @brief Retrieves the value of a named Area object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedAreaUnit Named, IsArea DimType>
   constexpr PrecisionType get_area_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Area;

   /// @brief Represents the default Area
   template<>
   class Area<> : public BaseDimension<
      UnitExponent<PrimaryLength, 2>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryLength, 2>>;
      using Base::Base;

      explicit constexpr Area(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsArea<Other>
      constexpr Area(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Area units
   /// @tparam Named The named unit this Area type is in terms of
   template<IsNamedAreaUnit Named>
   class Area<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsArea<Other>
      constexpr Area(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Area<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<LengthType, Units...>::type, 2>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<LengthType, Units...>::type, 2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsArea<T>
      constexpr Area(const T& base) : Base(base) {}
   };

   template<IsArea Dim>
   Area(Dim) -> 
   Area<
      DimExtractor<LengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_AREA_IMPL_H