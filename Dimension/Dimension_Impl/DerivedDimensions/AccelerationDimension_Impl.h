#ifndef STATIC_DIMENSION_ACCELERATION_IMPL_H
#define STATIC_DIMENSION_ACCELERATION_IMPL_H

#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Acceleration unit.
   /// @tparam NamedAcceleration The type to be checked as a named Acceleration unit.
   template<typename NamedAcceleration>
   concept IsNamedAccelerationUnit = requires {
      typename NamedAcceleration::NumTuple;
      typename NamedAcceleration::DenTuple;
   };

   /// @brief Concept for a Acceleration dimension.
   /// @details Checks if the provided types satisfy the Acceleration dimension requirements.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Length1,typename Time1, typename Time2>
   concept IsAccelerationUnits = 
      std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Time2::Dim, TimeType>;

   /// @brief Concept for a Acceleration type.
   /// @details Ensures that the type meets Acceleration type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsAccelerationType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 2 &&
   IsAccelerationUnits<typename std::tuple_element_t<0, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Acceleration object.
   /// @details Provides access to the underlying value represented by a Acceleration object.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam AccelerationType The type of the object being accessed.
   /// @param obj The Acceleration object.
   /// @return The underlying value as `PrecisionType`
   template<typename Length1,typename Time1, typename Time2, typename AccelerationType>
   requires IsAccelerationUnits<Length1,Time1, Time2> && IsAccelerationType<AccelerationType>
   constexpr PrecisionType getAcceleration(const AccelerationType& obj)
   {
      return obj.template GetVal<std::tuple<Length1>, std::tuple<Time1, Time2>>();
   }

   /// @brief Retrieves the value of a named Acceleration object.
   /// @details Provides access to the value represented by a named Acceleration object.
   /// @tparam NamedAcceleration The named unit type.
   /// @tparam AccelerationType The type of the object being accessed.
   /// @param obj The Acceleration object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedAcceleration, typename AccelerationType>
   requires IsNamedAccelerationUnit<NamedAcceleration> && IsAccelerationType<AccelerationType>
   constexpr PrecisionType getAcceleration(const AccelerationType& obj)
   {
      return obj.template GetVal<typename NamedAcceleration::NumTuple, typename NamedAcceleration::DenTuple>();
   }

   template<typename... Ts>
   class Acceleration;

   /// @brief Represents a Acceleration.
   /// @details Defines operations and data storage for Acceleration dimensions.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Length1,typename Time1, typename Time2>
   requires IsAccelerationUnits<Length1,Time1, Time2>
   class Acceleration<Length1,Time1, Time2> : public BaseDimension<std::tuple<Length1>, std::tuple<Time1, Time2>>
   {
   public:
      using Base = BaseDimension<std::tuple<Length1>, std::tuple<Time1, Time2>>;
      using Base::Base;

      /// @brief Constructs a Acceleration object with a value.
      /// @param val The value of the Acceleration.
      constexpr Acceleration(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Acceleration object from a named unit.
      /// @tparam NamedAcceleration The named unit type.
      /// @param base The base unit object.
      template<typename NamedAcceleration>
      requires IsNamedAccelerationUnit<NamedAcceleration>
      constexpr Acceleration(const NamedAcceleration& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Acceleration.
      /// @details Prefer using the free function `getAcceleration()` instead.
      /// @return The value of the Acceleration.
      template<typename Length1T,typename Time1T, typename Time2T>
      requires IsAccelerationUnits<Length1T,Time1T, Time2T>
      [[deprecated("Use the free function getAcceleration() instead.")]]
      double GetAcceleration() const
      {
         return getAcceleration<Length1T,Time1T, Time2T>(*this);
      }

      /// @brief Deprecated function to get the value of Acceleration.
      /// @details Prefer using the free function `getAcceleration()` instead.
      /// @return The value of the Acceleration.
      template<typename NamedAcceleration>
      requires IsNamedAccelerationUnit<NamedAcceleration>
      [[deprecated("Use the free function getAcceleration() instead.")]]
      double GetAcceleration() const
      {
         return getAcceleration<NamedAcceleration>(*this);
      }
   };

   /// @brief Represents a named Acceleration class.
   /// @details Provides functionality for named Acceleration units.
   /// @tparam NamedAcceleration The named unit type.
   template<typename NamedAcceleration>
   requires IsNamedAccelerationUnit<NamedAcceleration>
   class Acceleration<NamedAcceleration> : public BaseDimension<typename NamedAcceleration::NumTuple, typename NamedAcceleration::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedAcceleration::NumTuple, typename NamedAcceleration::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Acceleration object with a value.
      /// @param val The value of the Acceleration.
      constexpr Acceleration(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Acceleration object from another Acceleration object.
      /// @tparam OtherAcceleration The other Acceleration type.
      /// @param base The base Acceleration object.
      template<typename OtherAcceleration>
      requires IsAccelerationType<OtherAcceleration>
      constexpr Acceleration(const OtherAcceleration& base)
         : Base(base.template GetVal<typename NamedAcceleration::NumTuple, typename NamedAcceleration::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Acceleration.
      /// @details Prefer using the free function `getAcceleration()` instead.
      /// @return The value of the Acceleration.
      template<typename Length1T,typename Time1T, typename Time2T>
      requires IsAccelerationUnits<Length1T,Time1T, Time2T>
      [[deprecated("Use the free function getAcceleration() instead.")]]
      double GetAcceleration() const
      {
         return getAcceleration<Length1T,Time1T, Time2T>(*this);
      }

      /// @brief Deprecated function to get the value of Acceleration.
      /// @details Prefer using the free function `getAcceleration()` instead.
      /// @return The value of the Acceleration.
      template<typename NamedAccelerationUnit>
      requires IsNamedAccelerationUnit<NamedAccelerationUnit>
      [[deprecated("Use the free function getAcceleration() instead.")]]
      double GetAcceleration() const
      {
         return getAcceleration<NamedAccelerationUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Acceleration.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Length1,typename Time1, typename Time2>
   requires IsAccelerationUnits<Length1,Time1, Time2>
   Acceleration(Length1,Time1, Time2) -> Acceleration<Length1,Time1, Time2>;

   /// @brief Template deduction guide for Acceleration.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename NamedAcceleration>
   requires IsNamedAccelerationUnit<NamedAcceleration>
   Acceleration(NamedAcceleration) -> Acceleration<NamedAcceleration>;

   /// @brief Template deduction guide for Acceleration.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Length1,typename Time1, typename Time2>
   requires IsAccelerationUnits<Length1,Time1, Time2>
   Acceleration(BaseDimension<std::tuple<Length1>, std::tuple<Time1, Time2>>) -> Acceleration<Length1,Time1, Time2>;

}

#endif // STATIC_DIMENSION_ACCELERATION_IMPL_H