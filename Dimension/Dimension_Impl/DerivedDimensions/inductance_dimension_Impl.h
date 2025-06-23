#ifndef STATIC_DIMENSION_INDUCTANCE_IMPL_H
#define STATIC_DIMENSION_INDUCTANCE_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named inductance unit
   template<typename T>
   concept IsNamedinductanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_charge_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a inductance type
   template<typename T>
   concept Isinductance = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarylength, 2>, 
      unit_exponent<Primarycharge, -2>
   >>;

   /// @brief Retrieves the value of a inductance object with specific units
   /// @tparam massUnit The mass unit used for all mass components of inductance
   /// @tparam lengthUnit The length unit used for all length components of inductance
   /// @tparam chargeUnit The charge unit used for all charge components of inductance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_charge_unit chargeUnit,
      Isinductance DimType>
   constexpr PrecisionType get_inductance_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<chargeUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named inductance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedinductanceUnit Named, Isinductance DimType>
   constexpr PrecisionType get_inductance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class inductance;

   /// @brief Represents the default inductance
   template<>
   class inductance<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primarylength, 2>,
      unit_exponent<Primarycharge, -2>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primarylength, 2>,
         unit_exponent<Primarycharge, -2>>;
      using Base::Base;

      explicit constexpr inductance(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isinductance<Other>
      constexpr inductance(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named inductance units
   /// @tparam Named The named unit this inductance type is in terms of
   template<IsNamedinductanceUnit Named>
   class inductance<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isinductance<Other>
      constexpr inductance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class inductance<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
      unit_exponent<typename Extractor<chargeType, Units...>::type, -2>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
         unit_exponent<typename Extractor<chargeType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isinductance<T>
      constexpr inductance(const T& base) : Base(base) {}
   };

   template<Isinductance Dim>
   inductance(Dim) -> 
   inductance<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<chargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_INDUCTANCE_IMPL_H