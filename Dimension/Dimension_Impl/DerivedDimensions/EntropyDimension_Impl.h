#ifndef STATIC_DIMENSION_ENTROPY_IMPL_H
#define STATIC_DIMENSION_ENTROPY_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"
#include "../../TemperatureDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Entropy unit
   template<typename T>
   concept IsNamedEntropyUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         IsTemperatureUnit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Entropy type
   template<typename T>
   concept IsEntropy = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryLength, 2>, 
      UnitExponent<PrimaryTime, -2>, 
      UnitExponent<PrimaryTemperature, -1>
   >>;

   /// @brief Retrieves the value of a Entropy object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of Entropy
   /// @tparam LengthUnit The Length unit used for all Length components of Entropy
   /// @tparam TimeUnit The Time unit used for all Time components of Entropy
   /// @tparam TemperatureUnit The Temperature unit used for all Temperature components of Entropy
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsTemperatureUnit TemperatureUnit,
      IsEntropy DimType>
   constexpr PrecisionType get_entropy_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<LengthUnit, 2>,
         UnitExponent<TimeUnit, -2>,
         UnitExponent<TemperatureUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named Entropy object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedEntropyUnit Named, IsEntropy DimType>
   constexpr PrecisionType get_entropy_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Entropy;

   /// @brief Represents the default Entropy
   template<>
   class Entropy<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryLength, 2>,
      UnitExponent<PrimaryTime, -2>,
      UnitExponent<PrimaryTemperature, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryLength, 2>,
         UnitExponent<PrimaryTime, -2>,
         UnitExponent<PrimaryTemperature, -1>>;
      using Base::Base;

      explicit constexpr Entropy(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsEntropy<Other>
      constexpr Entropy(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Entropy units
   /// @tparam Named The named unit this Entropy type is in terms of
   template<IsNamedEntropyUnit Named>
   class Entropy<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsEntropy<Other>
      constexpr Entropy(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Entropy<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -2>,
      UnitExponent<typename Extractor<TemperatureType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -2>,
         UnitExponent<typename Extractor<TemperatureType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsEntropy<T>
      constexpr Entropy(const T& base) : Base(base) {}
   };

   template<IsEntropy Dim>
   Entropy(Dim) -> 
   Entropy<
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>,
      DimExtractor<TemperatureType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ENTROPY_IMPL_H