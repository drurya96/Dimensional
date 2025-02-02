#ifndef STATIC_DIMENSION_CAPACITANCE_IMPL_H
#define STATIC_DIMENSION_CAPACITANCE_IMPL_H

#include "../../ChargeDimension.h"
#include "../../TimeDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Capacitance unit.
   /// @tparam NamedCapacitance The type to be checked as a named Capacitance unit.
   template<typename NamedCapacitance>
   concept IsNamedCapacitanceUnit = requires {
      typename NamedCapacitance::NumTuple;
      typename NamedCapacitance::DenTuple;
   };

   /// @brief Concept for a Capacitance dimension.
   /// @details Checks if the provided types satisfy the Capacitance dimension requirements.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Time2 Numerator Time2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   template<typename Charge1, typename Charge2, typename Time1, typename Time2, typename Mass1, typename Length1, typename Length2>
   concept IsCapacitanceUnits = 
      std::is_same_v<typename Charge1::Dim, ChargeType> &&
      std::is_same_v<typename Charge2::Dim, ChargeType> &&
      std::is_same_v<typename Time1::Dim, TimeType> &&
      std::is_same_v<typename Time2::Dim, TimeType> &&
      std::is_same_v<typename Mass1::Dim, MassType> &&
      std::is_same_v<typename Length1::Dim, LengthType> &&
      std::is_same_v<typename Length2::Dim, LengthType>;

   /// @brief Concept for a Capacitance type.
   /// @details Ensures that the type meets Capacitance type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsCapacitanceType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 4 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsCapacitanceUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>, typename std::tuple_element_t<3, typename T::NumTuple>, typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Capacitance object.
   /// @details Provides access to the underlying value represented by a Capacitance object.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Time2 Numerator Time2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   /// @tparam CapacitanceType The type of the object being accessed.
   /// @param obj The Capacitance object.
   /// @return The underlying value as `PrecisionType`
   template<typename Charge1, typename Charge2, typename Time1, typename Time2, typename Mass1, typename Length1, typename Length2, typename CapacitanceType>
   requires IsCapacitanceUnits<Charge1, Charge2, Time1, Time2, Mass1, Length1, Length2> && IsCapacitanceType<CapacitanceType>
   constexpr PrecisionType getCapacitance(const CapacitanceType& obj)
   {
      return obj.template GetVal<std::tuple<Charge1, Charge2, Time1, Time2>, std::tuple<Mass1, Length1, Length2>>();
   }

   /// @brief Retrieves the value of a named Capacitance object.
   /// @details Provides access to the value represented by a named Capacitance object.
   /// @tparam NamedCapacitance The named unit type.
   /// @tparam CapacitanceType The type of the object being accessed.
   /// @param obj The Capacitance object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedCapacitance, typename CapacitanceType>
   requires IsNamedCapacitanceUnit<NamedCapacitance> && IsCapacitanceType<CapacitanceType>
   constexpr PrecisionType getCapacitance(const CapacitanceType& obj)
   {
      return obj.template GetVal<typename NamedCapacitance::NumTuple, typename NamedCapacitance::DenTuple>();
   }

   template<typename... Ts>
   class Capacitance;

   /// @brief Represents a default Capacitance.
   /// @details This Capacitance is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "Capacitance" type, without regard for the underlying units.
   template<>
   class Capacitance<> : public BaseDimension<std::tuple<PrimaryCharge, PrimaryCharge, PrimaryTime, PrimaryTime>, std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryCharge, PrimaryCharge, PrimaryTime, PrimaryTime>, std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>>;
      using Base::Base;

      /// @brief Constructs a Capacitance object with a value.
      /// @param val The value of the Capacitance.
      explicit constexpr Capacitance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Capacitance object from another Capacitance object.
      /// @tparam OtherCapacitance The other Capacitance type.
      /// @param base The base Capacitance object.
      template<typename OtherCapacitance>
      requires IsCapacitanceType<OtherCapacitance>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Capacitance(const OtherCapacitance& base)
         : Base(base.template GetVal<std::tuple<PrimaryCharge, PrimaryCharge, PrimaryTime, PrimaryTime>, std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>>()) {}
   };

   /// @brief Represents a Capacitance.
   /// @details Defines operations and data storage for Capacitance dimensions.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Time2 Numerator Time2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   template<typename Charge1, typename Charge2, typename Time1, typename Time2, typename Mass1, typename Length1, typename Length2>
   requires IsCapacitanceUnits<Charge1, Charge2, Time1, Time2, Mass1, Length1, Length2>
   class Capacitance<Charge1, Charge2, Time1, Time2, Mass1, Length1, Length2> : public BaseDimension<std::tuple<Charge1, Charge2, Time1, Time2>, std::tuple<Mass1, Length1, Length2>>
   {
   public:
      using Base = BaseDimension<std::tuple<Charge1, Charge2, Time1, Time2>, std::tuple<Mass1, Length1, Length2>>;
      using Base::Base;

      /// @brief Constructs a Capacitance object with a value.
      /// @param val The value of the Capacitance.
      explicit constexpr Capacitance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Capacitance object from a named unit.
      /// @tparam NamedCapacitance The named unit type.
      /// @param base The base unit object.
      template<typename NamedCapacitance>
      requires IsNamedCapacitanceUnit<NamedCapacitance>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Capacitance(const NamedCapacitance& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Capacitance.
      /// @details Prefer using the free function `getCapacitance()` instead.
      /// @return The value of the Capacitance.
      template<typename Charge1T, typename Charge2T, typename Time1T, typename Time2T, typename Mass1T, typename Length1T, typename Length2T>
      requires IsCapacitanceUnits<Charge1T, Charge2T, Time1T, Time2T, Mass1T, Length1T, Length2T>
      [[deprecated("Use the free function getCapacitance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetCapacitance() const
      {
         return getCapacitance<Charge1T, Charge2T, Time1T, Time2T, Mass1T, Length1T, Length2T>(*this);
      }

      /// @brief Deprecated function to get the value of Capacitance.
      /// @details Prefer using the free function `getCapacitance()` instead.
      /// @return The value of the Capacitance.
      template<typename NamedCapacitance>
      requires IsNamedCapacitanceUnit<NamedCapacitance>
      [[deprecated("Use the free function getCapacitance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetCapacitance() const
      {
         return getCapacitance<NamedCapacitance>(*this);
      }
   };

   /// @brief Represents a named Capacitance class.
   /// @details Provides functionality for named Capacitance units.
   /// @tparam NamedCapacitance The named unit type.
   template<typename NamedCapacitance>
   requires IsNamedCapacitanceUnit<NamedCapacitance>
   class Capacitance<NamedCapacitance> : public BaseDimension<typename NamedCapacitance::NumTuple, typename NamedCapacitance::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedCapacitance::NumTuple, typename NamedCapacitance::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Capacitance object with a value.
      /// @param val The value of the Capacitance.
      explicit constexpr Capacitance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Capacitance object from another Capacitance object.
      /// @tparam OtherCapacitance The other Capacitance type.
      /// @param base The base Capacitance object.
      template<typename OtherCapacitance>
      requires IsCapacitanceType<OtherCapacitance>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Capacitance(const OtherCapacitance& base)
         : Base(base.template GetVal<typename NamedCapacitance::NumTuple, typename NamedCapacitance::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Capacitance.
      /// @details Prefer using the free function `getCapacitance()` instead.
      /// @return The value of the Capacitance.
      template<typename Charge1T, typename Charge2T, typename Time1T, typename Time2T, typename Mass1T, typename Length1T, typename Length2T>
      requires IsCapacitanceUnits<Charge1T, Charge2T, Time1T, Time2T, Mass1T, Length1T, Length2T>
      [[deprecated("Use the free function getCapacitance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetCapacitance() const
      {
         return getCapacitance<Charge1T, Charge2T, Time1T, Time2T, Mass1T, Length1T, Length2T>(*this);
      }

      /// @brief Deprecated function to get the value of Capacitance.
      /// @details Prefer using the free function `getCapacitance()` instead.
      /// @return The value of the Capacitance.
      template<typename NamedCapacitanceUnit>
      requires IsNamedCapacitanceUnit<NamedCapacitanceUnit>
      [[deprecated("Use the free function getCapacitance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetCapacitance() const
      {
         return getCapacitance<NamedCapacitanceUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Capacitance.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Time2 Numerator Time2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   template<typename Charge1, typename Charge2, typename Time1, typename Time2, typename Mass1, typename Length1, typename Length2>
   requires IsCapacitanceUnits<Charge1, Charge2, Time1, Time2, Mass1, Length1, Length2>
   Capacitance(Charge1, Charge2, Time1, Time2, Mass1, Length1, Length2) -> Capacitance<Charge1, Charge2, Time1, Time2, Mass1, Length1, Length2>;

   /// @brief Template deduction guide for Capacitance.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Time2 Numerator Time2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   template<typename NamedCapacitance>
   requires IsNamedCapacitanceUnit<NamedCapacitance>
   Capacitance(NamedCapacitance) -> Capacitance<NamedCapacitance>;

   /// @brief Template deduction guide for Capacitance.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Time2 Numerator Time2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   template<typename Charge1, typename Charge2, typename Time1, typename Time2, typename Mass1, typename Length1, typename Length2>
   requires IsCapacitanceUnits<Charge1, Charge2, Time1, Time2, Mass1, Length1, Length2>
   Capacitance(BaseDimension<std::tuple<Charge1, Charge2, Time1, Time2>, std::tuple<Mass1, Length1, Length2>>) -> Capacitance<Charge1, Charge2, Time1, Time2, Mass1, Length1, Length2>;

}

#endif // STATIC_DIMENSION_CAPACITANCE_IMPL_H