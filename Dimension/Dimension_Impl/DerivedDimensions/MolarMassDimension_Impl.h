#ifndef STATIC_DIMENSION_MOLARMASS_IMPL_H
#define STATIC_DIMENSION_MOLARMASS_IMPL_H

#include "../../MassDimension.h"
#include "../../AmountDimension.h"

namespace Dimension
{
   /// @brief Concept for a named MolarMass unit.
   /// @tparam NamedMolarMass The type to be checked as a named MolarMass unit.
   template<typename NamedMolarMass>
   concept IsNamedMolarMassUnit = requires {
      typename NamedMolarMass::NumTuple;
      typename NamedMolarMass::DenTuple;
   };

   /// @brief Concept for a MolarMass dimension.
   /// @details Checks if the provided types satisfy the MolarMass dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Amount1 Denominator Amount1 type
   template<typename Mass1, typename Amount1>
   concept IsMolarMassUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
      std::is_same_v<typename Amount1::Dim, AmountType>;

   /// @brief Concept for a MolarMass type.
   /// @details Ensures that the type meets MolarMass type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsMolarMassType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 1 &&
   IsMolarMassUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<0, typename T::DenTuple>>;

   /// @brief Retrieves the value of a MolarMass object.
   /// @details Provides access to the underlying value represented by a MolarMass object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Amount1 Denominator Amount1 type
   /// @tparam MolarMassType The type of the object being accessed.
   /// @param obj The MolarMass object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Amount1, typename MolarMassType>
   requires IsMolarMassUnits<Mass1, Amount1> && IsMolarMassType<MolarMassType>
   constexpr PrecisionType getMolarMass(const MolarMassType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1>, std::tuple<Amount1>>();
   }

   /// @brief Retrieves the value of a named MolarMass object.
   /// @details Provides access to the value represented by a named MolarMass object.
   /// @tparam NamedMolarMass The named unit type.
   /// @tparam MolarMassType The type of the object being accessed.
   /// @param obj The MolarMass object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedMolarMass, typename MolarMassType>
   requires IsNamedMolarMassUnit<NamedMolarMass> && IsMolarMassType<MolarMassType>
   constexpr PrecisionType getMolarMass(const MolarMassType& obj)
   {
      return obj.template GetVal<typename NamedMolarMass::NumTuple, typename NamedMolarMass::DenTuple>();
   }

   template<typename... Ts>
   class MolarMass;

   /// @brief Represents a default MolarMass.
   /// @details This MolarMass is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "MolarMass" type, without regard for the underlying units.
   template<>
   class MolarMass<> : public BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryAmount>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryAmount>>;
      using Base::Base;

      /// @brief Constructs a MolarMass object with a value.
      /// @param val The value of the MolarMass.
      explicit constexpr MolarMass(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MolarMass object from another MolarMass object.
      /// @tparam OtherMolarMass The other MolarMass type.
      /// @param base The base MolarMass object.
      template<typename OtherMolarMass>
      requires IsMolarMassType<OtherMolarMass>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MolarMass(const OtherMolarMass& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass>, std::tuple<PrimaryAmount>>()) {}
   };

   /// @brief Represents a MolarMass.
   /// @details Defines operations and data storage for MolarMass dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Amount1 Denominator Amount1 type
   template<typename Mass1, typename Amount1>
   requires IsMolarMassUnits<Mass1, Amount1>
   class MolarMass<Mass1, Amount1> : public BaseDimension<std::tuple<Mass1>, std::tuple<Amount1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1>, std::tuple<Amount1>>;
      using Base::Base;

      /// @brief Constructs a MolarMass object with a value.
      /// @param val The value of the MolarMass.
      explicit constexpr MolarMass(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MolarMass object from a named unit.
      /// @tparam NamedMolarMass The named unit type.
      /// @param base The base unit object.
      template<typename NamedMolarMass>
      requires IsNamedMolarMassUnit<NamedMolarMass>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MolarMass(const NamedMolarMass& base) : Base(base) {}

      /// @brief Deprecated function to get the value of MolarMass.
      /// @details Prefer using the free function `getMolarMass()` instead.
      /// @return The value of the MolarMass.
      template<typename Mass1T, typename Amount1T>
      requires IsMolarMassUnits<Mass1T, Amount1T>
      [[deprecated("Use the free function getMolarMass() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMolarMass() const
      {
         return getMolarMass<Mass1T, Amount1T>(*this);
      }

      /// @brief Deprecated function to get the value of MolarMass.
      /// @details Prefer using the free function `getMolarMass()` instead.
      /// @return The value of the MolarMass.
      template<typename NamedMolarMass>
      requires IsNamedMolarMassUnit<NamedMolarMass>
      [[deprecated("Use the free function getMolarMass() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMolarMass() const
      {
         return getMolarMass<NamedMolarMass>(*this);
      }
   };

   /// @brief Represents a named MolarMass class.
   /// @details Provides functionality for named MolarMass units.
   /// @tparam NamedMolarMass The named unit type.
   template<typename NamedMolarMass>
   requires IsNamedMolarMassUnit<NamedMolarMass>
   class MolarMass<NamedMolarMass> : public BaseDimension<typename NamedMolarMass::NumTuple, typename NamedMolarMass::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedMolarMass::NumTuple, typename NamedMolarMass::DenTuple>;
      using Base::Base;

      /// @brief Constructs a MolarMass object with a value.
      /// @param val The value of the MolarMass.
      explicit constexpr MolarMass(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MolarMass object from another MolarMass object.
      /// @tparam OtherMolarMass The other MolarMass type.
      /// @param base The base MolarMass object.
      template<typename OtherMolarMass>
      requires IsMolarMassType<OtherMolarMass>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MolarMass(const OtherMolarMass& base)
         : Base(base.template GetVal<typename NamedMolarMass::NumTuple, typename NamedMolarMass::DenTuple>()) {}

      /// @brief Deprecated function to get the value of MolarMass.
      /// @details Prefer using the free function `getMolarMass()` instead.
      /// @return The value of the MolarMass.
      template<typename Mass1T, typename Amount1T>
      requires IsMolarMassUnits<Mass1T, Amount1T>
      [[deprecated("Use the free function getMolarMass() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMolarMass() const
      {
         return getMolarMass<Mass1T, Amount1T>(*this);
      }

      /// @brief Deprecated function to get the value of MolarMass.
      /// @details Prefer using the free function `getMolarMass()` instead.
      /// @return The value of the MolarMass.
      template<typename NamedMolarMassUnit>
      requires IsNamedMolarMassUnit<NamedMolarMassUnit>
      [[deprecated("Use the free function getMolarMass() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMolarMass() const
      {
         return getMolarMass<NamedMolarMassUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for MolarMass.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Amount1 Denominator Amount1 type
   template<typename Mass1, typename Amount1>
   requires IsMolarMassUnits<Mass1, Amount1>
   MolarMass(Mass1, Amount1) -> MolarMass<Mass1, Amount1>;

   /// @brief Template deduction guide for MolarMass.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Amount1 Denominator Amount1 type
   template<typename NamedMolarMass>
   requires IsNamedMolarMassUnit<NamedMolarMass>
   MolarMass(NamedMolarMass) -> MolarMass<NamedMolarMass>;

   /// @brief Template deduction guide for MolarMass.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Amount1 Denominator Amount1 type
   template<typename Mass1, typename Amount1>
   requires IsMolarMassUnits<Mass1, Amount1>
   MolarMass(BaseDimension<std::tuple<Mass1>, std::tuple<Amount1>>) -> MolarMass<Mass1, Amount1>;

}

#endif // STATIC_DIMENSION_MOLARMASS_IMPL_H