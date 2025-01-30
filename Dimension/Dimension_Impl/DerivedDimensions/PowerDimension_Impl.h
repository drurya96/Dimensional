#ifndef STATIC_DIMENSION_POWER_IMPL_H
#define STATIC_DIMENSION_POWER_IMPL_H

#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Power unit.
   /// @tparam NamedPower The type to be checked as a named Power unit.
   template<typename NamedPower>
   concept IsNamedPowerUnit = requires {
      typename NamedPower::NumTuple;
      typename NamedPower::DenTuple;
   };

   /// @brief Concept for a Power dimension.
   /// @details Checks if the provided types satisfy the Power dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Time3>
   concept IsPowerUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Time2::Dim, TimeType> &&
        std::is_same_v<typename Time3::Dim, TimeType>;

   /// @brief Concept for a Power type.
   /// @details Ensures that the type meets Power type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsPowerType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsPowerUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Power object.
   /// @details Provides access to the underlying value represented by a Power object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   /// @tparam PowerType The type of the object being accessed.
   /// @param obj The Power object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Time3, typename PowerType>
   requires IsPowerUnits<Mass1, Length1, Length2,Time1, Time2, Time3> && IsPowerType<PowerType>
   constexpr PrecisionType getPower(const PowerType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Time3>>();
   }

   /// @brief Retrieves the value of a named Power object.
   /// @details Provides access to the value represented by a named Power object.
   /// @tparam NamedPower The named unit type.
   /// @tparam PowerType The type of the object being accessed.
   /// @param obj The Power object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedPower, typename PowerType>
   requires IsNamedPowerUnit<NamedPower> && IsPowerType<PowerType>
   constexpr PrecisionType getPower(const PowerType& obj)
   {
      return obj.template GetVal<typename NamedPower::NumTuple, typename NamedPower::DenTuple>();
   }

   template<typename... Ts>
   class Power;

   /// @brief Represents a default Power.
   /// @details This Power is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "Power" type, without regard for the underlying units.
   template<>
   class Power<> : public BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryTime>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryTime>>;
      using Base::Base;

      /// @brief Constructs a Power object with a value.
      /// @param val The value of the Power.
      explicit constexpr Power(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Power object from another Power object.
      /// @tparam OtherPower The other Power type.
      /// @param base The base Power object.
      template<typename OtherPower>
      requires IsPowerType<OtherPower>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Power(const OtherPower& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryTime>>()) {}
   };

   /// @brief Represents a Power.
   /// @details Defines operations and data storage for Power dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Time3>
   requires IsPowerUnits<Mass1, Length1, Length2,Time1, Time2, Time3>
   class Power<Mass1, Length1, Length2,Time1, Time2, Time3> : public BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Time3>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Time3>>;
      using Base::Base;

      /// @brief Constructs a Power object with a value.
      /// @param val The value of the Power.
      explicit constexpr Power(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Power object from a named unit.
      /// @tparam NamedPower The named unit type.
      /// @param base The base unit object.
      template<typename NamedPower>
      requires IsNamedPowerUnit<NamedPower>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Power(const NamedPower& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Power.
      /// @details Prefer using the free function `getPower()` instead.
      /// @return The value of the Power.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Time2T, typename Time3T>
      requires IsPowerUnits<Mass1T, Length1T, Length2T,Time1T, Time2T, Time3T>
      [[deprecated("Use the free function getPower() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetPower() const
      {
         return getPower<Mass1T, Length1T, Length2T,Time1T, Time2T, Time3T>(*this);
      }

      /// @brief Deprecated function to get the value of Power.
      /// @details Prefer using the free function `getPower()` instead.
      /// @return The value of the Power.
      template<typename NamedPower>
      requires IsNamedPowerUnit<NamedPower>
      [[deprecated("Use the free function getPower() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetPower() const
      {
         return getPower<NamedPower>(*this);
      }
   };

   /// @brief Represents a named Power class.
   /// @details Provides functionality for named Power units.
   /// @tparam NamedPower The named unit type.
   template<typename NamedPower>
   requires IsNamedPowerUnit<NamedPower>
   class Power<NamedPower> : public BaseDimension<typename NamedPower::NumTuple, typename NamedPower::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedPower::NumTuple, typename NamedPower::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Power object with a value.
      /// @param val The value of the Power.
      explicit constexpr Power(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Power object from another Power object.
      /// @tparam OtherPower The other Power type.
      /// @param base The base Power object.
      template<typename OtherPower>
      requires IsPowerType<OtherPower>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Power(const OtherPower& base)
         : Base(base.template GetVal<typename NamedPower::NumTuple, typename NamedPower::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Power.
      /// @details Prefer using the free function `getPower()` instead.
      /// @return The value of the Power.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Time2T, typename Time3T>
      requires IsPowerUnits<Mass1T, Length1T, Length2T,Time1T, Time2T, Time3T>
      [[deprecated("Use the free function getPower() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetPower() const
      {
         return getPower<Mass1T, Length1T, Length2T,Time1T, Time2T, Time3T>(*this);
      }

      /// @brief Deprecated function to get the value of Power.
      /// @details Prefer using the free function `getPower()` instead.
      /// @return The value of the Power.
      template<typename NamedPowerUnit>
      requires IsNamedPowerUnit<NamedPowerUnit>
      [[deprecated("Use the free function getPower() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetPower() const
      {
         return getPower<NamedPowerUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Power.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Time3>
   requires IsPowerUnits<Mass1, Length1, Length2,Time1, Time2, Time3>
   Power(Mass1, Length1, Length2,Time1, Time2, Time3) -> Power<Mass1, Length1, Length2,Time1, Time2, Time3>;

   /// @brief Template deduction guide for Power.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   template<typename NamedPower>
   requires IsNamedPowerUnit<NamedPower>
   Power(NamedPower) -> Power<NamedPower>;

   /// @brief Template deduction guide for Power.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Time3>
   requires IsPowerUnits<Mass1, Length1, Length2,Time1, Time2, Time3>
   Power(BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Time3>>) -> Power<Mass1, Length1, Length2,Time1, Time2, Time3>;

}

#endif // STATIC_DIMENSION_POWER_IMPL_H