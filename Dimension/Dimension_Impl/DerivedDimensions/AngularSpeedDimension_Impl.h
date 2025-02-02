#ifndef STATIC_DIMENSION_ANGULARSPEED_IMPL_H
#define STATIC_DIMENSION_ANGULARSPEED_IMPL_H

#include "../../AngleDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named AngularSpeed unit.
   /// @tparam NamedAngularSpeed The type to be checked as a named AngularSpeed unit.
   template<typename NamedAngularSpeed>
   concept IsNamedAngularSpeedUnit = requires {
      typename NamedAngularSpeed::NumTuple;
      typename NamedAngularSpeed::DenTuple;
   };

   /// @brief Concept for a AngularSpeed dimension.
   /// @details Checks if the provided types satisfy the AngularSpeed dimension requirements.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Angle1, typename Time1>
   concept IsAngularSpeedUnits = 
      std::is_same_v<typename Angle1::Dim, AngleType> &&
      std::is_same_v<typename Time1::Dim, TimeType>;

   /// @brief Concept for a AngularSpeed type.
   /// @details Ensures that the type meets AngularSpeed type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsAngularSpeedType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 1 &&
   IsAngularSpeedUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<0, typename T::DenTuple>>;

   /// @brief Retrieves the value of a AngularSpeed object.
   /// @details Provides access to the underlying value represented by a AngularSpeed object.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam AngularSpeedType The type of the object being accessed.
   /// @param obj The AngularSpeed object.
   /// @return The underlying value as `PrecisionType`
   template<typename Angle1, typename Time1, typename AngularSpeedType>
   requires IsAngularSpeedUnits<Angle1, Time1> && IsAngularSpeedType<AngularSpeedType>
   constexpr PrecisionType getAngularSpeed(const AngularSpeedType& obj)
   {
      return obj.template GetVal<std::tuple<Angle1>, std::tuple<Time1>>();
   }

   /// @brief Retrieves the value of a named AngularSpeed object.
   /// @details Provides access to the value represented by a named AngularSpeed object.
   /// @tparam NamedAngularSpeed The named unit type.
   /// @tparam AngularSpeedType The type of the object being accessed.
   /// @param obj The AngularSpeed object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedAngularSpeed, typename AngularSpeedType>
   requires IsNamedAngularSpeedUnit<NamedAngularSpeed> && IsAngularSpeedType<AngularSpeedType>
   constexpr PrecisionType getAngularSpeed(const AngularSpeedType& obj)
   {
      return obj.template GetVal<typename NamedAngularSpeed::NumTuple, typename NamedAngularSpeed::DenTuple>();
   }

   template<typename... Ts>
   class AngularSpeed;

   /// @brief Represents a default AngularSpeed.
   /// @details This AngularSpeed is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "AngularSpeed" type, without regard for the underlying units.
   template<>
   class AngularSpeed<> : public BaseDimension<std::tuple<PrimaryAngle>, std::tuple<PrimaryTime>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryAngle>, std::tuple<PrimaryTime>>;
      using Base::Base;

      /// @brief Constructs a AngularSpeed object with a value.
      /// @param val The value of the AngularSpeed.
      explicit constexpr AngularSpeed(PrecisionType val) : Base(val) {}

      /// @brief Constructs a AngularSpeed object from another AngularSpeed object.
      /// @tparam OtherAngularSpeed The other AngularSpeed type.
      /// @param base The base AngularSpeed object.
      template<typename OtherAngularSpeed>
      requires IsAngularSpeedType<OtherAngularSpeed>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr AngularSpeed(const OtherAngularSpeed& base)
         : Base(base.template GetVal<std::tuple<PrimaryAngle>, std::tuple<PrimaryTime>>()) {}
   };

   /// @brief Represents a AngularSpeed.
   /// @details Defines operations and data storage for AngularSpeed dimensions.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Angle1, typename Time1>
   requires IsAngularSpeedUnits<Angle1, Time1>
   class AngularSpeed<Angle1, Time1> : public BaseDimension<std::tuple<Angle1>, std::tuple<Time1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Angle1>, std::tuple<Time1>>;
      using Base::Base;

      /// @brief Constructs a AngularSpeed object with a value.
      /// @param val The value of the AngularSpeed.
      explicit constexpr AngularSpeed(PrecisionType val) : Base(val) {}

      /// @brief Constructs a AngularSpeed object from a named unit.
      /// @tparam NamedAngularSpeed The named unit type.
      /// @param base The base unit object.
      template<typename NamedAngularSpeed>
      requires IsNamedAngularSpeedUnit<NamedAngularSpeed>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr AngularSpeed(const NamedAngularSpeed& base) : Base(base) {}

      /// @brief Deprecated function to get the value of AngularSpeed.
      /// @details Prefer using the free function `getAngularSpeed()` instead.
      /// @return The value of the AngularSpeed.
      template<typename Angle1T, typename Time1T>
      requires IsAngularSpeedUnits<Angle1T, Time1T>
      [[deprecated("Use the free function getAngularSpeed() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetAngularSpeed() const
      {
         return getAngularSpeed<Angle1T, Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of AngularSpeed.
      /// @details Prefer using the free function `getAngularSpeed()` instead.
      /// @return The value of the AngularSpeed.
      template<typename NamedAngularSpeed>
      requires IsNamedAngularSpeedUnit<NamedAngularSpeed>
      [[deprecated("Use the free function getAngularSpeed() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetAngularSpeed() const
      {
         return getAngularSpeed<NamedAngularSpeed>(*this);
      }
   };

   /// @brief Represents a named AngularSpeed class.
   /// @details Provides functionality for named AngularSpeed units.
   /// @tparam NamedAngularSpeed The named unit type.
   template<typename NamedAngularSpeed>
   requires IsNamedAngularSpeedUnit<NamedAngularSpeed>
   class AngularSpeed<NamedAngularSpeed> : public BaseDimension<typename NamedAngularSpeed::NumTuple, typename NamedAngularSpeed::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedAngularSpeed::NumTuple, typename NamedAngularSpeed::DenTuple>;
      using Base::Base;

      /// @brief Constructs a AngularSpeed object with a value.
      /// @param val The value of the AngularSpeed.
      explicit constexpr AngularSpeed(PrecisionType val) : Base(val) {}

      /// @brief Constructs a AngularSpeed object from another AngularSpeed object.
      /// @tparam OtherAngularSpeed The other AngularSpeed type.
      /// @param base The base AngularSpeed object.
      template<typename OtherAngularSpeed>
      requires IsAngularSpeedType<OtherAngularSpeed>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr AngularSpeed(const OtherAngularSpeed& base)
         : Base(base.template GetVal<typename NamedAngularSpeed::NumTuple, typename NamedAngularSpeed::DenTuple>()) {}

      /// @brief Deprecated function to get the value of AngularSpeed.
      /// @details Prefer using the free function `getAngularSpeed()` instead.
      /// @return The value of the AngularSpeed.
      template<typename Angle1T, typename Time1T>
      requires IsAngularSpeedUnits<Angle1T, Time1T>
      [[deprecated("Use the free function getAngularSpeed() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetAngularSpeed() const
      {
         return getAngularSpeed<Angle1T, Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of AngularSpeed.
      /// @details Prefer using the free function `getAngularSpeed()` instead.
      /// @return The value of the AngularSpeed.
      template<typename NamedAngularSpeedUnit>
      requires IsNamedAngularSpeedUnit<NamedAngularSpeedUnit>
      [[deprecated("Use the free function getAngularSpeed() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetAngularSpeed() const
      {
         return getAngularSpeed<NamedAngularSpeedUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for AngularSpeed.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Angle1, typename Time1>
   requires IsAngularSpeedUnits<Angle1, Time1>
   AngularSpeed(Angle1, Time1) -> AngularSpeed<Angle1, Time1>;

   /// @brief Template deduction guide for AngularSpeed.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename NamedAngularSpeed>
   requires IsNamedAngularSpeedUnit<NamedAngularSpeed>
   AngularSpeed(NamedAngularSpeed) -> AngularSpeed<NamedAngularSpeed>;

   /// @brief Template deduction guide for AngularSpeed.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Angle1, typename Time1>
   requires IsAngularSpeedUnits<Angle1, Time1>
   AngularSpeed(BaseDimension<std::tuple<Angle1>, std::tuple<Time1>>) -> AngularSpeed<Angle1, Time1>;

}

#endif // STATIC_DIMENSION_ANGULARSPEED_IMPL_H