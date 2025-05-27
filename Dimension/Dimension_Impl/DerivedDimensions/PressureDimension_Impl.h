#ifndef STATIC_DIMENSION_PRESSURE_IMPL_H
#define STATIC_DIMENSION_PRESSURE_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Pressure unit
   template<typename T>
   concept IsNamedPressureUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Pressure type
   template<typename T>
   concept IsPressure = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryLength, -1>, 
      UnitExponent<PrimaryTime, -2>
   >>;

   /// @brief Retrieves the value of a Pressure object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of Pressure
   /// @tparam LengthUnit The Length unit used for all Length components of Pressure
   /// @tparam TimeUnit The Time unit used for all Time components of Pressure
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsPressure DimType>
   constexpr PrecisionType get_pressure_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<LengthUnit, -1>,
         UnitExponent<TimeUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named Pressure object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedPressureUnit Named, IsPressure DimType>
   constexpr PrecisionType get_pressure_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Pressure;

   /// @brief Represents the default Pressure
   template<>
   class Pressure<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryLength, -1>,
      UnitExponent<PrimaryTime, -2>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryLength, -1>,
         UnitExponent<PrimaryTime, -2>>;
      using Base::Base;

      explicit constexpr Pressure(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsPressure<Other>
      constexpr Pressure(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Pressure units
   /// @tparam Named The named unit this Pressure type is in terms of
   template<IsNamedPressureUnit Named>
   class Pressure<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsPressure<Other>
      constexpr Pressure(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Pressure<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, -1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -2>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, -1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsPressure<T>
      constexpr Pressure(const T& base) : Base(base) {}
   };

   template<IsPressure Dim>
   Pressure(Dim) -> 
   Pressure<
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_PRESSURE_IMPL_H