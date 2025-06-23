#ifndef STATIC_DIMENSION_MOLAR_MASS_IMPL_H
#define STATIC_DIMENSION_MOLAR_MASS_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/amount_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named molar_mass unit
   template<typename T>
   concept IsNamedmolar_massUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_amount_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a molar_mass type
   template<typename T>
   concept Ismolar_mass = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primaryamount, -1>
   >>;

   /// @brief Retrieves the value of a molar_mass object with specific units
   /// @tparam massUnit The mass unit used for all mass components of molar_mass
   /// @tparam amountUnit The amount unit used for all amount components of molar_mass
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_amount_unit amountUnit,
      Ismolar_mass DimType>
   constexpr PrecisionType get_molar_mass_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<amountUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named molar_mass object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedmolar_massUnit Named, Ismolar_mass DimType>
   constexpr PrecisionType get_molar_mass_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class molar_mass;

   /// @brief Represents the default molar_mass
   template<>
   class molar_mass<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primaryamount, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primaryamount, -1>>;
      using Base::Base;

      explicit constexpr molar_mass(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Ismolar_mass<Other>
      constexpr molar_mass(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named molar_mass units
   /// @tparam Named The named unit this molar_mass type is in terms of
   template<IsNamedmolar_massUnit Named>
   class molar_mass<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Ismolar_mass<Other>
      constexpr molar_mass(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class molar_mass<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<amountType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<amountType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Ismolar_mass<T>
      constexpr molar_mass(const T& base) : Base(base) {}
   };

   template<Ismolar_mass Dim>
   molar_mass(Dim) -> 
   molar_mass<
      DimExtractor<massType, Dim>,
      DimExtractor<amountType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MOLAR_MASS_IMPL_H