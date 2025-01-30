#ifndef STATIC_DIMENSION_PRESSURE_IMPL_H
#define STATIC_DIMENSION_PRESSURE_IMPL_H

#include "../../TimeDimension.h"
#include "../../LengthDimension.h"
#include "../../MassDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Pressure unit.
   /// @tparam NamedPressure The type to be checked as a named Pressure unit.
   template<typename NamedPressure>
   concept IsNamedPressureUnit = requires {
      typename NamedPressure::NumTuple;
      typename NamedPressure::DenTuple;
   };

   /// @brief Concept for a Pressure dimension.
   /// @details Checks if the provided types satisfy the Pressure dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1,typename Length1, typename Time1, typename Time2>
   concept IsPressureUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Time2::Dim, TimeType>;

   /// @brief Concept for a Pressure type.
   /// @details Ensures that the type meets Pressure type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsPressureType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsPressureUnits<typename std::tuple_element_t<0, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Pressure object.
   /// @details Provides access to the underlying value represented by a Pressure object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam PressureType The type of the object being accessed.
   /// @param obj The Pressure object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1,typename Length1, typename Time1, typename Time2, typename PressureType>
   requires IsPressureUnits<Mass1,Length1, Time1, Time2> && IsPressureType<PressureType>
   constexpr PrecisionType getPressure(const PressureType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1>, std::tuple<Length1, Time1, Time2>>();
   }

   /// @brief Retrieves the value of a named Pressure object.
   /// @details Provides access to the value represented by a named Pressure object.
   /// @tparam NamedPressure The named unit type.
   /// @tparam PressureType The type of the object being accessed.
   /// @param obj The Pressure object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedPressure, typename PressureType>
   requires IsNamedPressureUnit<NamedPressure> && IsPressureType<PressureType>
   constexpr PrecisionType getPressure(const PressureType& obj)
   {
      return obj.template GetVal<typename NamedPressure::NumTuple, typename NamedPressure::DenTuple>();
   }

   template<typename... Ts>
   class Pressure;

   /// @brief Represents a default Pressure.
   /// @details This Pressure is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "Pressure" type, without regard for the underlying units.
   template<>
   class Pressure<> : public BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryLength, PrimaryTime, PrimaryTime>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryLength, PrimaryTime, PrimaryTime>>;
      using Base::Base;

      /// @brief Constructs a Pressure object with a value.
      /// @param val The value of the Pressure.
      explicit constexpr Pressure(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Pressure object from another Pressure object.
      /// @tparam OtherPressure The other Pressure type.
      /// @param base The base Pressure object.
      template<typename OtherPressure>
      requires IsPressureType<OtherPressure>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Pressure(const OtherPressure& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass>, std::tuple<PrimaryLength, PrimaryTime, PrimaryTime>>()) {}
   };

   /// @brief Represents a Pressure.
   /// @details Defines operations and data storage for Pressure dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1,typename Length1, typename Time1, typename Time2>
   requires IsPressureUnits<Mass1,Length1, Time1, Time2>
   class Pressure<Mass1,Length1, Time1, Time2> : public BaseDimension<std::tuple<Mass1>, std::tuple<Length1, Time1, Time2>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1>, std::tuple<Length1, Time1, Time2>>;
      using Base::Base;

      /// @brief Constructs a Pressure object with a value.
      /// @param val The value of the Pressure.
      explicit constexpr Pressure(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Pressure object from a named unit.
      /// @tparam NamedPressure The named unit type.
      /// @param base The base unit object.
      template<typename NamedPressure>
      requires IsNamedPressureUnit<NamedPressure>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Pressure(const NamedPressure& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Pressure.
      /// @details Prefer using the free function `getPressure()` instead.
      /// @return The value of the Pressure.
      template<typename Mass1T,typename Length1T, typename Time1T, typename Time2T>
      requires IsPressureUnits<Mass1T,Length1T, Time1T, Time2T>
      [[deprecated("Use the free function getPressure() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetPressure() const
      {
         return getPressure<Mass1T,Length1T, Time1T, Time2T>(*this);
      }

      /// @brief Deprecated function to get the value of Pressure.
      /// @details Prefer using the free function `getPressure()` instead.
      /// @return The value of the Pressure.
      template<typename NamedPressure>
      requires IsNamedPressureUnit<NamedPressure>
      [[deprecated("Use the free function getPressure() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetPressure() const
      {
         return getPressure<NamedPressure>(*this);
      }
   };

   /// @brief Represents a named Pressure class.
   /// @details Provides functionality for named Pressure units.
   /// @tparam NamedPressure The named unit type.
   template<typename NamedPressure>
   requires IsNamedPressureUnit<NamedPressure>
   class Pressure<NamedPressure> : public BaseDimension<typename NamedPressure::NumTuple, typename NamedPressure::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedPressure::NumTuple, typename NamedPressure::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Pressure object with a value.
      /// @param val The value of the Pressure.
      explicit constexpr Pressure(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Pressure object from another Pressure object.
      /// @tparam OtherPressure The other Pressure type.
      /// @param base The base Pressure object.
      template<typename OtherPressure>
      requires IsPressureType<OtherPressure>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Pressure(const OtherPressure& base)
         : Base(base.template GetVal<typename NamedPressure::NumTuple, typename NamedPressure::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Pressure.
      /// @details Prefer using the free function `getPressure()` instead.
      /// @return The value of the Pressure.
      template<typename Mass1T,typename Length1T, typename Time1T, typename Time2T>
      requires IsPressureUnits<Mass1T,Length1T, Time1T, Time2T>
      [[deprecated("Use the free function getPressure() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetPressure() const
      {
         return getPressure<Mass1T,Length1T, Time1T, Time2T>(*this);
      }

      /// @brief Deprecated function to get the value of Pressure.
      /// @details Prefer using the free function `getPressure()` instead.
      /// @return The value of the Pressure.
      template<typename NamedPressureUnit>
      requires IsNamedPressureUnit<NamedPressureUnit>
      [[deprecated("Use the free function getPressure() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetPressure() const
      {
         return getPressure<NamedPressureUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Pressure.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1,typename Length1, typename Time1, typename Time2>
   requires IsPressureUnits<Mass1,Length1, Time1, Time2>
   Pressure(Mass1,Length1, Time1, Time2) -> Pressure<Mass1,Length1, Time1, Time2>;

   /// @brief Template deduction guide for Pressure.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename NamedPressure>
   requires IsNamedPressureUnit<NamedPressure>
   Pressure(NamedPressure) -> Pressure<NamedPressure>;

   /// @brief Template deduction guide for Pressure.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1,typename Length1, typename Time1, typename Time2>
   requires IsPressureUnits<Mass1,Length1, Time1, Time2>
   Pressure(BaseDimension<std::tuple<Mass1>, std::tuple<Length1, Time1, Time2>>) -> Pressure<Mass1,Length1, Time1, Time2>;

}

#endif // STATIC_DIMENSION_PRESSURE_IMPL_H