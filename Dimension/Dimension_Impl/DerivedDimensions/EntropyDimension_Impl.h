#ifndef STATIC_DIMENSION_ENTROPY_IMPL_H
#define STATIC_DIMENSION_ENTROPY_IMPL_H

#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"
#include "../../TemperatureDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Entropy unit.
   /// @tparam NamedEntropy The type to be checked as a named Entropy unit.
   template<typename NamedEntropy>
   concept IsNamedEntropyUnit = requires {
      typename NamedEntropy::NumTuple;
      typename NamedEntropy::DenTuple;
   };

   /// @brief Concept for a Entropy dimension.
   /// @details Checks if the provided types satisfy the Entropy dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   template<typename Mass1, typename Length1, typename Length2, typename Time1, typename Time2, typename Temperature1>
   concept IsEntropyUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
      std::is_same_v<typename Length1::Dim, LengthType> &&
      std::is_same_v<typename Length2::Dim, LengthType> &&
      std::is_same_v<typename Time1::Dim, TimeType> &&
      std::is_same_v<typename Time2::Dim, TimeType> &&
      std::is_same_v<typename Temperature1::Dim, TemperatureType>;

   /// @brief Concept for a Entropy type.
   /// @details Ensures that the type meets Entropy type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsEntropyType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsEntropyUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>, typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Entropy object.
   /// @details Provides access to the underlying value represented by a Entropy object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   /// @tparam EntropyType The type of the object being accessed.
   /// @param obj The Entropy object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1, typename Length2, typename Time1, typename Time2, typename Temperature1, typename EntropyType>
   requires IsEntropyUnits<Mass1, Length1, Length2, Time1, Time2, Temperature1> && IsEntropyType<EntropyType>
   constexpr PrecisionType getEntropy(const EntropyType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Temperature1>>();
   }

   /// @brief Retrieves the value of a named Entropy object.
   /// @details Provides access to the value represented by a named Entropy object.
   /// @tparam NamedEntropy The named unit type.
   /// @tparam EntropyType The type of the object being accessed.
   /// @param obj The Entropy object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedEntropy, typename EntropyType>
   requires IsNamedEntropyUnit<NamedEntropy> && IsEntropyType<EntropyType>
   constexpr PrecisionType getEntropy(const EntropyType& obj)
   {
      return obj.template GetVal<typename NamedEntropy::NumTuple, typename NamedEntropy::DenTuple>();
   }

   template<typename... Ts>
   class Entropy;

   /// @brief Represents a default Entropy.
   /// @details This Entropy is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "Entropy" type, without regard for the underlying units.
   template<>
   class Entropy<> : public BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryTemperature>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryTemperature>>;
      using Base::Base;

      /// @brief Constructs a Entropy object with a value.
      /// @param val The value of the Entropy.
      explicit constexpr Entropy(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Entropy object from another Entropy object.
      /// @tparam OtherEntropy The other Entropy type.
      /// @param base The base Entropy object.
      template<typename OtherEntropy>
      requires IsEntropyType<OtherEntropy>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Entropy(const OtherEntropy& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryTemperature>>()) {}
   };

   /// @brief Represents a Entropy.
   /// @details Defines operations and data storage for Entropy dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   template<typename Mass1, typename Length1, typename Length2, typename Time1, typename Time2, typename Temperature1>
   requires IsEntropyUnits<Mass1, Length1, Length2, Time1, Time2, Temperature1>
   class Entropy<Mass1, Length1, Length2, Time1, Time2, Temperature1> : public BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Temperature1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Temperature1>>;
      using Base::Base;

      /// @brief Constructs a Entropy object with a value.
      /// @param val The value of the Entropy.
      explicit constexpr Entropy(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Entropy object from a named unit.
      /// @tparam NamedEntropy The named unit type.
      /// @param base The base unit object.
      template<typename NamedEntropy>
      requires IsNamedEntropyUnit<NamedEntropy>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Entropy(const NamedEntropy& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Entropy.
      /// @details Prefer using the free function `getEntropy()` instead.
      /// @return The value of the Entropy.
      template<typename Mass1T, typename Length1T, typename Length2T, typename Time1T, typename Time2T, typename Temperature1T>
      requires IsEntropyUnits<Mass1T, Length1T, Length2T, Time1T, Time2T, Temperature1T>
      [[deprecated("Use the free function getEntropy() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetEntropy() const
      {
         return getEntropy<Mass1T, Length1T, Length2T, Time1T, Time2T, Temperature1T>(*this);
      }

      /// @brief Deprecated function to get the value of Entropy.
      /// @details Prefer using the free function `getEntropy()` instead.
      /// @return The value of the Entropy.
      template<typename NamedEntropy>
      requires IsNamedEntropyUnit<NamedEntropy>
      [[deprecated("Use the free function getEntropy() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetEntropy() const
      {
         return getEntropy<NamedEntropy>(*this);
      }
   };

   /// @brief Represents a named Entropy class.
   /// @details Provides functionality for named Entropy units.
   /// @tparam NamedEntropy The named unit type.
   template<typename NamedEntropy>
   requires IsNamedEntropyUnit<NamedEntropy>
   class Entropy<NamedEntropy> : public BaseDimension<typename NamedEntropy::NumTuple, typename NamedEntropy::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedEntropy::NumTuple, typename NamedEntropy::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Entropy object with a value.
      /// @param val The value of the Entropy.
      explicit constexpr Entropy(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Entropy object from another Entropy object.
      /// @tparam OtherEntropy The other Entropy type.
      /// @param base The base Entropy object.
      template<typename OtherEntropy>
      requires IsEntropyType<OtherEntropy>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Entropy(const OtherEntropy& base)
         : Base(base.template GetVal<typename NamedEntropy::NumTuple, typename NamedEntropy::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Entropy.
      /// @details Prefer using the free function `getEntropy()` instead.
      /// @return The value of the Entropy.
      template<typename Mass1T, typename Length1T, typename Length2T, typename Time1T, typename Time2T, typename Temperature1T>
      requires IsEntropyUnits<Mass1T, Length1T, Length2T, Time1T, Time2T, Temperature1T>
      [[deprecated("Use the free function getEntropy() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetEntropy() const
      {
         return getEntropy<Mass1T, Length1T, Length2T, Time1T, Time2T, Temperature1T>(*this);
      }

      /// @brief Deprecated function to get the value of Entropy.
      /// @details Prefer using the free function `getEntropy()` instead.
      /// @return The value of the Entropy.
      template<typename NamedEntropyUnit>
      requires IsNamedEntropyUnit<NamedEntropyUnit>
      [[deprecated("Use the free function getEntropy() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetEntropy() const
      {
         return getEntropy<NamedEntropyUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Entropy.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   template<typename Mass1, typename Length1, typename Length2, typename Time1, typename Time2, typename Temperature1>
   requires IsEntropyUnits<Mass1, Length1, Length2, Time1, Time2, Temperature1>
   Entropy(Mass1, Length1, Length2, Time1, Time2, Temperature1) -> Entropy<Mass1, Length1, Length2, Time1, Time2, Temperature1>;

   /// @brief Template deduction guide for Entropy.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   template<typename NamedEntropy>
   requires IsNamedEntropyUnit<NamedEntropy>
   Entropy(NamedEntropy) -> Entropy<NamedEntropy>;

   /// @brief Template deduction guide for Entropy.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Temperature1 Denominator Temperature1 type
   template<typename Mass1, typename Length1, typename Length2, typename Time1, typename Time2, typename Temperature1>
   requires IsEntropyUnits<Mass1, Length1, Length2, Time1, Time2, Temperature1>
   Entropy(BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Temperature1>>) -> Entropy<Mass1, Length1, Length2, Time1, Time2, Temperature1>;

}

#endif // STATIC_DIMENSION_ENTROPY_IMPL_H