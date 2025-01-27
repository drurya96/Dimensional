#ifndef STATIC_DIMENSION_VOLUMETRICFLOWRATE_IMPL_H
#define STATIC_DIMENSION_VOLUMETRICFLOWRATE_IMPL_H

#include "../../TimeDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept for a named VolumetricFlowRate unit.
   /// @tparam NamedVolumetricFlowRate The type to be checked as a named VolumetricFlowRate unit.
   template<typename NamedVolumetricFlowRate>
   concept IsNamedVolumetricFlowRateUnit = requires {
      typename NamedVolumetricFlowRate::NumTuple;
      typename NamedVolumetricFlowRate::DenTuple;
   };

   /// @brief Concept for a VolumetricFlowRate dimension.
   /// @details Checks if the provided types satisfy the VolumetricFlowRate dimension requirements.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Length2, typename Length3,typename Time1>
   concept IsVolumetricFlowRateUnits = 
      std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Length3::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType>;

   /// @brief Concept for a VolumetricFlowRate type.
   /// @details Ensures that the type meets VolumetricFlowRate type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsVolumetricFlowRateType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 1 &&
   IsVolumetricFlowRateUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>>;

   /// @brief Retrieves the value of a VolumetricFlowRate object.
   /// @details Provides access to the underlying value represented by a VolumetricFlowRate object.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam VolumetricFlowRateType The type of the object being accessed.
   /// @param obj The VolumetricFlowRate object.
   /// @return The underlying value as `PrecisionType`
   template<typename Length1, typename Length2, typename Length3,typename Time1, typename VolumetricFlowRateType>
   requires IsVolumetricFlowRateUnits<Length1, Length2, Length3,Time1> && IsVolumetricFlowRateType<VolumetricFlowRateType>
   constexpr PrecisionType getVolumetricFlowRate(const VolumetricFlowRateType& obj)
   {
      return obj.template GetVal<std::tuple<Length1, Length2, Length3>, std::tuple<Time1>>();
   }

   /// @brief Retrieves the value of a named VolumetricFlowRate object.
   /// @details Provides access to the value represented by a named VolumetricFlowRate object.
   /// @tparam NamedVolumetricFlowRate The named unit type.
   /// @tparam VolumetricFlowRateType The type of the object being accessed.
   /// @param obj The VolumetricFlowRate object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedVolumetricFlowRate, typename VolumetricFlowRateType>
   requires IsNamedVolumetricFlowRateUnit<NamedVolumetricFlowRate> && IsVolumetricFlowRateType<VolumetricFlowRateType>
   constexpr PrecisionType getVolumetricFlowRate(const VolumetricFlowRateType& obj)
   {
      return obj.template GetVal<typename NamedVolumetricFlowRate::NumTuple, typename NamedVolumetricFlowRate::DenTuple>();
   }

   template<typename... Ts>
   class VolumetricFlowRate;

   /// @brief Represents a VolumetricFlowRate.
   /// @details Defines operations and data storage for VolumetricFlowRate dimensions.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Length2, typename Length3,typename Time1>
   requires IsVolumetricFlowRateUnits<Length1, Length2, Length3,Time1>
   class VolumetricFlowRate<Length1, Length2, Length3,Time1> : public BaseDimension<std::tuple<Length1, Length2, Length3>, std::tuple<Time1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Length1, Length2, Length3>, std::tuple<Time1>>;
      using Base::Base;

      /// @brief Constructs a VolumetricFlowRate object with a value.
      /// @param val The value of the VolumetricFlowRate.
      explicit constexpr VolumetricFlowRate(PrecisionType val) : Base(val) {}

      /// @brief Constructs a VolumetricFlowRate object from a named unit.
      /// @tparam NamedVolumetricFlowRate The named unit type.
      /// @param base The base unit object.
      template<typename NamedVolumetricFlowRate>
      requires IsNamedVolumetricFlowRateUnit<NamedVolumetricFlowRate>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr VolumetricFlowRate(const NamedVolumetricFlowRate& base) : Base(base) {}

      /// @brief Deprecated function to get the value of VolumetricFlowRate.
      /// @details Prefer using the free function `getVolumetricFlowRate()` instead.
      /// @return The value of the VolumetricFlowRate.
      template<typename Length1T, typename Length2T, typename Length3T,typename Time1T>
      requires IsVolumetricFlowRateUnits<Length1T, Length2T, Length3T,Time1T>
      [[deprecated("Use the free function getVolumetricFlowRate() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetVolumetricFlowRate() const
      {
         return getVolumetricFlowRate<Length1T, Length2T, Length3T,Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of VolumetricFlowRate.
      /// @details Prefer using the free function `getVolumetricFlowRate()` instead.
      /// @return The value of the VolumetricFlowRate.
      template<typename NamedVolumetricFlowRate>
      requires IsNamedVolumetricFlowRateUnit<NamedVolumetricFlowRate>
      [[deprecated("Use the free function getVolumetricFlowRate() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetVolumetricFlowRate() const
      {
         return getVolumetricFlowRate<NamedVolumetricFlowRate>(*this);
      }
   };

   /// @brief Represents a named VolumetricFlowRate class.
   /// @details Provides functionality for named VolumetricFlowRate units.
   /// @tparam NamedVolumetricFlowRate The named unit type.
   template<typename NamedVolumetricFlowRate>
   requires IsNamedVolumetricFlowRateUnit<NamedVolumetricFlowRate>
   class VolumetricFlowRate<NamedVolumetricFlowRate> : public BaseDimension<typename NamedVolumetricFlowRate::NumTuple, typename NamedVolumetricFlowRate::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedVolumetricFlowRate::NumTuple, typename NamedVolumetricFlowRate::DenTuple>;
      using Base::Base;

      /// @brief Constructs a VolumetricFlowRate object with a value.
      /// @param val The value of the VolumetricFlowRate.
      explicit constexpr VolumetricFlowRate(PrecisionType val) : Base(val) {}

      /// @brief Constructs a VolumetricFlowRate object from another VolumetricFlowRate object.
      /// @tparam OtherVolumetricFlowRate The other VolumetricFlowRate type.
      /// @param base The base VolumetricFlowRate object.
      template<typename OtherVolumetricFlowRate>
      requires IsVolumetricFlowRateType<OtherVolumetricFlowRate>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr VolumetricFlowRate(const OtherVolumetricFlowRate& base)
         : Base(base.template GetVal<typename NamedVolumetricFlowRate::NumTuple, typename NamedVolumetricFlowRate::DenTuple>()) {}

      /// @brief Deprecated function to get the value of VolumetricFlowRate.
      /// @details Prefer using the free function `getVolumetricFlowRate()` instead.
      /// @return The value of the VolumetricFlowRate.
      template<typename Length1T, typename Length2T, typename Length3T,typename Time1T>
      requires IsVolumetricFlowRateUnits<Length1T, Length2T, Length3T,Time1T>
      [[deprecated("Use the free function getVolumetricFlowRate() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetVolumetricFlowRate() const
      {
         return getVolumetricFlowRate<Length1T, Length2T, Length3T,Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of VolumetricFlowRate.
      /// @details Prefer using the free function `getVolumetricFlowRate()` instead.
      /// @return The value of the VolumetricFlowRate.
      template<typename NamedVolumetricFlowRateUnit>
      requires IsNamedVolumetricFlowRateUnit<NamedVolumetricFlowRateUnit>
      [[deprecated("Use the free function getVolumetricFlowRate() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetVolumetricFlowRate() const
      {
         return getVolumetricFlowRate<NamedVolumetricFlowRateUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for VolumetricFlowRate.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Length2, typename Length3,typename Time1>
   requires IsVolumetricFlowRateUnits<Length1, Length2, Length3,Time1>
   VolumetricFlowRate(Length1, Length2, Length3,Time1) -> VolumetricFlowRate<Length1, Length2, Length3,Time1>;

   /// @brief Template deduction guide for VolumetricFlowRate.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Time1 Denominator Time1 type
   template<typename NamedVolumetricFlowRate>
   requires IsNamedVolumetricFlowRateUnit<NamedVolumetricFlowRate>
   VolumetricFlowRate(NamedVolumetricFlowRate) -> VolumetricFlowRate<NamedVolumetricFlowRate>;

   /// @brief Template deduction guide for VolumetricFlowRate.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Length2, typename Length3,typename Time1>
   requires IsVolumetricFlowRateUnits<Length1, Length2, Length3,Time1>
   VolumetricFlowRate(BaseDimension<std::tuple<Length1, Length2, Length3>, std::tuple<Time1>>) -> VolumetricFlowRate<Length1, Length2, Length3,Time1>;

}

#endif // STATIC_DIMENSION_VOLUMETRICFLOWRATE_IMPL_H