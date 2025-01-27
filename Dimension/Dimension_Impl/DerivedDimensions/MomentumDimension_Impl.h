#ifndef STATIC_DIMENSION_MOMENTUM_IMPL_H
#define STATIC_DIMENSION_MOMENTUM_IMPL_H

#include "../../LengthDimension.h"
#include "../../MassDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Momentum unit.
   /// @tparam NamedMomentum The type to be checked as a named Momentum unit.
   template<typename NamedMomentum>
   concept IsNamedMomentumUnit = requires {
      typename NamedMomentum::NumTuple;
      typename NamedMomentum::DenTuple;
   };

   /// @brief Concept for a Momentum dimension.
   /// @details Checks if the provided types satisfy the Momentum dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Mass1, typename Length1,typename Time1>
   concept IsMomentumUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType>;

   /// @brief Concept for a Momentum type.
   /// @details Ensures that the type meets Momentum type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsMomentumType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 2 && std::tuple_size_v<typename T::DenTuple> == 1 &&
   IsMomentumUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Momentum object.
   /// @details Provides access to the underlying value represented by a Momentum object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam MomentumType The type of the object being accessed.
   /// @param obj The Momentum object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1,typename Time1, typename MomentumType>
   requires IsMomentumUnits<Mass1, Length1,Time1> && IsMomentumType<MomentumType>
   constexpr PrecisionType getMomentum(const MomentumType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1>, std::tuple<Time1>>();
   }

   /// @brief Retrieves the value of a named Momentum object.
   /// @details Provides access to the value represented by a named Momentum object.
   /// @tparam NamedMomentum The named unit type.
   /// @tparam MomentumType The type of the object being accessed.
   /// @param obj The Momentum object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedMomentum, typename MomentumType>
   requires IsNamedMomentumUnit<NamedMomentum> && IsMomentumType<MomentumType>
   constexpr PrecisionType getMomentum(const MomentumType& obj)
   {
      return obj.template GetVal<typename NamedMomentum::NumTuple, typename NamedMomentum::DenTuple>();
   }

   template<typename... Ts>
   class Momentum;

   /// @brief Represents a Momentum.
   /// @details Defines operations and data storage for Momentum dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Mass1, typename Length1,typename Time1>
   requires IsMomentumUnits<Mass1, Length1,Time1>
   class Momentum<Mass1, Length1,Time1> : public BaseDimension<std::tuple<Mass1, Length1>, std::tuple<Time1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1>, std::tuple<Time1>>;
      using Base::Base;

      /// @brief Constructs a Momentum object with a value.
      /// @param val The value of the Momentum.
      explicit constexpr Momentum(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Momentum object from a named unit.
      /// @tparam NamedMomentum The named unit type.
      /// @param base The base unit object.
      template<typename NamedMomentum>
      requires IsNamedMomentumUnit<NamedMomentum>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Momentum(const NamedMomentum& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Momentum.
      /// @details Prefer using the free function `getMomentum()` instead.
      /// @return The value of the Momentum.
      template<typename Mass1T, typename Length1T,typename Time1T>
      requires IsMomentumUnits<Mass1T, Length1T,Time1T>
      [[deprecated("Use the free function getMomentum() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMomentum() const
      {
         return getMomentum<Mass1T, Length1T,Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of Momentum.
      /// @details Prefer using the free function `getMomentum()` instead.
      /// @return The value of the Momentum.
      template<typename NamedMomentum>
      requires IsNamedMomentumUnit<NamedMomentum>
      [[deprecated("Use the free function getMomentum() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMomentum() const
      {
         return getMomentum<NamedMomentum>(*this);
      }
   };

   /// @brief Represents a named Momentum class.
   /// @details Provides functionality for named Momentum units.
   /// @tparam NamedMomentum The named unit type.
   template<typename NamedMomentum>
   requires IsNamedMomentumUnit<NamedMomentum>
   class Momentum<NamedMomentum> : public BaseDimension<typename NamedMomentum::NumTuple, typename NamedMomentum::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedMomentum::NumTuple, typename NamedMomentum::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Momentum object with a value.
      /// @param val The value of the Momentum.
      explicit constexpr Momentum(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Momentum object from another Momentum object.
      /// @tparam OtherMomentum The other Momentum type.
      /// @param base The base Momentum object.
      template<typename OtherMomentum>
      requires IsMomentumType<OtherMomentum>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Momentum(const OtherMomentum& base)
         : Base(base.template GetVal<typename NamedMomentum::NumTuple, typename NamedMomentum::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Momentum.
      /// @details Prefer using the free function `getMomentum()` instead.
      /// @return The value of the Momentum.
      template<typename Mass1T, typename Length1T,typename Time1T>
      requires IsMomentumUnits<Mass1T, Length1T,Time1T>
      [[deprecated("Use the free function getMomentum() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMomentum() const
      {
         return getMomentum<Mass1T, Length1T,Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of Momentum.
      /// @details Prefer using the free function `getMomentum()` instead.
      /// @return The value of the Momentum.
      template<typename NamedMomentumUnit>
      requires IsNamedMomentumUnit<NamedMomentumUnit>
      [[deprecated("Use the free function getMomentum() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMomentum() const
      {
         return getMomentum<NamedMomentumUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Momentum.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Mass1, typename Length1,typename Time1>
   requires IsMomentumUnits<Mass1, Length1,Time1>
   Momentum(Mass1, Length1,Time1) -> Momentum<Mass1, Length1,Time1>;

   /// @brief Template deduction guide for Momentum.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename NamedMomentum>
   requires IsNamedMomentumUnit<NamedMomentum>
   Momentum(NamedMomentum) -> Momentum<NamedMomentum>;

   /// @brief Template deduction guide for Momentum.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Mass1, typename Length1,typename Time1>
   requires IsMomentumUnits<Mass1, Length1,Time1>
   Momentum(BaseDimension<std::tuple<Mass1, Length1>, std::tuple<Time1>>) -> Momentum<Mass1, Length1,Time1>;

}

#endif // STATIC_DIMENSION_MOMENTUM_IMPL_H