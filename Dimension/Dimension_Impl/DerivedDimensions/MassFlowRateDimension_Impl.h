#ifndef STATIC_DIMENSION_MASSFLOWRATE_IMPL_H
#define STATIC_DIMENSION_MASSFLOWRATE_IMPL_H

#include "../../MassDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named MassFlowRate unit.
   /// @tparam NamedMassFlowRate The type to be checked as a named MassFlowRate unit.
   template<typename NamedMassFlowRate>
   concept IsNamedMassFlowRateUnit = requires {
      typename NamedMassFlowRate::NumTuple;
      typename NamedMassFlowRate::DenTuple;
   };

   /// @brief Concept for a MassFlowRate dimension.
   /// @details Checks if the provided types satisfy the MassFlowRate dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Mass1, typename Time1>
   concept IsMassFlowRateUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
      std::is_same_v<typename Time1::Dim, TimeType>;

   /// @brief Concept for a MassFlowRate type.
   /// @details Ensures that the type meets MassFlowRate type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsMassFlowRateType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 1 &&
   IsMassFlowRateUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<0, typename T::DenTuple>>;

   /// @brief Retrieves the value of a MassFlowRate object.
   /// @details Provides access to the underlying value represented by a MassFlowRate object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam MassFlowRateType The type of the object being accessed.
   /// @param obj The MassFlowRate object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Time1, typename MassFlowRateType>
   requires IsMassFlowRateUnits<Mass1, Time1> && IsMassFlowRateType<MassFlowRateType>
   constexpr PrecisionType getMassFlowRate(const MassFlowRateType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1>, std::tuple<Time1>>();
   }

   /// @brief Retrieves the value of a named MassFlowRate object.
   /// @details Provides access to the value represented by a named MassFlowRate object.
   /// @tparam NamedMassFlowRate The named unit type.
   /// @tparam MassFlowRateType The type of the object being accessed.
   /// @param obj The MassFlowRate object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedMassFlowRate, typename MassFlowRateType>
   requires IsNamedMassFlowRateUnit<NamedMassFlowRate> && IsMassFlowRateType<MassFlowRateType>
   constexpr PrecisionType getMassFlowRate(const MassFlowRateType& obj)
   {
      return obj.template GetVal<typename NamedMassFlowRate::NumTuple, typename NamedMassFlowRate::DenTuple>();
   }

   template<typename... Ts>
   class MassFlowRate;

   /// @brief Represents a default MassFlowRate.
   /// @details This MassFlowRate is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "MassFlowRate" type, without regard for the underlying units.
   template<>
   class MassFlowRate<> : public BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryTime>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryTime>>;
      using Base::Base;

      /// @brief Constructs a MassFlowRate object with a value.
      /// @param val The value of the MassFlowRate.
      explicit constexpr MassFlowRate(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MassFlowRate object from another MassFlowRate object.
      /// @tparam OtherMassFlowRate The other MassFlowRate type.
      /// @param base The base MassFlowRate object.
      template<typename OtherMassFlowRate>
      requires IsMassFlowRateType<OtherMassFlowRate>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MassFlowRate(const OtherMassFlowRate& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass>, std::tuple<PrimaryTime>>()) {}
   };

   /// @brief Represents a MassFlowRate.
   /// @details Defines operations and data storage for MassFlowRate dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Mass1, typename Time1>
   requires IsMassFlowRateUnits<Mass1, Time1>
   class MassFlowRate<Mass1, Time1> : public BaseDimension<std::tuple<Mass1>, std::tuple<Time1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1>, std::tuple<Time1>>;
      using Base::Base;

      /// @brief Constructs a MassFlowRate object with a value.
      /// @param val The value of the MassFlowRate.
      explicit constexpr MassFlowRate(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MassFlowRate object from a named unit.
      /// @tparam NamedMassFlowRate The named unit type.
      /// @param base The base unit object.
      template<typename NamedMassFlowRate>
      requires IsNamedMassFlowRateUnit<NamedMassFlowRate>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MassFlowRate(const NamedMassFlowRate& base) : Base(base) {}

      /// @brief Deprecated function to get the value of MassFlowRate.
      /// @details Prefer using the free function `getMassFlowRate()` instead.
      /// @return The value of the MassFlowRate.
      template<typename Mass1T, typename Time1T>
      requires IsMassFlowRateUnits<Mass1T, Time1T>
      [[deprecated("Use the free function getMassFlowRate() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMassFlowRate() const
      {
         return getMassFlowRate<Mass1T, Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of MassFlowRate.
      /// @details Prefer using the free function `getMassFlowRate()` instead.
      /// @return The value of the MassFlowRate.
      template<typename NamedMassFlowRate>
      requires IsNamedMassFlowRateUnit<NamedMassFlowRate>
      [[deprecated("Use the free function getMassFlowRate() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMassFlowRate() const
      {
         return getMassFlowRate<NamedMassFlowRate>(*this);
      }
   };

   /// @brief Represents a named MassFlowRate class.
   /// @details Provides functionality for named MassFlowRate units.
   /// @tparam NamedMassFlowRate The named unit type.
   template<typename NamedMassFlowRate>
   requires IsNamedMassFlowRateUnit<NamedMassFlowRate>
   class MassFlowRate<NamedMassFlowRate> : public BaseDimension<typename NamedMassFlowRate::NumTuple, typename NamedMassFlowRate::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedMassFlowRate::NumTuple, typename NamedMassFlowRate::DenTuple>;
      using Base::Base;

      /// @brief Constructs a MassFlowRate object with a value.
      /// @param val The value of the MassFlowRate.
      explicit constexpr MassFlowRate(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MassFlowRate object from another MassFlowRate object.
      /// @tparam OtherMassFlowRate The other MassFlowRate type.
      /// @param base The base MassFlowRate object.
      template<typename OtherMassFlowRate>
      requires IsMassFlowRateType<OtherMassFlowRate>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MassFlowRate(const OtherMassFlowRate& base)
         : Base(base.template GetVal<typename NamedMassFlowRate::NumTuple, typename NamedMassFlowRate::DenTuple>()) {}

      /// @brief Deprecated function to get the value of MassFlowRate.
      /// @details Prefer using the free function `getMassFlowRate()` instead.
      /// @return The value of the MassFlowRate.
      template<typename Mass1T, typename Time1T>
      requires IsMassFlowRateUnits<Mass1T, Time1T>
      [[deprecated("Use the free function getMassFlowRate() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMassFlowRate() const
      {
         return getMassFlowRate<Mass1T, Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of MassFlowRate.
      /// @details Prefer using the free function `getMassFlowRate()` instead.
      /// @return The value of the MassFlowRate.
      template<typename NamedMassFlowRateUnit>
      requires IsNamedMassFlowRateUnit<NamedMassFlowRateUnit>
      [[deprecated("Use the free function getMassFlowRate() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMassFlowRate() const
      {
         return getMassFlowRate<NamedMassFlowRateUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for MassFlowRate.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Mass1, typename Time1>
   requires IsMassFlowRateUnits<Mass1, Time1>
   MassFlowRate(Mass1, Time1) -> MassFlowRate<Mass1, Time1>;

   /// @brief Template deduction guide for MassFlowRate.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename NamedMassFlowRate>
   requires IsNamedMassFlowRateUnit<NamedMassFlowRate>
   MassFlowRate(NamedMassFlowRate) -> MassFlowRate<NamedMassFlowRate>;

   /// @brief Template deduction guide for MassFlowRate.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Mass1, typename Time1>
   requires IsMassFlowRateUnits<Mass1, Time1>
   MassFlowRate(BaseDimension<std::tuple<Mass1>, std::tuple<Time1>>) -> MassFlowRate<Mass1, Time1>;

}

#endif // STATIC_DIMENSION_MASSFLOWRATE_IMPL_H