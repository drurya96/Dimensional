#ifndef STATIC_DIMENSION_DIFFUSIONCOEFFICIENT_IMPL_H
#define STATIC_DIMENSION_DIFFUSIONCOEFFICIENT_IMPL_H

#include "../../BaseDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named DiffusionCoefficient unit
   template<typename T>
   concept IsNamedDiffusionCoefficientUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsLengthUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a DiffusionCoefficient type
   template<typename T>
   concept IsDiffusionCoefficient = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryLength, 2>, 
      UnitExponent<PrimaryTime, -1>
   >>;

   /// @brief Retrieves the value of a DiffusionCoefficient object with specific units
   /// @tparam LengthUnit The Length unit used for all Length components of DiffusionCoefficient
   /// @tparam TimeUnit The Time unit used for all Time components of DiffusionCoefficient
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsDiffusionCoefficient DimType>
   constexpr PrecisionType get_diffusioncoefficient_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<LengthUnit, 2>,
         UnitExponent<TimeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named DiffusionCoefficient object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedDiffusionCoefficientUnit Named, IsDiffusionCoefficient DimType>
   constexpr PrecisionType get_diffusioncoefficient_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class DiffusionCoefficient;

   /// @brief Represents the default DiffusionCoefficient
   template<>
   class DiffusionCoefficient<> : public BaseDimension<
      UnitExponent<PrimaryLength, 2>,
      UnitExponent<PrimaryTime, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryLength, 2>,
         UnitExponent<PrimaryTime, -1>>;
      using Base::Base;

      explicit constexpr DiffusionCoefficient(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsDiffusionCoefficient<Other>
      constexpr DiffusionCoefficient(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named DiffusionCoefficient units
   /// @tparam Named The named unit this DiffusionCoefficient type is in terms of
   template<IsNamedDiffusionCoefficientUnit Named>
   class DiffusionCoefficient<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsDiffusionCoefficient<Other>
      constexpr DiffusionCoefficient(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class DiffusionCoefficient<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsDiffusionCoefficient<T>
      constexpr DiffusionCoefficient(const T& base) : Base(base) {}
   };

   template<IsDiffusionCoefficient Dim>
   DiffusionCoefficient(Dim) -> 
   DiffusionCoefficient<
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_DIFFUSIONCOEFFICIENT_IMPL_H