#ifndef STATIC_DIMENSION_MOMENTOFINERTIA_IMPL_H
#define STATIC_DIMENSION_MOMENTOFINERTIA_IMPL_H

#include "../../MassDimension.h"
#include "../../LengthDimension.h"


namespace Dimension
{
   /// @brief Concept for a named MomentOfInertia unit.
   /// @tparam NamedMomentOfInertia The type to be checked as a named MomentOfInertia unit.
   template<typename NamedMomentOfInertia>
   concept IsNamedMomentOfInertiaUnit = requires {
      typename NamedMomentOfInertia::NumTuple;
      typename NamedMomentOfInertia::DenTuple;
   };

   /// @brief Concept for a MomentOfInertia dimension.
   /// @details Checks if the provided types satisfy the MomentOfInertia dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   template<typename Mass1, typename Length1, typename Length2>
   concept IsMomentOfInertiaUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
      std::is_same_v<typename Length1::Dim, LengthType> &&
      std::is_same_v<typename Length2::Dim, LengthType>;

   /// @brief Concept for a MomentOfInertia type.
   /// @details Ensures that the type meets MomentOfInertia type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsMomentOfInertiaType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 0 &&
   IsMomentOfInertiaUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>>;

   /// @brief Retrieves the value of a MomentOfInertia object.
   /// @details Provides access to the underlying value represented by a MomentOfInertia object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam MomentOfInertiaType The type of the object being accessed.
   /// @param obj The MomentOfInertia object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1, typename Length2, typename MomentOfInertiaType>
   requires IsMomentOfInertiaUnits<Mass1, Length1, Length2> && IsMomentOfInertiaType<MomentOfInertiaType>
   constexpr PrecisionType getMomentOfInertia(const MomentOfInertiaType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1, Length2>, std::tuple<>>();
   }

   /// @brief Retrieves the value of a named MomentOfInertia object.
   /// @details Provides access to the value represented by a named MomentOfInertia object.
   /// @tparam NamedMomentOfInertia The named unit type.
   /// @tparam MomentOfInertiaType The type of the object being accessed.
   /// @param obj The MomentOfInertia object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedMomentOfInertia, typename MomentOfInertiaType>
   requires IsNamedMomentOfInertiaUnit<NamedMomentOfInertia> && IsMomentOfInertiaType<MomentOfInertiaType>
   constexpr PrecisionType getMomentOfInertia(const MomentOfInertiaType& obj)
   {
      return obj.template GetVal<typename NamedMomentOfInertia::NumTuple, typename NamedMomentOfInertia::DenTuple>();
   }

   template<typename... Ts>
   class MomentOfInertia;

   /// @brief Represents a default MomentOfInertia.
   /// @details This MomentOfInertia is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "MomentOfInertia" type, without regard for the underlying units.
   template<>
   class MomentOfInertia<> : public BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<>>;
      using Base::Base;

      /// @brief Constructs a MomentOfInertia object with a value.
      /// @param val The value of the MomentOfInertia.
      explicit constexpr MomentOfInertia(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MomentOfInertia object from another MomentOfInertia object.
      /// @tparam OtherMomentOfInertia The other MomentOfInertia type.
      /// @param base The base MomentOfInertia object.
      template<typename OtherMomentOfInertia>
      requires IsMomentOfInertiaType<OtherMomentOfInertia>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MomentOfInertia(const OtherMomentOfInertia& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<>>()) {}
   };

   /// @brief Represents a MomentOfInertia.
   /// @details Defines operations and data storage for MomentOfInertia dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   template<typename Mass1, typename Length1, typename Length2>
   requires IsMomentOfInertiaUnits<Mass1, Length1, Length2>
   class MomentOfInertia<Mass1, Length1, Length2> : public BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<>>;
      using Base::Base;

      /// @brief Constructs a MomentOfInertia object with a value.
      /// @param val The value of the MomentOfInertia.
      explicit constexpr MomentOfInertia(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MomentOfInertia object from a named unit.
      /// @tparam NamedMomentOfInertia The named unit type.
      /// @param base The base unit object.
      template<typename NamedMomentOfInertia>
      requires IsNamedMomentOfInertiaUnit<NamedMomentOfInertia>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MomentOfInertia(const NamedMomentOfInertia& base) : Base(base) {}

      /// @brief Deprecated function to get the value of MomentOfInertia.
      /// @details Prefer using the free function `getMomentOfInertia()` instead.
      /// @return The value of the MomentOfInertia.
      template<typename Mass1T, typename Length1T, typename Length2T>
      requires IsMomentOfInertiaUnits<Mass1T, Length1T, Length2T>
      [[deprecated("Use the free function getMomentOfInertia() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMomentOfInertia() const
      {
         return getMomentOfInertia<Mass1T, Length1T, Length2T>(*this);
      }

      /// @brief Deprecated function to get the value of MomentOfInertia.
      /// @details Prefer using the free function `getMomentOfInertia()` instead.
      /// @return The value of the MomentOfInertia.
      template<typename NamedMomentOfInertia>
      requires IsNamedMomentOfInertiaUnit<NamedMomentOfInertia>
      [[deprecated("Use the free function getMomentOfInertia() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMomentOfInertia() const
      {
         return getMomentOfInertia<NamedMomentOfInertia>(*this);
      }
   };

   /// @brief Represents a named MomentOfInertia class.
   /// @details Provides functionality for named MomentOfInertia units.
   /// @tparam NamedMomentOfInertia The named unit type.
   template<typename NamedMomentOfInertia>
   requires IsNamedMomentOfInertiaUnit<NamedMomentOfInertia>
   class MomentOfInertia<NamedMomentOfInertia> : public BaseDimension<typename NamedMomentOfInertia::NumTuple, typename NamedMomentOfInertia::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedMomentOfInertia::NumTuple, typename NamedMomentOfInertia::DenTuple>;
      using Base::Base;

      /// @brief Constructs a MomentOfInertia object with a value.
      /// @param val The value of the MomentOfInertia.
      explicit constexpr MomentOfInertia(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MomentOfInertia object from another MomentOfInertia object.
      /// @tparam OtherMomentOfInertia The other MomentOfInertia type.
      /// @param base The base MomentOfInertia object.
      template<typename OtherMomentOfInertia>
      requires IsMomentOfInertiaType<OtherMomentOfInertia>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr MomentOfInertia(const OtherMomentOfInertia& base)
         : Base(base.template GetVal<typename NamedMomentOfInertia::NumTuple, typename NamedMomentOfInertia::DenTuple>()) {}

      /// @brief Deprecated function to get the value of MomentOfInertia.
      /// @details Prefer using the free function `getMomentOfInertia()` instead.
      /// @return The value of the MomentOfInertia.
      template<typename Mass1T, typename Length1T, typename Length2T>
      requires IsMomentOfInertiaUnits<Mass1T, Length1T, Length2T>
      [[deprecated("Use the free function getMomentOfInertia() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMomentOfInertia() const
      {
         return getMomentOfInertia<Mass1T, Length1T, Length2T>(*this);
      }

      /// @brief Deprecated function to get the value of MomentOfInertia.
      /// @details Prefer using the free function `getMomentOfInertia()` instead.
      /// @return The value of the MomentOfInertia.
      template<typename NamedMomentOfInertiaUnit>
      requires IsNamedMomentOfInertiaUnit<NamedMomentOfInertiaUnit>
      [[deprecated("Use the free function getMomentOfInertia() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetMomentOfInertia() const
      {
         return getMomentOfInertia<NamedMomentOfInertiaUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for MomentOfInertia.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   template<typename Mass1, typename Length1, typename Length2>
   requires IsMomentOfInertiaUnits<Mass1, Length1, Length2>
   MomentOfInertia(Mass1, Length1, Length2) -> MomentOfInertia<Mass1, Length1, Length2>;

   /// @brief Template deduction guide for MomentOfInertia.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   template<typename NamedMomentOfInertia>
   requires IsNamedMomentOfInertiaUnit<NamedMomentOfInertia>
   MomentOfInertia(NamedMomentOfInertia) -> MomentOfInertia<NamedMomentOfInertia>;

   /// @brief Template deduction guide for MomentOfInertia.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   template<typename Mass1, typename Length1, typename Length2>
   requires IsMomentOfInertiaUnits<Mass1, Length1, Length2>
   MomentOfInertia(BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<>>) -> MomentOfInertia<Mass1, Length1, Length2>;

}

#endif // STATIC_DIMENSION_MOMENTOFINERTIA_IMPL_H