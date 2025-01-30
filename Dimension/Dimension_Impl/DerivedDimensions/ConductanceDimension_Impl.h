#ifndef STATIC_DIMENSION_CONDUCTANCE_IMPL_H
#define STATIC_DIMENSION_CONDUCTANCE_IMPL_H

#include "../../TimeDimension.h"
#include "../../LengthDimension.h"
#include "../../MassDimension.h"
#include "../../ChargeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Conductance unit.
   /// @tparam NamedConductance The type to be checked as a named Conductance unit.
   template<typename NamedConductance>
   concept IsNamedConductanceUnit = requires {
      typename NamedConductance::NumTuple;
      typename NamedConductance::DenTuple;
   };

   /// @brief Concept for a Conductance dimension.
   /// @details Checks if the provided types satisfy the Conductance dimension requirements.
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   template<typename Time1, typename Charge1, typename Charge2,typename Mass1, typename Length1, typename Length2>
   concept IsConductanceUnits = 
      std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Charge1::Dim, ChargeType> &&
        std::is_same_v<typename Charge2::Dim, ChargeType> &&
        std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType>;

   /// @brief Concept for a Conductance type.
   /// @details Ensures that the type meets Conductance type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsConductanceType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsConductanceUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Conductance object.
   /// @details Provides access to the underlying value represented by a Conductance object.
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   /// @tparam ConductanceType The type of the object being accessed.
   /// @param obj The Conductance object.
   /// @return The underlying value as `PrecisionType`
   template<typename Time1, typename Charge1, typename Charge2,typename Mass1, typename Length1, typename Length2, typename ConductanceType>
   requires IsConductanceUnits<Time1, Charge1, Charge2,Mass1, Length1, Length2> && IsConductanceType<ConductanceType>
   constexpr PrecisionType getConductance(const ConductanceType& obj)
   {
      return obj.template GetVal<std::tuple<Time1, Charge1, Charge2>, std::tuple<Mass1, Length1, Length2>>();
   }

   /// @brief Retrieves the value of a named Conductance object.
   /// @details Provides access to the value represented by a named Conductance object.
   /// @tparam NamedConductance The named unit type.
   /// @tparam ConductanceType The type of the object being accessed.
   /// @param obj The Conductance object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedConductance, typename ConductanceType>
   requires IsNamedConductanceUnit<NamedConductance> && IsConductanceType<ConductanceType>
   constexpr PrecisionType getConductance(const ConductanceType& obj)
   {
      return obj.template GetVal<typename NamedConductance::NumTuple, typename NamedConductance::DenTuple>();
   }

   template<typename... Ts>
   class Conductance;

   /// @brief Represents a default Conductance.
   /// @details This Conductance is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "Conductance" type, without regard for the underlying units.
   template<>
   class Conductance<> : public BaseDimension<std::tuple<PrimaryTime, PrimaryCharge, PrimaryCharge>, std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryTime, PrimaryCharge, PrimaryCharge>, std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>>;
      using Base::Base;

      /// @brief Constructs a Conductance object with a value.
      /// @param val The value of the Conductance.
      explicit constexpr Conductance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Conductance object from another Conductance object.
      /// @tparam OtherConductance The other Conductance type.
      /// @param base The base Conductance object.
      template<typename OtherConductance>
      requires IsConductanceType<OtherConductance>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Conductance(const OtherConductance& base)
         : Base(base.template GetVal<std::tuple<PrimaryTime, PrimaryCharge, PrimaryCharge>, std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>>()) {}
   };

   /// @brief Represents a Conductance.
   /// @details Defines operations and data storage for Conductance dimensions.
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   template<typename Time1, typename Charge1, typename Charge2,typename Mass1, typename Length1, typename Length2>
   requires IsConductanceUnits<Time1, Charge1, Charge2,Mass1, Length1, Length2>
   class Conductance<Time1, Charge1, Charge2,Mass1, Length1, Length2> : public BaseDimension<std::tuple<Time1, Charge1, Charge2>, std::tuple<Mass1, Length1, Length2>>
   {
   public:
      using Base = BaseDimension<std::tuple<Time1, Charge1, Charge2>, std::tuple<Mass1, Length1, Length2>>;
      using Base::Base;

      /// @brief Constructs a Conductance object with a value.
      /// @param val The value of the Conductance.
      explicit constexpr Conductance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Conductance object from a named unit.
      /// @tparam NamedConductance The named unit type.
      /// @param base The base unit object.
      template<typename NamedConductance>
      requires IsNamedConductanceUnit<NamedConductance>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Conductance(const NamedConductance& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Conductance.
      /// @details Prefer using the free function `getConductance()` instead.
      /// @return The value of the Conductance.
      template<typename Time1T, typename Charge1T, typename Charge2T,typename Mass1T, typename Length1T, typename Length2T>
      requires IsConductanceUnits<Time1T, Charge1T, Charge2T,Mass1T, Length1T, Length2T>
      [[deprecated("Use the free function getConductance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetConductance() const
      {
         return getConductance<Time1T, Charge1T, Charge2T,Mass1T, Length1T, Length2T>(*this);
      }

      /// @brief Deprecated function to get the value of Conductance.
      /// @details Prefer using the free function `getConductance()` instead.
      /// @return The value of the Conductance.
      template<typename NamedConductance>
      requires IsNamedConductanceUnit<NamedConductance>
      [[deprecated("Use the free function getConductance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetConductance() const
      {
         return getConductance<NamedConductance>(*this);
      }
   };

   /// @brief Represents a named Conductance class.
   /// @details Provides functionality for named Conductance units.
   /// @tparam NamedConductance The named unit type.
   template<typename NamedConductance>
   requires IsNamedConductanceUnit<NamedConductance>
   class Conductance<NamedConductance> : public BaseDimension<typename NamedConductance::NumTuple, typename NamedConductance::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedConductance::NumTuple, typename NamedConductance::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Conductance object with a value.
      /// @param val The value of the Conductance.
      explicit constexpr Conductance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Conductance object from another Conductance object.
      /// @tparam OtherConductance The other Conductance type.
      /// @param base The base Conductance object.
      template<typename OtherConductance>
      requires IsConductanceType<OtherConductance>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Conductance(const OtherConductance& base)
         : Base(base.template GetVal<typename NamedConductance::NumTuple, typename NamedConductance::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Conductance.
      /// @details Prefer using the free function `getConductance()` instead.
      /// @return The value of the Conductance.
      template<typename Time1T, typename Charge1T, typename Charge2T,typename Mass1T, typename Length1T, typename Length2T>
      requires IsConductanceUnits<Time1T, Charge1T, Charge2T,Mass1T, Length1T, Length2T>
      [[deprecated("Use the free function getConductance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetConductance() const
      {
         return getConductance<Time1T, Charge1T, Charge2T,Mass1T, Length1T, Length2T>(*this);
      }

      /// @brief Deprecated function to get the value of Conductance.
      /// @details Prefer using the free function `getConductance()` instead.
      /// @return The value of the Conductance.
      template<typename NamedConductanceUnit>
      requires IsNamedConductanceUnit<NamedConductanceUnit>
      [[deprecated("Use the free function getConductance() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetConductance() const
      {
         return getConductance<NamedConductanceUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Conductance.
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   template<typename Time1, typename Charge1, typename Charge2,typename Mass1, typename Length1, typename Length2>
   requires IsConductanceUnits<Time1, Charge1, Charge2,Mass1, Length1, Length2>
   Conductance(Time1, Charge1, Charge2,Mass1, Length1, Length2) -> Conductance<Time1, Charge1, Charge2,Mass1, Length1, Length2>;

   /// @brief Template deduction guide for Conductance.
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   template<typename NamedConductance>
   requires IsNamedConductanceUnit<NamedConductance>
   Conductance(NamedConductance) -> Conductance<NamedConductance>;

   /// @brief Template deduction guide for Conductance.
   /// @tparam Time1 Numerator Time1 type
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Charge2 Numerator Charge2 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   template<typename Time1, typename Charge1, typename Charge2,typename Mass1, typename Length1, typename Length2>
   requires IsConductanceUnits<Time1, Charge1, Charge2,Mass1, Length1, Length2>
   Conductance(BaseDimension<std::tuple<Time1, Charge1, Charge2>, std::tuple<Mass1, Length1, Length2>>) -> Conductance<Time1, Charge1, Charge2,Mass1, Length1, Length2>;

}

#endif // STATIC_DIMENSION_CONDUCTANCE_IMPL_H