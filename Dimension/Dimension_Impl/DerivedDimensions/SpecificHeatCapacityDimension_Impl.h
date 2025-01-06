#ifndef STATIC_DIMENSION_SPECIFICHEATCAPACITY_IMPL_H
#define STATIC_DIMENSION_SPECIFICHEATCAPACITY_IMPL_H

#include "../../TemperatureDimension.h"
#include "../../TimeDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept for a named SpecificHeatCapacity unit.
   /// @tparam NamedSpecificHeatCapacity The type to be checked as a named SpecificHeatCapacity unit.
   template<typename NamedSpecificHeatCapacity>
   concept IsNamedSpecificHeatCapacityUnit = requires {
      typename NamedSpecificHeatCapacity::NumTuple;
      typename NamedSpecificHeatCapacity::DenTuple;
   };

   /// @brief Concept for a SpecificHeatCapacity dimension.
   /// @details Checks if the provided types satisfy the SpecificHeatCapacity dimension requirements.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   template<typename Length1, typename Length2,typename Time1, typename Time2, typename Temperature1>
   concept IsSpecificHeatCapacityUnits = 
      std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Time2::Dim, TimeType> &&
        std::is_same_v<typename Temperature1::Dim, TemperatureType>;

   /// @brief Concept for a SpecificHeatCapacity type.
   /// @details Ensures that the type meets SpecificHeatCapacity type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsSpecificHeatCapacityType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 2 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsSpecificHeatCapacityUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a SpecificHeatCapacity object.
   /// @details Provides access to the underlying value represented by a SpecificHeatCapacity object.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   /// @tparam SpecificHeatCapacityType The type of the object being accessed.
   /// @param obj The SpecificHeatCapacity object.
   /// @return The underlying value as `PrecisionType`
   template<typename Length1, typename Length2,typename Time1, typename Time2, typename Temperature1, typename SpecificHeatCapacityType>
   requires IsSpecificHeatCapacityUnits<Length1, Length2,Time1, Time2, Temperature1> && IsSpecificHeatCapacityType<SpecificHeatCapacityType>
   constexpr PrecisionType getSpecificHeatCapacity(const SpecificHeatCapacityType& obj)
   {
      return obj.template GetVal<std::tuple<Length1, Length2>, std::tuple<Time1, Time2, Temperature1>>();
   }

   /// @brief Retrieves the value of a named SpecificHeatCapacity object.
   /// @details Provides access to the value represented by a named SpecificHeatCapacity object.
   /// @tparam NamedSpecificHeatCapacity The named unit type.
   /// @tparam SpecificHeatCapacityType The type of the object being accessed.
   /// @param obj The SpecificHeatCapacity object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedSpecificHeatCapacity, typename SpecificHeatCapacityType>
   requires IsNamedSpecificHeatCapacityUnit<NamedSpecificHeatCapacity> && IsSpecificHeatCapacityType<SpecificHeatCapacityType>
   constexpr PrecisionType getSpecificHeatCapacity(const SpecificHeatCapacityType& obj)
   {
      return obj.template GetVal<typename NamedSpecificHeatCapacity::NumTuple, typename NamedSpecificHeatCapacity::DenTuple>();
   }

   template<typename... Ts>
   class SpecificHeatCapacity;

   /// @brief Represents a SpecificHeatCapacity.
   /// @details Defines operations and data storage for SpecificHeatCapacity dimensions.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   template<typename Length1, typename Length2,typename Time1, typename Time2, typename Temperature1>
   requires IsSpecificHeatCapacityUnits<Length1, Length2,Time1, Time2, Temperature1>
   class SpecificHeatCapacity<Length1, Length2,Time1, Time2, Temperature1> : public BaseDimension<std::tuple<Length1, Length2>, std::tuple<Time1, Time2, Temperature1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Length1, Length2>, std::tuple<Time1, Time2, Temperature1>>;
      using Base::Base;

      /// @brief Constructs a SpecificHeatCapacity object with a value.
      /// @param val The value of the SpecificHeatCapacity.
      constexpr SpecificHeatCapacity(PrecisionType val) : Base(val) {}

      /// @brief Constructs a SpecificHeatCapacity object from a named unit.
      /// @tparam NamedSpecificHeatCapacity The named unit type.
      /// @param base The base unit object.
      template<typename NamedSpecificHeatCapacity>
      requires IsNamedSpecificHeatCapacityUnit<NamedSpecificHeatCapacity>
      constexpr SpecificHeatCapacity(const NamedSpecificHeatCapacity& base) : Base(base) {}

      /// @brief Deprecated function to get the value of SpecificHeatCapacity.
      /// @details Prefer using the free function `getSpecificHeatCapacity()` instead.
      /// @return The value of the SpecificHeatCapacity.
      template<typename Length1T, typename Length2T,typename Time1T, typename Time2T, typename Temperature1T>
      requires IsSpecificHeatCapacityUnits<Length1T, Length2T,Time1T, Time2T, Temperature1T>
      [[deprecated("Use the free function getSpecificHeatCapacity() instead.")]]
      double GetSpecificHeatCapacity() const
      {
         return getSpecificHeatCapacity<Length1T, Length2T,Time1T, Time2T, Temperature1T>(*this);
      }

      /// @brief Deprecated function to get the value of SpecificHeatCapacity.
      /// @details Prefer using the free function `getSpecificHeatCapacity()` instead.
      /// @return The value of the SpecificHeatCapacity.
      template<typename NamedSpecificHeatCapacity>
      requires IsNamedSpecificHeatCapacityUnit<NamedSpecificHeatCapacity>
      [[deprecated("Use the free function getSpecificHeatCapacity() instead.")]]
      double GetSpecificHeatCapacity() const
      {
         return getSpecificHeatCapacity<NamedSpecificHeatCapacity>(*this);
      }
   };

   /// @brief Represents a named SpecificHeatCapacity class.
   /// @details Provides functionality for named SpecificHeatCapacity units.
   /// @tparam NamedSpecificHeatCapacity The named unit type.
   template<typename NamedSpecificHeatCapacity>
   requires IsNamedSpecificHeatCapacityUnit<NamedSpecificHeatCapacity>
   class SpecificHeatCapacity<NamedSpecificHeatCapacity> : public BaseDimension<typename NamedSpecificHeatCapacity::NumTuple, typename NamedSpecificHeatCapacity::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedSpecificHeatCapacity::NumTuple, typename NamedSpecificHeatCapacity::DenTuple>;
      using Base::Base;

      /// @brief Constructs a SpecificHeatCapacity object with a value.
      /// @param val The value of the SpecificHeatCapacity.
      constexpr SpecificHeatCapacity(PrecisionType val) : Base(val) {}

      /// @brief Constructs a SpecificHeatCapacity object from another SpecificHeatCapacity object.
      /// @tparam OtherSpecificHeatCapacity The other SpecificHeatCapacity type.
      /// @param base The base SpecificHeatCapacity object.
      template<typename OtherSpecificHeatCapacity>
      requires IsSpecificHeatCapacityType<OtherSpecificHeatCapacity>
      constexpr SpecificHeatCapacity(const OtherSpecificHeatCapacity& base)
         : Base(base.template GetVal<typename NamedSpecificHeatCapacity::NumTuple, typename NamedSpecificHeatCapacity::DenTuple>()) {}

      /// @brief Deprecated function to get the value of SpecificHeatCapacity.
      /// @details Prefer using the free function `getSpecificHeatCapacity()` instead.
      /// @return The value of the SpecificHeatCapacity.
      template<typename Length1T, typename Length2T,typename Time1T, typename Time2T, typename Temperature1T>
      requires IsSpecificHeatCapacityUnits<Length1T, Length2T,Time1T, Time2T, Temperature1T>
      [[deprecated("Use the free function getSpecificHeatCapacity() instead.")]]
      double GetSpecificHeatCapacity() const
      {
         return getSpecificHeatCapacity<Length1T, Length2T,Time1T, Time2T, Temperature1T>(*this);
      }

      /// @brief Deprecated function to get the value of SpecificHeatCapacity.
      /// @details Prefer using the free function `getSpecificHeatCapacity()` instead.
      /// @return The value of the SpecificHeatCapacity.
      template<typename NamedSpecificHeatCapacityUnit>
      requires IsNamedSpecificHeatCapacityUnit<NamedSpecificHeatCapacityUnit>
      [[deprecated("Use the free function getSpecificHeatCapacity() instead.")]]
      double GetSpecificHeatCapacity() const
      {
         return getSpecificHeatCapacity<NamedSpecificHeatCapacityUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for SpecificHeatCapacity.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   template<typename Length1, typename Length2,typename Time1, typename Time2, typename Temperature1>
   requires IsSpecificHeatCapacityUnits<Length1, Length2,Time1, Time2, Temperature1>
   SpecificHeatCapacity(Length1, Length2,Time1, Time2, Temperature1) -> SpecificHeatCapacity<Length1, Length2,Time1, Time2, Temperature1>;

   /// @brief Template deduction guide for SpecificHeatCapacity.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   template<typename NamedSpecificHeatCapacity>
   requires IsNamedSpecificHeatCapacityUnit<NamedSpecificHeatCapacity>
   SpecificHeatCapacity(NamedSpecificHeatCapacity) -> SpecificHeatCapacity<NamedSpecificHeatCapacity>;

   /// @brief Template deduction guide for SpecificHeatCapacity.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   template<typename Length1, typename Length2,typename Time1, typename Time2, typename Temperature1>
   requires IsSpecificHeatCapacityUnits<Length1, Length2,Time1, Time2, Temperature1>
   SpecificHeatCapacity(BaseDimension<std::tuple<Length1, Length2>, std::tuple<Time1, Time2, Temperature1>>) -> SpecificHeatCapacity<Length1, Length2,Time1, Time2, Temperature1>;

}

#endif // STATIC_DIMENSION_SPECIFICHEATCAPACITY_IMPL_H