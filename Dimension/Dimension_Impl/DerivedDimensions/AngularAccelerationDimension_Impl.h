#ifndef STATIC_DIMENSION_ANGULARACCELERATION_IMPL_H
#define STATIC_DIMENSION_ANGULARACCELERATION_IMPL_H

#include "../../AngleDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named AngularAcceleration unit.
   /// @tparam NamedAngularAcceleration The type to be checked as a named AngularAcceleration unit.
   template<typename NamedAngularAcceleration>
   concept IsNamedAngularAccelerationUnit = requires {
      typename NamedAngularAcceleration::NumTuple;
      typename NamedAngularAcceleration::DenTuple;
   };

   /// @brief Concept for a AngularAcceleration dimension.
   /// @details Checks if the provided types satisfy the AngularAcceleration dimension requirements.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Angle1, typename Time1, typename Time2>
   concept IsAngularAccelerationUnits = 
      std::is_same_v<typename Angle1::Dim, AngleType> &&
      std::is_same_v<typename Time1::Dim, TimeType> &&
      std::is_same_v<typename Time2::Dim, TimeType>;

   /// @brief Concept for a AngularAcceleration type.
   /// @details Ensures that the type meets AngularAcceleration type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsAngularAccelerationType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 2 &&
   IsAngularAccelerationUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>>;

   /// @brief Retrieves the value of a AngularAcceleration object.
   /// @details Provides access to the underlying value represented by a AngularAcceleration object.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam AngularAccelerationType The type of the object being accessed.
   /// @param obj The AngularAcceleration object.
   /// @return The underlying value as `PrecisionType`
   template<typename Angle1, typename Time1, typename Time2, typename AngularAccelerationType>
   requires IsAngularAccelerationUnits<Angle1, Time1, Time2> && IsAngularAccelerationType<AngularAccelerationType>
   constexpr PrecisionType getAngularAcceleration(const AngularAccelerationType& obj)
   {
      return obj.template GetVal<std::tuple<Angle1>, std::tuple<Time1, Time2>>();
   }

   /// @brief Retrieves the value of a named AngularAcceleration object.
   /// @details Provides access to the value represented by a named AngularAcceleration object.
   /// @tparam NamedAngularAcceleration The named unit type.
   /// @tparam AngularAccelerationType The type of the object being accessed.
   /// @param obj The AngularAcceleration object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedAngularAcceleration, typename AngularAccelerationType>
   requires IsNamedAngularAccelerationUnit<NamedAngularAcceleration> && IsAngularAccelerationType<AngularAccelerationType>
   constexpr PrecisionType getAngularAcceleration(const AngularAccelerationType& obj)
   {
      return obj.template GetVal<typename NamedAngularAcceleration::NumTuple, typename NamedAngularAcceleration::DenTuple>();
   }

   template<typename... Ts>
   class AngularAcceleration;

   /// @brief Represents a default AngularAcceleration.
   /// @details This AngularAcceleration is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "AngularAcceleration" type, without regard for the underlying units.
   template<>
   class AngularAcceleration<> : public BaseDimension<std::tuple<PrimaryAngle>, std::tuple<PrimaryTime, PrimaryTime>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryAngle>, std::tuple<PrimaryTime, PrimaryTime>>;
      using Base::Base;

      /// @brief Constructs a AngularAcceleration object with a value.
      /// @param val The value of the AngularAcceleration.
      explicit constexpr AngularAcceleration(PrecisionType val) : Base(val) {}

      /// @brief Constructs a AngularAcceleration object from another AngularAcceleration object.
      /// @tparam OtherAngularAcceleration The other AngularAcceleration type.
      /// @param base The base AngularAcceleration object.
      template<typename OtherAngularAcceleration>
      requires IsAngularAccelerationType<OtherAngularAcceleration>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr AngularAcceleration(const OtherAngularAcceleration& base)
         : Base(base.template GetVal<std::tuple<PrimaryAngle>, std::tuple<PrimaryTime, PrimaryTime>>()) {}
   };

   /// @brief Represents a AngularAcceleration.
   /// @details Defines operations and data storage for AngularAcceleration dimensions.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Angle1, typename Time1, typename Time2>
   requires IsAngularAccelerationUnits<Angle1, Time1, Time2>
   class AngularAcceleration<Angle1, Time1, Time2> : public BaseDimension<std::tuple<Angle1>, std::tuple<Time1, Time2>>
   {
   public:
      using Base = BaseDimension<std::tuple<Angle1>, std::tuple<Time1, Time2>>;
      using Base::Base;

      /// @brief Constructs a AngularAcceleration object with a value.
      /// @param val The value of the AngularAcceleration.
      explicit constexpr AngularAcceleration(PrecisionType val) : Base(val) {}

      /// @brief Constructs a AngularAcceleration object from a named unit.
      /// @tparam NamedAngularAcceleration The named unit type.
      /// @param base The base unit object.
      template<typename NamedAngularAcceleration>
      requires IsNamedAngularAccelerationUnit<NamedAngularAcceleration>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr AngularAcceleration(const NamedAngularAcceleration& base) : Base(base) {}

      /// @brief Deprecated function to get the value of AngularAcceleration.
      /// @details Prefer using the free function `getAngularAcceleration()` instead.
      /// @return The value of the AngularAcceleration.
      template<typename Angle1T, typename Time1T, typename Time2T>
      requires IsAngularAccelerationUnits<Angle1T, Time1T, Time2T>
      [[deprecated("Use the free function getAngularAcceleration() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetAngularAcceleration() const
      {
         return getAngularAcceleration<Angle1T, Time1T, Time2T>(*this);
      }

      /// @brief Deprecated function to get the value of AngularAcceleration.
      /// @details Prefer using the free function `getAngularAcceleration()` instead.
      /// @return The value of the AngularAcceleration.
      template<typename NamedAngularAcceleration>
      requires IsNamedAngularAccelerationUnit<NamedAngularAcceleration>
      [[deprecated("Use the free function getAngularAcceleration() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetAngularAcceleration() const
      {
         return getAngularAcceleration<NamedAngularAcceleration>(*this);
      }
   };

   /// @brief Represents a named AngularAcceleration class.
   /// @details Provides functionality for named AngularAcceleration units.
   /// @tparam NamedAngularAcceleration The named unit type.
   template<typename NamedAngularAcceleration>
   requires IsNamedAngularAccelerationUnit<NamedAngularAcceleration>
   class AngularAcceleration<NamedAngularAcceleration> : public BaseDimension<typename NamedAngularAcceleration::NumTuple, typename NamedAngularAcceleration::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedAngularAcceleration::NumTuple, typename NamedAngularAcceleration::DenTuple>;
      using Base::Base;

      /// @brief Constructs a AngularAcceleration object with a value.
      /// @param val The value of the AngularAcceleration.
      explicit constexpr AngularAcceleration(PrecisionType val) : Base(val) {}

      /// @brief Constructs a AngularAcceleration object from another AngularAcceleration object.
      /// @tparam OtherAngularAcceleration The other AngularAcceleration type.
      /// @param base The base AngularAcceleration object.
      template<typename OtherAngularAcceleration>
      requires IsAngularAccelerationType<OtherAngularAcceleration>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr AngularAcceleration(const OtherAngularAcceleration& base)
         : Base(base.template GetVal<typename NamedAngularAcceleration::NumTuple, typename NamedAngularAcceleration::DenTuple>()) {}

      /// @brief Deprecated function to get the value of AngularAcceleration.
      /// @details Prefer using the free function `getAngularAcceleration()` instead.
      /// @return The value of the AngularAcceleration.
      template<typename Angle1T, typename Time1T, typename Time2T>
      requires IsAngularAccelerationUnits<Angle1T, Time1T, Time2T>
      [[deprecated("Use the free function getAngularAcceleration() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetAngularAcceleration() const
      {
         return getAngularAcceleration<Angle1T, Time1T, Time2T>(*this);
      }

      /// @brief Deprecated function to get the value of AngularAcceleration.
      /// @details Prefer using the free function `getAngularAcceleration()` instead.
      /// @return The value of the AngularAcceleration.
      template<typename NamedAngularAccelerationUnit>
      requires IsNamedAngularAccelerationUnit<NamedAngularAccelerationUnit>
      [[deprecated("Use the free function getAngularAcceleration() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetAngularAcceleration() const
      {
         return getAngularAcceleration<NamedAngularAccelerationUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for AngularAcceleration.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Angle1, typename Time1, typename Time2>
   requires IsAngularAccelerationUnits<Angle1, Time1, Time2>
   AngularAcceleration(Angle1, Time1, Time2) -> AngularAcceleration<Angle1, Time1, Time2>;

   /// @brief Template deduction guide for AngularAcceleration.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename NamedAngularAcceleration>
   requires IsNamedAngularAccelerationUnit<NamedAngularAcceleration>
   AngularAcceleration(NamedAngularAcceleration) -> AngularAcceleration<NamedAngularAcceleration>;

   /// @brief Template deduction guide for AngularAcceleration.
   /// @tparam Angle1 Numerator Angle1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Angle1, typename Time1, typename Time2>
   requires IsAngularAccelerationUnits<Angle1, Time1, Time2>
   AngularAcceleration(BaseDimension<std::tuple<Angle1>, std::tuple<Time1, Time2>>) -> AngularAcceleration<Angle1, Time1, Time2>;

}

#endif // STATIC_DIMENSION_ANGULARACCELERATION_IMPL_H