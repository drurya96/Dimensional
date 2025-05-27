#ifndef STATIC_DIMENSION_DENSITY_IMPL_H
#define STATIC_DIMENSION_DENSITY_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Density unit
   template<typename T>
   concept IsNamedDensityUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Density type
   template<typename T>
   concept IsDensity = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryLength, -3>
   >>;

   /// @brief Retrieves the value of a Density object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of Density
   /// @tparam LengthUnit The Length unit used for all Length components of Density
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsDensity DimType>
   constexpr PrecisionType get_density_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<LengthUnit, -3>
      >(obj);
   }

   /// @brief Retrieves the value of a named Density object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedDensityUnit Named, IsDensity DimType>
   constexpr PrecisionType get_density_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Density;

   /// @brief Represents the default Density
   template<>
   class Density<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryLength, -3>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryLength, -3>>;
      using Base::Base;

      explicit constexpr Density(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsDensity<Other>
      constexpr Density(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Density units
   /// @tparam Named The named unit this Density type is in terms of
   template<IsNamedDensityUnit Named>
   class Density<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsDensity<Other>
      constexpr Density(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Density<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, -3>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, -3>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsDensity<T>
      constexpr Density(const T& base) : Base(base) {}
   };

   template<IsDensity Dim>
   Density(Dim) -> 
   Density<
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_DENSITY_IMPL_H