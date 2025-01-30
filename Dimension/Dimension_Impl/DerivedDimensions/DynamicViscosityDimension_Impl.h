#ifndef STATIC_DIMENSION_DYNAMICVISCOSITY_IMPL_H
#define STATIC_DIMENSION_DYNAMICVISCOSITY_IMPL_H

#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named DynamicViscosity unit.
   /// @tparam NamedDynamicViscosity The type to be checked as a named DynamicViscosity unit.
   template<typename NamedDynamicViscosity>
   concept IsNamedDynamicViscosityUnit = requires {
      typename NamedDynamicViscosity::NumTuple;
      typename NamedDynamicViscosity::DenTuple;
   };

   /// @brief Concept for a DynamicViscosity dimension.
   /// @details Checks if the provided types satisfy the DynamicViscosity dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Length1 Denominator Length1 type
   template<typename Mass1,typename Time1, typename Length1>
   concept IsDynamicViscosityUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Length1::Dim, LengthType>;

   /// @brief Concept for a DynamicViscosity type.
   /// @details Ensures that the type meets DynamicViscosity type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsDynamicViscosityType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 2 &&
   IsDynamicViscosityUnits<typename std::tuple_element_t<0, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>>;

   /// @brief Retrieves the value of a DynamicViscosity object.
   /// @details Provides access to the underlying value represented by a DynamicViscosity object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam DynamicViscosityType The type of the object being accessed.
   /// @param obj The DynamicViscosity object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1,typename Time1, typename Length1, typename DynamicViscosityType>
   requires IsDynamicViscosityUnits<Mass1,Time1, Length1> && IsDynamicViscosityType<DynamicViscosityType>
   constexpr PrecisionType getDynamicViscosity(const DynamicViscosityType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1>, std::tuple<Time1, Length1>>();
   }

   /// @brief Retrieves the value of a named DynamicViscosity object.
   /// @details Provides access to the value represented by a named DynamicViscosity object.
   /// @tparam NamedDynamicViscosity The named unit type.
   /// @tparam DynamicViscosityType The type of the object being accessed.
   /// @param obj The DynamicViscosity object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedDynamicViscosity, typename DynamicViscosityType>
   requires IsNamedDynamicViscosityUnit<NamedDynamicViscosity> && IsDynamicViscosityType<DynamicViscosityType>
   constexpr PrecisionType getDynamicViscosity(const DynamicViscosityType& obj)
   {
      return obj.template GetVal<typename NamedDynamicViscosity::NumTuple, typename NamedDynamicViscosity::DenTuple>();
   }

   template<typename... Ts>
   class DynamicViscosity;

   /// @brief Represents a default DynamicViscosity.
   /// @details This DynamicViscosity is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "DynamicViscosity" type, without regard for the underlying units.
   template<>
   class DynamicViscosity<> : public BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryTime, PrimaryLength>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryTime, PrimaryLength>>;
      using Base::Base;

      /// @brief Constructs a DynamicViscosity object with a value.
      /// @param val The value of the DynamicViscosity.
      explicit constexpr DynamicViscosity(PrecisionType val) : Base(val) {}

      /// @brief Constructs a DynamicViscosity object from another DynamicViscosity object.
      /// @tparam OtherDynamicViscosity The other DynamicViscosity type.
      /// @param base The base DynamicViscosity object.
      template<typename OtherDynamicViscosity>
      requires IsDynamicViscosityType<OtherDynamicViscosity>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr DynamicViscosity(const OtherDynamicViscosity& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass>, std::tuple<PrimaryTime, PrimaryLength>>()) {}
   };

   /// @brief Represents a DynamicViscosity.
   /// @details Defines operations and data storage for DynamicViscosity dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Length1 Denominator Length1 type
   template<typename Mass1,typename Time1, typename Length1>
   requires IsDynamicViscosityUnits<Mass1,Time1, Length1>
   class DynamicViscosity<Mass1,Time1, Length1> : public BaseDimension<std::tuple<Mass1>, std::tuple<Time1, Length1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1>, std::tuple<Time1, Length1>>;
      using Base::Base;

      /// @brief Constructs a DynamicViscosity object with a value.
      /// @param val The value of the DynamicViscosity.
      explicit constexpr DynamicViscosity(PrecisionType val) : Base(val) {}

      /// @brief Constructs a DynamicViscosity object from a named unit.
      /// @tparam NamedDynamicViscosity The named unit type.
      /// @param base The base unit object.
      template<typename NamedDynamicViscosity>
      requires IsNamedDynamicViscosityUnit<NamedDynamicViscosity>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr DynamicViscosity(const NamedDynamicViscosity& base) : Base(base) {}

      /// @brief Deprecated function to get the value of DynamicViscosity.
      /// @details Prefer using the free function `getDynamicViscosity()` instead.
      /// @return The value of the DynamicViscosity.
      template<typename Mass1T,typename Time1T, typename Length1T>
      requires IsDynamicViscosityUnits<Mass1T,Time1T, Length1T>
      [[deprecated("Use the free function getDynamicViscosity() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDynamicViscosity() const
      {
         return getDynamicViscosity<Mass1T,Time1T, Length1T>(*this);
      }

      /// @brief Deprecated function to get the value of DynamicViscosity.
      /// @details Prefer using the free function `getDynamicViscosity()` instead.
      /// @return The value of the DynamicViscosity.
      template<typename NamedDynamicViscosity>
      requires IsNamedDynamicViscosityUnit<NamedDynamicViscosity>
      [[deprecated("Use the free function getDynamicViscosity() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDynamicViscosity() const
      {
         return getDynamicViscosity<NamedDynamicViscosity>(*this);
      }
   };

   /// @brief Represents a named DynamicViscosity class.
   /// @details Provides functionality for named DynamicViscosity units.
   /// @tparam NamedDynamicViscosity The named unit type.
   template<typename NamedDynamicViscosity>
   requires IsNamedDynamicViscosityUnit<NamedDynamicViscosity>
   class DynamicViscosity<NamedDynamicViscosity> : public BaseDimension<typename NamedDynamicViscosity::NumTuple, typename NamedDynamicViscosity::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedDynamicViscosity::NumTuple, typename NamedDynamicViscosity::DenTuple>;
      using Base::Base;

      /// @brief Constructs a DynamicViscosity object with a value.
      /// @param val The value of the DynamicViscosity.
      explicit constexpr DynamicViscosity(PrecisionType val) : Base(val) {}

      /// @brief Constructs a DynamicViscosity object from another DynamicViscosity object.
      /// @tparam OtherDynamicViscosity The other DynamicViscosity type.
      /// @param base The base DynamicViscosity object.
      template<typename OtherDynamicViscosity>
      requires IsDynamicViscosityType<OtherDynamicViscosity>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr DynamicViscosity(const OtherDynamicViscosity& base)
         : Base(base.template GetVal<typename NamedDynamicViscosity::NumTuple, typename NamedDynamicViscosity::DenTuple>()) {}

      /// @brief Deprecated function to get the value of DynamicViscosity.
      /// @details Prefer using the free function `getDynamicViscosity()` instead.
      /// @return The value of the DynamicViscosity.
      template<typename Mass1T,typename Time1T, typename Length1T>
      requires IsDynamicViscosityUnits<Mass1T,Time1T, Length1T>
      [[deprecated("Use the free function getDynamicViscosity() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDynamicViscosity() const
      {
         return getDynamicViscosity<Mass1T,Time1T, Length1T>(*this);
      }

      /// @brief Deprecated function to get the value of DynamicViscosity.
      /// @details Prefer using the free function `getDynamicViscosity()` instead.
      /// @return The value of the DynamicViscosity.
      template<typename NamedDynamicViscosityUnit>
      requires IsNamedDynamicViscosityUnit<NamedDynamicViscosityUnit>
      [[deprecated("Use the free function getDynamicViscosity() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDynamicViscosity() const
      {
         return getDynamicViscosity<NamedDynamicViscosityUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for DynamicViscosity.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Length1 Denominator Length1 type
   template<typename Mass1,typename Time1, typename Length1>
   requires IsDynamicViscosityUnits<Mass1,Time1, Length1>
   DynamicViscosity(Mass1,Time1, Length1) -> DynamicViscosity<Mass1,Time1, Length1>;

   /// @brief Template deduction guide for DynamicViscosity.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Length1 Denominator Length1 type
   template<typename NamedDynamicViscosity>
   requires IsNamedDynamicViscosityUnit<NamedDynamicViscosity>
   DynamicViscosity(NamedDynamicViscosity) -> DynamicViscosity<NamedDynamicViscosity>;

   /// @brief Template deduction guide for DynamicViscosity.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Length1 Denominator Length1 type
   template<typename Mass1,typename Time1, typename Length1>
   requires IsDynamicViscosityUnits<Mass1,Time1, Length1>
   DynamicViscosity(BaseDimension<std::tuple<Mass1>, std::tuple<Time1, Length1>>) -> DynamicViscosity<Mass1,Time1, Length1>;

}

#endif // STATIC_DIMENSION_DYNAMICVISCOSITY_IMPL_H