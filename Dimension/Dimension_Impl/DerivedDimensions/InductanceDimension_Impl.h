#ifndef STATIC_DIMENSION_INDUCTANCE_IMPL_H
#define STATIC_DIMENSION_INDUCTANCE_IMPL_H

#include "../../ChargeDimension.h"
#include "../../LengthDimension.h"
#include "../../MassDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Inductance unit.
   /// @tparam NamedInductance The type to be checked as a named Inductance unit.
   template<typename NamedInductance>
   concept IsNamedInductanceUnit = requires {
      typename NamedInductance::NumTuple;
      typename NamedInductance::DenTuple;
   };

   /// @brief Concept for a Inductance dimension.
   /// @details Checks if the provided types satisfy the Inductance dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   template<typename Mass1, typename Length1, typename Length2,typename Charge1, typename Charge2>
   concept IsInductanceUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Charge1::Dim, ChargeType> &&
        std::is_same_v<typename Charge2::Dim, ChargeType>;

   /// @brief Concept for a Inductance type.
   /// @details Ensures that the type meets Inductance type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsInductanceType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 2 &&
   IsInductanceUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Inductance object.
   /// @details Provides access to the underlying value represented by a Inductance object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   /// @tparam InductanceType The type of the object being accessed.
   /// @param obj The Inductance object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1, typename Length2,typename Charge1, typename Charge2, typename InductanceType>
   requires IsInductanceUnits<Mass1, Length1, Length2,Charge1, Charge2> && IsInductanceType<InductanceType>
   constexpr PrecisionType getInductance(const InductanceType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1, Length2>, std::tuple<Charge1, Charge2>>();
   }

   /// @brief Retrieves the value of a named Inductance object.
   /// @details Provides access to the value represented by a named Inductance object.
   /// @tparam NamedInductance The named unit type.
   /// @tparam InductanceType The type of the object being accessed.
   /// @param obj The Inductance object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedInductance, typename InductanceType>
   requires IsNamedInductanceUnit<NamedInductance> && IsInductanceType<InductanceType>
   constexpr PrecisionType getInductance(const InductanceType& obj)
   {
      return obj.template GetVal<typename NamedInductance::NumTuple, typename NamedInductance::DenTuple>();
   }

   template<typename... Ts>
   class Inductance;

   /// @brief Represents a Inductance.
   /// @details Defines operations and data storage for Inductance dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   template<typename Mass1, typename Length1, typename Length2,typename Charge1, typename Charge2>
   requires IsInductanceUnits<Mass1, Length1, Length2,Charge1, Charge2>
   class Inductance<Mass1, Length1, Length2,Charge1, Charge2> : public BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Charge1, Charge2>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Charge1, Charge2>>;
      using Base::Base;

      /// @brief Constructs a Inductance object with a value.
      /// @param val The value of the Inductance.
      constexpr Inductance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Inductance object from a named unit.
      /// @tparam NamedInductance The named unit type.
      /// @param base The base unit object.
      template<typename NamedInductance>
      requires IsNamedInductanceUnit<NamedInductance>
      constexpr Inductance(const NamedInductance& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Inductance.
      /// @details Prefer using the free function `getInductance()` instead.
      /// @return The value of the Inductance.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Charge1T, typename Charge2T>
      requires IsInductanceUnits<Mass1T, Length1T, Length2T,Charge1T, Charge2T>
      [[deprecated("Use the free function getInductance() instead.")]]
      double GetInductance() const
      {
         return getInductance<Mass1T, Length1T, Length2T,Charge1T, Charge2T>(*this);
      }

      /// @brief Deprecated function to get the value of Inductance.
      /// @details Prefer using the free function `getInductance()` instead.
      /// @return The value of the Inductance.
      template<typename NamedInductance>
      requires IsNamedInductanceUnit<NamedInductance>
      [[deprecated("Use the free function getInductance() instead.")]]
      double GetInductance() const
      {
         return getInductance<NamedInductance>(*this);
      }
   };

   /// @brief Represents a named Inductance class.
   /// @details Provides functionality for named Inductance units.
   /// @tparam NamedInductance The named unit type.
   template<typename NamedInductance>
   requires IsNamedInductanceUnit<NamedInductance>
   class Inductance<NamedInductance> : public BaseDimension<typename NamedInductance::NumTuple, typename NamedInductance::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedInductance::NumTuple, typename NamedInductance::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Inductance object with a value.
      /// @param val The value of the Inductance.
      constexpr Inductance(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Inductance object from another Inductance object.
      /// @tparam OtherInductance The other Inductance type.
      /// @param base The base Inductance object.
      template<typename OtherInductance>
      requires IsInductanceType<OtherInductance>
      constexpr Inductance(const OtherInductance& base)
         : Base(base.template GetVal<typename NamedInductance::NumTuple, typename NamedInductance::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Inductance.
      /// @details Prefer using the free function `getInductance()` instead.
      /// @return The value of the Inductance.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Charge1T, typename Charge2T>
      requires IsInductanceUnits<Mass1T, Length1T, Length2T,Charge1T, Charge2T>
      [[deprecated("Use the free function getInductance() instead.")]]
      double GetInductance() const
      {
         return getInductance<Mass1T, Length1T, Length2T,Charge1T, Charge2T>(*this);
      }

      /// @brief Deprecated function to get the value of Inductance.
      /// @details Prefer using the free function `getInductance()` instead.
      /// @return The value of the Inductance.
      template<typename NamedInductanceUnit>
      requires IsNamedInductanceUnit<NamedInductanceUnit>
      [[deprecated("Use the free function getInductance() instead.")]]
      double GetInductance() const
      {
         return getInductance<NamedInductanceUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Inductance.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   template<typename Mass1, typename Length1, typename Length2,typename Charge1, typename Charge2>
   requires IsInductanceUnits<Mass1, Length1, Length2,Charge1, Charge2>
   Inductance(Mass1, Length1, Length2,Charge1, Charge2) -> Inductance<Mass1, Length1, Length2,Charge1, Charge2>;

   /// @brief Template deduction guide for Inductance.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   template<typename NamedInductance>
   requires IsNamedInductanceUnit<NamedInductance>
   Inductance(NamedInductance) -> Inductance<NamedInductance>;

   /// @brief Template deduction guide for Inductance.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam Charge2 Denominator Charge2 type
   template<typename Mass1, typename Length1, typename Length2,typename Charge1, typename Charge2>
   requires IsInductanceUnits<Mass1, Length1, Length2,Charge1, Charge2>
   Inductance(BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Charge1, Charge2>>) -> Inductance<Mass1, Length1, Length2,Charge1, Charge2>;

}

#endif // STATIC_DIMENSION_INDUCTANCE_IMPL_H