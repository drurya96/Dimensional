#ifndef STATIC_DIMENSION_FREQUENCY_IMPL_H
#define STATIC_DIMENSION_FREQUENCY_IMPL_H

#include "../../BaseDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Frequency unit
   template<typename T>
   concept IsNamedFrequencyUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 1 &&
         IsTimeUnit<typename std::tuple_element_t<0, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Frequency type
   template<typename T>
   concept IsFrequency = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryTime, -1>
   >>;

   /// @brief Retrieves the value of a Frequency object with specific units
   /// @tparam TimeUnit The Time unit used for all Time components of Frequency
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsTimeUnit TimeUnit,
      IsFrequency DimType>
   constexpr PrecisionType get_frequency_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<TimeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named Frequency object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedFrequencyUnit Named, IsFrequency DimType>
   constexpr PrecisionType get_frequency_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Frequency;

   /// @brief Represents the default Frequency
   template<>
   class Frequency<> : public BaseDimension<
      UnitExponent<PrimaryTime, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryTime, -1>>;
      using Base::Base;

      explicit constexpr Frequency(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsFrequency<Other>
      constexpr Frequency(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Frequency units
   /// @tparam Named The named unit this Frequency type is in terms of
   template<IsNamedFrequencyUnit Named>
   class Frequency<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsFrequency<Other>
      constexpr Frequency(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Frequency<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsFrequency<T>
      constexpr Frequency(const T& base) : Base(base) {}
   };

   template<IsFrequency Dim>
   Frequency(Dim) -> 
   Frequency<
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_FREQUENCY_IMPL_H