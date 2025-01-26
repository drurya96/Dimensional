#ifndef STATIC_DIMENSION_FORCE_IMPL_H
#define STATIC_DIMENSION_FORCE_IMPL_H

#include "../../LengthDimension.h"
#include "../../MassDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Force unit.
   /// @tparam NamedForce The type to be checked as a named Force unit.
   template<typename NamedForce>
   concept IsNamedForceUnit = requires {
      typename NamedForce::NumTuple;
      typename NamedForce::DenTuple;
   };

   /// @brief Concept for a Force dimension.
   /// @details Checks if the provided types satisfy the Force dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1, typename Length1,typename Time1, typename Time2>
   concept IsForceUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Time2::Dim, TimeType>;

   /// @brief Concept for a Force type.
   /// @details Ensures that the type meets Force type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsForceType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 2 && std::tuple_size_v<typename T::DenTuple> == 2 &&
   IsForceUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Force object.
   /// @details Provides access to the underlying value represented by a Force object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam ForceType The type of the object being accessed.
   /// @param obj The Force object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1,typename Time1, typename Time2, typename ForceType>
   requires IsForceUnits<Mass1, Length1,Time1, Time2> && IsForceType<ForceType>
   constexpr PrecisionType getForce(const ForceType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1>, std::tuple<Time1, Time2>>();
   }

   /// @brief Retrieves the value of a named Force object.
   /// @details Provides access to the value represented by a named Force object.
   /// @tparam NamedForce The named unit type.
   /// @tparam ForceType The type of the object being accessed.
   /// @param obj The Force object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedForce, typename ForceType>
   requires IsNamedForceUnit<NamedForce> && IsForceType<ForceType>
   constexpr PrecisionType getForce(const ForceType& obj)
   {
      return obj.template GetVal<typename NamedForce::NumTuple, typename NamedForce::DenTuple>();
   }

   template<typename... Ts>
   class Force;

   /// @brief Represents a Force.
   /// @details Defines operations and data storage for Force dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1, typename Length1,typename Time1, typename Time2>
   requires IsForceUnits<Mass1, Length1,Time1, Time2>
   class Force<Mass1, Length1,Time1, Time2> : public BaseDimension<std::tuple<Mass1, Length1>, std::tuple<Time1, Time2>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1>, std::tuple<Time1, Time2>>;
      using Base::Base;

      /// @brief Constructs a Force object with a value.
      /// @param val The value of the Force.
      explicit constexpr Force(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Force object from a named unit.
      /// @tparam NamedForce The named unit type.
      /// @param base The base unit object.
      template<typename NamedForce>
      requires IsNamedForceUnit<NamedForce>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Force(const NamedForce& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Force.
      /// @details Prefer using the free function `getForce()` instead.
      /// @return The value of the Force.
      template<typename Mass1T, typename Length1T,typename Time1T, typename Time2T>
      requires IsForceUnits<Mass1T, Length1T,Time1T, Time2T>
      [[deprecated("Use the free function getForce() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetForce() const
      {
         return getForce<Mass1T, Length1T,Time1T, Time2T>(*this);
      }

      /// @brief Deprecated function to get the value of Force.
      /// @details Prefer using the free function `getForce()` instead.
      /// @return The value of the Force.
      template<typename NamedForce>
      requires IsNamedForceUnit<NamedForce>
      [[deprecated("Use the free function getForce() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetForce() const
      {
         return getForce<NamedForce>(*this);
      }
   };

   /// @brief Represents a named Force class.
   /// @details Provides functionality for named Force units.
   /// @tparam NamedForce The named unit type.
   template<typename NamedForce>
   requires IsNamedForceUnit<NamedForce>
   class Force<NamedForce> : public BaseDimension<typename NamedForce::NumTuple, typename NamedForce::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedForce::NumTuple, typename NamedForce::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Force object with a value.
      /// @param val The value of the Force.
      explicit constexpr Force(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Force object from another Force object.
      /// @tparam OtherForce The other Force type.
      /// @param base The base Force object.
      template<typename OtherForce>
      requires IsForceType<OtherForce>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Force(const OtherForce& base)
         : Base(base.template GetVal<typename NamedForce::NumTuple, typename NamedForce::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Force.
      /// @details Prefer using the free function `getForce()` instead.
      /// @return The value of the Force.
      template<typename Mass1T, typename Length1T,typename Time1T, typename Time2T>
      requires IsForceUnits<Mass1T, Length1T,Time1T, Time2T>
      [[deprecated("Use the free function getForce() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetForce() const
      {
         return getForce<Mass1T, Length1T,Time1T, Time2T>(*this);
      }

      /// @brief Deprecated function to get the value of Force.
      /// @details Prefer using the free function `getForce()` instead.
      /// @return The value of the Force.
      template<typename NamedForceUnit>
      requires IsNamedForceUnit<NamedForceUnit>
      [[deprecated("Use the free function getForce() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetForce() const
      {
         return getForce<NamedForceUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Force.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1, typename Length1,typename Time1, typename Time2>
   requires IsForceUnits<Mass1, Length1,Time1, Time2>
   Force(Mass1, Length1,Time1, Time2) -> Force<Mass1, Length1,Time1, Time2>;

   /// @brief Template deduction guide for Force.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename NamedForce>
   requires IsNamedForceUnit<NamedForce>
   Force(NamedForce) -> Force<NamedForce>;

   /// @brief Template deduction guide for Force.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1, typename Length1,typename Time1, typename Time2>
   requires IsForceUnits<Mass1, Length1,Time1, Time2>
   Force(BaseDimension<std::tuple<Mass1, Length1>, std::tuple<Time1, Time2>>) -> Force<Mass1, Length1,Time1, Time2>;

}

#endif // STATIC_DIMENSION_FORCE_IMPL_H