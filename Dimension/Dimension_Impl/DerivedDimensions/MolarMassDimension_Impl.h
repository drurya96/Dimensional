#ifndef STATIC_DIMENSION_MOLARMASS_IMPL_H
#define STATIC_DIMENSION_MOLARMASS_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../AmountDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named MolarMass unit
   template<typename T>
   concept IsNamedMolarMassUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsAmountUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a MolarMass type
   template<typename T>
   concept IsMolarMass = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryAmount, -1>
   >>;

   /// @brief Retrieves the value of a MolarMass object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of MolarMass
   /// @tparam AmountUnit The Amount unit used for all Amount components of MolarMass
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsAmountUnit AmountUnit,
      IsMolarMass DimType>
   constexpr PrecisionType get_molarmass_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<AmountUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named MolarMass object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedMolarMassUnit Named, IsMolarMass DimType>
   constexpr PrecisionType get_molarmass_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class MolarMass;

   /// @brief Represents the default MolarMass
   template<>
   class MolarMass<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryAmount, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryAmount, -1>>;
      using Base::Base;

      explicit constexpr MolarMass(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsMolarMass<Other>
      constexpr MolarMass(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named MolarMass units
   /// @tparam Named The named unit this MolarMass type is in terms of
   template<IsNamedMolarMassUnit Named>
   class MolarMass<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsMolarMass<Other>
      constexpr MolarMass(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class MolarMass<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<AmountType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<AmountType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsMolarMass<T>
      constexpr MolarMass(const T& base) : Base(base) {}
   };

   template<IsMolarMass Dim>
   MolarMass(Dim) -> 
   MolarMass<
      DimExtractor<MassType, Dim>,
      DimExtractor<AmountType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MOLARMASS_IMPL_H