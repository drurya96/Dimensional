#ifndef STATIC_DIMENSION_SPEED_IMPL_H
#define STATIC_DIMENSION_SPEED_IMPL_H

#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Speed unit.
   /// @tparam NamedSpeed The type to be checked as a named Speed unit.
   template<typename NamedSpeed>
   concept IsNamedSpeedUnit = requires {
      typename NamedSpeed::NumTuple;
      typename NamedSpeed::DenTuple;
   };

   /// @brief Concept for a Speed dimension.
   /// @details Checks if the provided types satisfy the Speed dimension requirements.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Time1>
   concept IsSpeedUnits = 
      std::is_same_v<typename Length1::Dim, LengthType> &&
      std::is_same_v<typename Time1::Dim, TimeType>;

   /// @brief Concept for a Speed type.
   /// @details Ensures that the type meets Speed type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsSpeedType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 1 &&
   IsSpeedUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<0, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Speed object.
   /// @details Provides access to the underlying value represented by a Speed object.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam SpeedType The type of the object being accessed.
   /// @param obj The Speed object.
   /// @return The underlying value as `PrecisionType`
   template<typename Length1, typename Time1, typename SpeedType>
   requires IsSpeedUnits<Length1, Time1> && IsSpeedType<SpeedType>
   constexpr PrecisionType getSpeed(const SpeedType& obj)
   {
      return obj.template GetVal<std::tuple<Length1>, std::tuple<Time1>>();
   }

   /// @brief Retrieves the value of a named Speed object.
   /// @details Provides access to the value represented by a named Speed object.
   /// @tparam NamedSpeed The named unit type.
   /// @tparam SpeedType The type of the object being accessed.
   /// @param obj The Speed object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedSpeed, typename SpeedType>
   requires IsNamedSpeedUnit<NamedSpeed> && IsSpeedType<SpeedType>
   constexpr PrecisionType getSpeed(const SpeedType& obj)
   {
      return obj.template GetVal<typename NamedSpeed::NumTuple, typename NamedSpeed::DenTuple>();
   }

   template<typename... Ts>
   class Speed;

   /// @brief Represents a default Speed.
   /// @details This Speed is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "Speed" type, without regard for the underlying units.
   template<>
   class Speed<> : public BaseDimension<std::tuple<PrimaryLength>, std::tuple<PrimaryTime>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryLength>, std::tuple<PrimaryTime>>;
      using Base::Base;

      /// @brief Constructs a Speed object with a value.
      /// @param val The value of the Speed.
      explicit constexpr Speed(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Speed object from another Speed object.
      /// @tparam OtherSpeed The other Speed type.
      /// @param base The base Speed object.
      template<typename OtherSpeed>
      requires IsSpeedType<OtherSpeed>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Speed(const OtherSpeed& base)
         : Base(base.template GetVal<std::tuple<PrimaryLength>, std::tuple<PrimaryTime>>()) {}
   };

   /// @brief Represents a Speed.
   /// @details Defines operations and data storage for Speed dimensions.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Time1>
   requires IsSpeedUnits<Length1, Time1>
   class Speed<Length1, Time1> : public BaseDimension<std::tuple<Length1>, std::tuple<Time1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Length1>, std::tuple<Time1>>;
      using Base::Base;

      /// @brief Constructs a Speed object with a value.
      /// @param val The value of the Speed.
      explicit constexpr Speed(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Speed object from a named unit.
      /// @tparam NamedSpeed The named unit type.
      /// @param base The base unit object.
      template<typename NamedSpeed>
      requires IsNamedSpeedUnit<NamedSpeed>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Speed(const NamedSpeed& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Speed.
      /// @details Prefer using the free function `getSpeed()` instead.
      /// @return The value of the Speed.
      template<typename Length1T, typename Time1T>
      requires IsSpeedUnits<Length1T, Time1T>
      [[deprecated("Use the free function getSpeed() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetSpeed() const
      {
         return getSpeed<Length1T, Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of Speed.
      /// @details Prefer using the free function `getSpeed()` instead.
      /// @return The value of the Speed.
      template<typename NamedSpeed>
      requires IsNamedSpeedUnit<NamedSpeed>
      [[deprecated("Use the free function getSpeed() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetSpeed() const
      {
         return getSpeed<NamedSpeed>(*this);
      }
   };

   /// @brief Represents a named Speed class.
   /// @details Provides functionality for named Speed units.
   /// @tparam NamedSpeed The named unit type.
   template<typename NamedSpeed>
   requires IsNamedSpeedUnit<NamedSpeed>
   class Speed<NamedSpeed> : public BaseDimension<typename NamedSpeed::NumTuple, typename NamedSpeed::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedSpeed::NumTuple, typename NamedSpeed::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Speed object with a value.
      /// @param val The value of the Speed.
      explicit constexpr Speed(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Speed object from another Speed object.
      /// @tparam OtherSpeed The other Speed type.
      /// @param base The base Speed object.
      template<typename OtherSpeed>
      requires IsSpeedType<OtherSpeed>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Speed(const OtherSpeed& base)
         : Base(base.template GetVal<typename NamedSpeed::NumTuple, typename NamedSpeed::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Speed.
      /// @details Prefer using the free function `getSpeed()` instead.
      /// @return The value of the Speed.
      template<typename Length1T, typename Time1T>
      requires IsSpeedUnits<Length1T, Time1T>
      [[deprecated("Use the free function getSpeed() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetSpeed() const
      {
         return getSpeed<Length1T, Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of Speed.
      /// @details Prefer using the free function `getSpeed()` instead.
      /// @return The value of the Speed.
      template<typename NamedSpeedUnit>
      requires IsNamedSpeedUnit<NamedSpeedUnit>
      [[deprecated("Use the free function getSpeed() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetSpeed() const
      {
         return getSpeed<NamedSpeedUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Speed.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Time1>
   requires IsSpeedUnits<Length1, Time1>
   Speed(Length1, Time1) -> Speed<Length1, Time1>;

   /// @brief Template deduction guide for Speed.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename NamedSpeed>
   requires IsNamedSpeedUnit<NamedSpeed>
   Speed(NamedSpeed) -> Speed<NamedSpeed>;

   /// @brief Template deduction guide for Speed.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Time1>
   requires IsSpeedUnits<Length1, Time1>
   Speed(BaseDimension<std::tuple<Length1>, std::tuple<Time1>>) -> Speed<Length1, Time1>;

}

#endif // STATIC_DIMENSION_SPEED_IMPL_H