#ifndef STATIC_DIMENSION_FREQUENCY_IMPL_H
#define STATIC_DIMENSION_FREQUENCY_IMPL_H

#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Frequency unit.
   /// @tparam NamedFrequency The type to be checked as a named Frequency unit.
   template<typename NamedFrequency>
   concept IsNamedFrequencyUnit = requires {
      typename NamedFrequency::NumTuple;
      typename NamedFrequency::DenTuple;
   };

   /// @brief Concept for a Frequency dimension.
   /// @details Checks if the provided types satisfy the Frequency dimension requirements.
   /// @tparam Time1 Denominator Time1 type
   template<typename Time1>
   concept IsFrequencyUnits = 
      std::is_same_v<typename Time1::Dim, TimeType>;

   /// @brief Concept for a Frequency type.
   /// @details Ensures that the type meets Frequency type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsFrequencyType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 0 && std::tuple_size_v<typename T::DenTuple> == 1 &&
   IsFrequencyUnits<typename std::tuple_element_t<0, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Frequency object.
   /// @details Provides access to the underlying value represented by a Frequency object.
   /// @tparam Time1 Denominator Time1 type
   /// @tparam FrequencyType The type of the object being accessed.
   /// @param obj The Frequency object.
   /// @return The underlying value as `PrecisionType`
   template<typename Time1, typename FrequencyType>
   requires IsFrequencyUnits<Time1> && IsFrequencyType<FrequencyType>
   constexpr PrecisionType getFrequency(const FrequencyType& obj)
   {
      return obj.template GetVal<std::tuple<>, std::tuple<Time1>>();
   }

   /// @brief Retrieves the value of a named Frequency object.
   /// @details Provides access to the value represented by a named Frequency object.
   /// @tparam NamedFrequency The named unit type.
   /// @tparam FrequencyType The type of the object being accessed.
   /// @param obj The Frequency object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedFrequency, typename FrequencyType>
   requires IsNamedFrequencyUnit<NamedFrequency> && IsFrequencyType<FrequencyType>
   constexpr PrecisionType getFrequency(const FrequencyType& obj)
   {
      return obj.template GetVal<typename NamedFrequency::NumTuple, typename NamedFrequency::DenTuple>();
   }

   template<typename... Ts>
   class Frequency;

   /// @brief Represents a Frequency.
   /// @details Defines operations and data storage for Frequency dimensions.
   /// @tparam Time1 Denominator Time1 type
   template<typename Time1>
   requires IsFrequencyUnits<Time1>
   class Frequency<Time1> : public BaseDimension<std::tuple<>, std::tuple<Time1>>
   {
   public:
      using Base = BaseDimension<std::tuple<>, std::tuple<Time1>>;
      using Base::Base;

      /// @brief Constructs a Frequency object with a value.
      /// @param val The value of the Frequency.
      constexpr Frequency(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Frequency object from a named unit.
      /// @tparam NamedFrequency The named unit type.
      /// @param base The base unit object.
      template<typename NamedFrequency>
      requires IsNamedFrequencyUnit<NamedFrequency>
      constexpr Frequency(const NamedFrequency& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Frequency.
      /// @details Prefer using the free function `getFrequency()` instead.
      /// @return The value of the Frequency.
      template<typename Time1T>
      requires IsFrequencyUnits<Time1T>
      [[deprecated("Use the free function getFrequency() instead.")]]
      double GetFrequency() const
      {
         return getFrequency<Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of Frequency.
      /// @details Prefer using the free function `getFrequency()` instead.
      /// @return The value of the Frequency.
      template<typename NamedFrequency>
      requires IsNamedFrequencyUnit<NamedFrequency>
      [[deprecated("Use the free function getFrequency() instead.")]]
      double GetFrequency() const
      {
         return getFrequency<NamedFrequency>(*this);
      }
   };

   /// @brief Represents a named Frequency class.
   /// @details Provides functionality for named Frequency units.
   /// @tparam NamedFrequency The named unit type.
   template<typename NamedFrequency>
   requires IsNamedFrequencyUnit<NamedFrequency>
   class Frequency<NamedFrequency> : public BaseDimension<typename NamedFrequency::NumTuple, typename NamedFrequency::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedFrequency::NumTuple, typename NamedFrequency::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Frequency object with a value.
      /// @param val The value of the Frequency.
      constexpr Frequency(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Frequency object from another Frequency object.
      /// @tparam OtherFrequency The other Frequency type.
      /// @param base The base Frequency object.
      template<typename OtherFrequency>
      requires IsFrequencyType<OtherFrequency>
      constexpr Frequency(const OtherFrequency& base)
         : Base(base.template GetVal<typename NamedFrequency::NumTuple, typename NamedFrequency::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Frequency.
      /// @details Prefer using the free function `getFrequency()` instead.
      /// @return The value of the Frequency.
      template<typename Time1T>
      requires IsFrequencyUnits<Time1T>
      [[deprecated("Use the free function getFrequency() instead.")]]
      double GetFrequency() const
      {
         return getFrequency<Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of Frequency.
      /// @details Prefer using the free function `getFrequency()` instead.
      /// @return The value of the Frequency.
      template<typename NamedFrequencyUnit>
      requires IsNamedFrequencyUnit<NamedFrequencyUnit>
      [[deprecated("Use the free function getFrequency() instead.")]]
      double GetFrequency() const
      {
         return getFrequency<NamedFrequencyUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Frequency.
   /// @tparam Time1 Denominator Time1 type
   template<typename Time1>
   requires IsFrequencyUnits<Time1>
   Frequency(Time1) -> Frequency<Time1>;

   /// @brief Template deduction guide for Frequency.
   /// @tparam Time1 Denominator Time1 type
   template<typename NamedFrequency>
   requires IsNamedFrequencyUnit<NamedFrequency>
   Frequency(NamedFrequency) -> Frequency<NamedFrequency>;

   /// @brief Template deduction guide for Frequency.
   /// @tparam Time1 Denominator Time1 type
   template<typename Time1>
   requires IsFrequencyUnits<Time1>
   Frequency(BaseDimension<std::tuple<>, std::tuple<Time1>>) -> Frequency<Time1>;

}

#endif // STATIC_DIMENSION_FREQUENCY_IMPL_H