#ifndef STATIC_DIMENSION_RESISTANCE_IMPL_H
#define STATIC_DIMENSION_RESISTANCE_IMPL_H

#include "../../LengthDimension.h"
#include "../../TimeDimension.h"
#include "../../MassDimension.h"
#include "../../ChargeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Resistance unit.
   /// @tparam NamedResistance The type to be checked as a named Resistance unit.
   template<typename NamedResistance>
   concept IsNamedResistanceUnit = requires {
      typename NamedResistance::NumTuple;
      typename NamedResistance::DenTuple;
   };

   /// @brief Concept for a Resistance dimension.
   /// @details Checks if the provided types satisfy the Resistance dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Charge1, typename Charge2>
   concept IsResistanceUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Charge1::Dim, ChargeType> &&
        std::is_same_v<typename Charge2::Dim, ChargeType>;

   /// @brief Concept for a Resistance type.
   /// @details Ensures that the type meets Resistance type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsResistanceType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsResistanceUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Resistance object.
   /// @details Provides access to the underlying value represented by a Resistance object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   /// @tparam ResistanceType The type of the object being accessed.
   /// @param obj The Resistance object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Charge1, typename Charge2, typename ResistanceType>
   requires IsResistanceUnits<Mass1, Length1, Length2,Time1, Charge1, Charge2> && IsResistanceType<ResistanceType>
   constexpr PrecisionType getResistance(const ResistanceType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Charge1, Charge2>>();
   }

   /// @brief Retrieves the value of a named Resistance object.
   /// @details Provides access to the value represented by a named Resistance object.
   /// @tparam NamedResistance The named unit type.
   /// @tparam ResistanceType The type of the object being accessed.
   /// @param obj The Resistance object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedResistance, typename ResistanceType>
   requires IsNamedResistanceUnit<NamedResistance> && IsResistanceType<ResistanceType>
   constexpr PrecisionType getResistance(const ResistanceType& obj)
   {
      return obj.template GetVal<typename NamedResistance::NumTuple, typename NamedResistance::DenTuple>();
   }

   template<typename... Ts>
   class Resistance;

   /// @brief Represents a default Resistance.
   /// @details This Resistance is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "Resistance" type, without regard for the underlying units.
   template<>
   class Resistance<> : public BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryCharge, PrimaryCharge>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryCharge, PrimaryCharge>>;
      using Base::Base;

      /// @brief Constructs a Resistance object with a value.
      /// @param val The value of the Resistance.
      explicit constexpr Resistance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Resistance object from another Resistance object.
      /// @tparam OtherResistance The other Resistance type.
      /// @param base The base Resistance object.
      template<typename OtherResistance>
      requires IsResistanceType<OtherResistance>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Resistance(const OtherResistance& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryCharge, PrimaryCharge>>()) {}
   };

   /// @brief Represents a Resistance.
   /// @details Defines operations and data storage for Resistance dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Charge1, typename Charge2>
   requires IsResistanceUnits<Mass1, Length1, Length2,Time1, Charge1, Charge2>
   class Resistance<Mass1, Length1, Length2,Time1, Charge1, Charge2> : public BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Charge1, Charge2>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Charge1, Charge2>>;
      using Base::Base;

      /// @brief Constructs a Resistance object with a value.
      /// @param val The value of the Resistance.
      explicit constexpr Resistance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Resistance object from a named unit.
      /// @tparam NamedResistance The named unit type.
      /// @param base The base unit object.
      template<typename NamedResistance>
      requires IsNamedResistanceUnit<NamedResistance>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Resistance(const NamedResistance& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Resistance.
      /// @details Prefer using the free function `getResistance()` instead.
      /// @return The value of the Resistance.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Charge1T, typename Charge2T>
      requires IsResistanceUnits<Mass1T, Length1T, Length2T,Time1T, Charge1T, Charge2T>
      [[deprecated("Use the free function getResistance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetResistance() const
      {
         return getResistance<Mass1T, Length1T, Length2T,Time1T, Charge1T, Charge2T>(*this);
      }

      /// @brief Deprecated function to get the value of Resistance.
      /// @details Prefer using the free function `getResistance()` instead.
      /// @return The value of the Resistance.
      template<typename NamedResistance>
      requires IsNamedResistanceUnit<NamedResistance>
      [[deprecated("Use the free function getResistance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetResistance() const
      {
         return getResistance<NamedResistance>(*this);
      }
   };

   /// @brief Represents a named Resistance class.
   /// @details Provides functionality for named Resistance units.
   /// @tparam NamedResistance The named unit type.
   template<typename NamedResistance>
   requires IsNamedResistanceUnit<NamedResistance>
   class Resistance<NamedResistance> : public BaseDimension<typename NamedResistance::NumTuple, typename NamedResistance::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedResistance::NumTuple, typename NamedResistance::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Resistance object with a value.
      /// @param val The value of the Resistance.
      explicit constexpr Resistance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Resistance object from another Resistance object.
      /// @tparam OtherResistance The other Resistance type.
      /// @param base The base Resistance object.
      template<typename OtherResistance>
      requires IsResistanceType<OtherResistance>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Resistance(const OtherResistance& base)
         : Base(base.template GetVal<typename NamedResistance::NumTuple, typename NamedResistance::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Resistance.
      /// @details Prefer using the free function `getResistance()` instead.
      /// @return The value of the Resistance.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Charge1T, typename Charge2T>
      requires IsResistanceUnits<Mass1T, Length1T, Length2T,Time1T, Charge1T, Charge2T>
      [[deprecated("Use the free function getResistance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetResistance() const
      {
         return getResistance<Mass1T, Length1T, Length2T,Time1T, Charge1T, Charge2T>(*this);
      }

      /// @brief Deprecated function to get the value of Resistance.
      /// @details Prefer using the free function `getResistance()` instead.
      /// @return The value of the Resistance.
      template<typename NamedResistanceUnit>
      requires IsNamedResistanceUnit<NamedResistanceUnit>
      [[deprecated("Use the free function getResistance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetResistance() const
      {
         return getResistance<NamedResistanceUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Resistance.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Charge1, typename Charge2>
   requires IsResistanceUnits<Mass1, Length1, Length2,Time1, Charge1, Charge2>
   Resistance(Mass1, Length1, Length2,Time1, Charge1, Charge2) -> Resistance<Mass1, Length1, Length2,Time1, Charge1, Charge2>;

   /// @brief Template deduction guide for Resistance.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   template<typename NamedResistance>
   requires IsNamedResistanceUnit<NamedResistance>
   Resistance(NamedResistance) -> Resistance<NamedResistance>;

   /// @brief Template deduction guide for Resistance.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Charge1, typename Charge2>
   requires IsResistanceUnits<Mass1, Length1, Length2,Time1, Charge1, Charge2>
   Resistance(BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Charge1, Charge2>>) -> Resistance<Mass1, Length1, Length2,Time1, Charge1, Charge2>;

}

#endif // STATIC_DIMENSION_RESISTANCE_IMPL_H