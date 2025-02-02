#ifndef STATIC_DIMENSION_MAGNETICFIELD_IMPL_H
#define STATIC_DIMENSION_MAGNETICFIELD_IMPL_H

#include "../../MassDimension.h"
#include "../../TimeDimension.h"
#include "../../ChargeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named MagneticField unit.
   /// @tparam NamedMagneticField The type to be checked as a named MagneticField unit.
   template<typename NamedMagneticField>
   concept IsNamedMagneticFieldUnit = requires {
      typename NamedMagneticField::NumTuple;
      typename NamedMagneticField::DenTuple;
   };

   /// @brief Concept for a MagneticField dimension.
   /// @details Checks if the provided types satisfy the MagneticField dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Time1, typename Charge1>
   concept IsMagneticFieldUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
      std::is_same_v<typename Time1::Dim, TimeType> &&
      std::is_same_v<typename Charge1::Dim, ChargeType>;

   /// @brief Concept for a MagneticField type.
   /// @details Ensures that the type meets MagneticField type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsMagneticFieldType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 2 &&
   IsMagneticFieldUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>>;

   /// @brief Retrieves the value of a MagneticField object.
   /// @details Provides access to the underlying value represented by a MagneticField object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam MagneticFieldType The type of the object being accessed.
   /// @param obj The MagneticField object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Time1, typename Charge1, typename MagneticFieldType>
   requires IsMagneticFieldUnits<Mass1, Time1, Charge1> && IsMagneticFieldType<MagneticFieldType>
   constexpr PrecisionType getMagneticField(const MagneticFieldType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1>, std::tuple<Time1, Charge1>>();
   }

   /// @brief Retrieves the value of a named MagneticField object.
   /// @details Provides access to the value represented by a named MagneticField object.
   /// @tparam NamedMagneticField The named unit type.
   /// @tparam MagneticFieldType The type of the object being accessed.
   /// @param obj The MagneticField object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedMagneticField, typename MagneticFieldType>
   requires IsNamedMagneticFieldUnit<NamedMagneticField> && IsMagneticFieldType<MagneticFieldType>
   constexpr PrecisionType getMagneticField(const MagneticFieldType& obj)
   {
      return obj.template GetVal<typename NamedMagneticField::NumTuple, typename NamedMagneticField::DenTuple>();
   }

   template<typename... Ts>
   class MagneticField;

   /// @brief Represents a default MagneticField.
   /// @details This MagneticField is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "MagneticField" type, without regard for the underlying units.
   template<>
   class MagneticField<> : public BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryTime, PrimaryCharge>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryTime, PrimaryCharge>>;
      using Base::Base;

      /// @brief Constructs a MagneticField object with a value.
      /// @param val The value of the MagneticField.
      explicit constexpr MagneticField(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MagneticField object from another MagneticField object.
      /// @tparam OtherMagneticField The other MagneticField type.
      /// @param base The base MagneticField object.
      template<typename OtherMagneticField>
      requires IsMagneticFieldType<OtherMagneticField>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MagneticField(const OtherMagneticField& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass>, std::tuple<PrimaryTime, PrimaryCharge>>()) {}
   };

   /// @brief Represents a MagneticField.
   /// @details Defines operations and data storage for MagneticField dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Time1, typename Charge1>
   requires IsMagneticFieldUnits<Mass1, Time1, Charge1>
   class MagneticField<Mass1, Time1, Charge1> : public BaseDimension<std::tuple<Mass1>, std::tuple<Time1, Charge1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1>, std::tuple<Time1, Charge1>>;
      using Base::Base;

      /// @brief Constructs a MagneticField object with a value.
      /// @param val The value of the MagneticField.
      explicit constexpr MagneticField(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MagneticField object from a named unit.
      /// @tparam NamedMagneticField The named unit type.
      /// @param base The base unit object.
      template<typename NamedMagneticField>
      requires IsNamedMagneticFieldUnit<NamedMagneticField>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MagneticField(const NamedMagneticField& base) : Base(base) {}

      /// @brief Deprecated function to get the value of MagneticField.
      /// @details Prefer using the free function `getMagneticField()` instead.
      /// @return The value of the MagneticField.
      template<typename Mass1T, typename Time1T, typename Charge1T>
      requires IsMagneticFieldUnits<Mass1T, Time1T, Charge1T>
      [[deprecated("Use the free function getMagneticField() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMagneticField() const
      {
         return getMagneticField<Mass1T, Time1T, Charge1T>(*this);
      }

      /// @brief Deprecated function to get the value of MagneticField.
      /// @details Prefer using the free function `getMagneticField()` instead.
      /// @return The value of the MagneticField.
      template<typename NamedMagneticField>
      requires IsNamedMagneticFieldUnit<NamedMagneticField>
      [[deprecated("Use the free function getMagneticField() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMagneticField() const
      {
         return getMagneticField<NamedMagneticField>(*this);
      }
   };

   /// @brief Represents a named MagneticField class.
   /// @details Provides functionality for named MagneticField units.
   /// @tparam NamedMagneticField The named unit type.
   template<typename NamedMagneticField>
   requires IsNamedMagneticFieldUnit<NamedMagneticField>
   class MagneticField<NamedMagneticField> : public BaseDimension<typename NamedMagneticField::NumTuple, typename NamedMagneticField::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedMagneticField::NumTuple, typename NamedMagneticField::DenTuple>;
      using Base::Base;

      /// @brief Constructs a MagneticField object with a value.
      /// @param val The value of the MagneticField.
      explicit constexpr MagneticField(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MagneticField object from another MagneticField object.
      /// @tparam OtherMagneticField The other MagneticField type.
      /// @param base The base MagneticField object.
      template<typename OtherMagneticField>
      requires IsMagneticFieldType<OtherMagneticField>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MagneticField(const OtherMagneticField& base)
         : Base(base.template GetVal<typename NamedMagneticField::NumTuple, typename NamedMagneticField::DenTuple>()) {}

      /// @brief Deprecated function to get the value of MagneticField.
      /// @details Prefer using the free function `getMagneticField()` instead.
      /// @return The value of the MagneticField.
      template<typename Mass1T, typename Time1T, typename Charge1T>
      requires IsMagneticFieldUnits<Mass1T, Time1T, Charge1T>
      [[deprecated("Use the free function getMagneticField() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMagneticField() const
      {
         return getMagneticField<Mass1T, Time1T, Charge1T>(*this);
      }

      /// @brief Deprecated function to get the value of MagneticField.
      /// @details Prefer using the free function `getMagneticField()` instead.
      /// @return The value of the MagneticField.
      template<typename NamedMagneticFieldUnit>
      requires IsNamedMagneticFieldUnit<NamedMagneticFieldUnit>
      [[deprecated("Use the free function getMagneticField() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMagneticField() const
      {
         return getMagneticField<NamedMagneticFieldUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for MagneticField.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Time1, typename Charge1>
   requires IsMagneticFieldUnits<Mass1, Time1, Charge1>
   MagneticField(Mass1, Time1, Charge1) -> MagneticField<Mass1, Time1, Charge1>;

   /// @brief Template deduction guide for MagneticField.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename NamedMagneticField>
   requires IsNamedMagneticFieldUnit<NamedMagneticField>
   MagneticField(NamedMagneticField) -> MagneticField<NamedMagneticField>;

   /// @brief Template deduction guide for MagneticField.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Time1, typename Charge1>
   requires IsMagneticFieldUnits<Mass1, Time1, Charge1>
   MagneticField(BaseDimension<std::tuple<Mass1>, std::tuple<Time1, Charge1>>) -> MagneticField<Mass1, Time1, Charge1>;

}

#endif // STATIC_DIMENSION_MAGNETICFIELD_IMPL_H