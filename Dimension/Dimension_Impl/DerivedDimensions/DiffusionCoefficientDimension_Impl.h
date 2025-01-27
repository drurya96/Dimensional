#ifndef STATIC_DIMENSION_DIFFUSIONCOEFFICIENT_IMPL_H
#define STATIC_DIMENSION_DIFFUSIONCOEFFICIENT_IMPL_H

#include "../../TimeDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept for a named DiffusionCoefficient unit.
   /// @tparam NamedDiffusionCoefficient The type to be checked as a named DiffusionCoefficient unit.
   template<typename NamedDiffusionCoefficient>
   concept IsNamedDiffusionCoefficientUnit = requires {
      typename NamedDiffusionCoefficient::NumTuple;
      typename NamedDiffusionCoefficient::DenTuple;
   };

   /// @brief Concept for a DiffusionCoefficient dimension.
   /// @details Checks if the provided types satisfy the DiffusionCoefficient dimension requirements.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Length2,typename Time1>
   concept IsDiffusionCoefficientUnits = 
      std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType>;

   /// @brief Concept for a DiffusionCoefficient type.
   /// @details Ensures that the type meets DiffusionCoefficient type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsDiffusionCoefficientType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 2 && std::tuple_size_v<typename T::DenTuple> == 1 &&
   IsDiffusionCoefficientUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>>;

   /// @brief Retrieves the value of a DiffusionCoefficient object.
   /// @details Provides access to the underlying value represented by a DiffusionCoefficient object.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam DiffusionCoefficientType The type of the object being accessed.
   /// @param obj The DiffusionCoefficient object.
   /// @return The underlying value as `PrecisionType`
   template<typename Length1, typename Length2,typename Time1, typename DiffusionCoefficientType>
   requires IsDiffusionCoefficientUnits<Length1, Length2,Time1> && IsDiffusionCoefficientType<DiffusionCoefficientType>
   constexpr PrecisionType getDiffusionCoefficient(const DiffusionCoefficientType& obj)
   {
      return obj.template GetVal<std::tuple<Length1, Length2>, std::tuple<Time1>>();
   }

   /// @brief Retrieves the value of a named DiffusionCoefficient object.
   /// @details Provides access to the value represented by a named DiffusionCoefficient object.
   /// @tparam NamedDiffusionCoefficient The named unit type.
   /// @tparam DiffusionCoefficientType The type of the object being accessed.
   /// @param obj The DiffusionCoefficient object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedDiffusionCoefficient, typename DiffusionCoefficientType>
   requires IsNamedDiffusionCoefficientUnit<NamedDiffusionCoefficient> && IsDiffusionCoefficientType<DiffusionCoefficientType>
   constexpr PrecisionType getDiffusionCoefficient(const DiffusionCoefficientType& obj)
   {
      return obj.template GetVal<typename NamedDiffusionCoefficient::NumTuple, typename NamedDiffusionCoefficient::DenTuple>();
   }

   template<typename... Ts>
   class DiffusionCoefficient;

   /// @brief Represents a DiffusionCoefficient.
   /// @details Defines operations and data storage for DiffusionCoefficient dimensions.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Length2,typename Time1>
   requires IsDiffusionCoefficientUnits<Length1, Length2,Time1>
   class DiffusionCoefficient<Length1, Length2,Time1> : public BaseDimension<std::tuple<Length1, Length2>, std::tuple<Time1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Length1, Length2>, std::tuple<Time1>>;
      using Base::Base;

      /// @brief Constructs a DiffusionCoefficient object with a value.
      /// @param val The value of the DiffusionCoefficient.
      explicit constexpr DiffusionCoefficient(PrecisionType val) : Base(val) {}

      /// @brief Constructs a DiffusionCoefficient object from a named unit.
      /// @tparam NamedDiffusionCoefficient The named unit type.
      /// @param base The base unit object.
      template<typename NamedDiffusionCoefficient>
      requires IsNamedDiffusionCoefficientUnit<NamedDiffusionCoefficient>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr DiffusionCoefficient(const NamedDiffusionCoefficient& base) : Base(base) {}

      /// @brief Deprecated function to get the value of DiffusionCoefficient.
      /// @details Prefer using the free function `getDiffusionCoefficient()` instead.
      /// @return The value of the DiffusionCoefficient.
      template<typename Length1T, typename Length2T,typename Time1T>
      requires IsDiffusionCoefficientUnits<Length1T, Length2T,Time1T>
      [[deprecated("Use the free function getDiffusionCoefficient() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDiffusionCoefficient() const
      {
         return getDiffusionCoefficient<Length1T, Length2T,Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of DiffusionCoefficient.
      /// @details Prefer using the free function `getDiffusionCoefficient()` instead.
      /// @return The value of the DiffusionCoefficient.
      template<typename NamedDiffusionCoefficient>
      requires IsNamedDiffusionCoefficientUnit<NamedDiffusionCoefficient>
      [[deprecated("Use the free function getDiffusionCoefficient() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDiffusionCoefficient() const
      {
         return getDiffusionCoefficient<NamedDiffusionCoefficient>(*this);
      }
   };

   /// @brief Represents a named DiffusionCoefficient class.
   /// @details Provides functionality for named DiffusionCoefficient units.
   /// @tparam NamedDiffusionCoefficient The named unit type.
   template<typename NamedDiffusionCoefficient>
   requires IsNamedDiffusionCoefficientUnit<NamedDiffusionCoefficient>
   class DiffusionCoefficient<NamedDiffusionCoefficient> : public BaseDimension<typename NamedDiffusionCoefficient::NumTuple, typename NamedDiffusionCoefficient::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedDiffusionCoefficient::NumTuple, typename NamedDiffusionCoefficient::DenTuple>;
      using Base::Base;

      /// @brief Constructs a DiffusionCoefficient object with a value.
      /// @param val The value of the DiffusionCoefficient.
      explicit constexpr DiffusionCoefficient(PrecisionType val) : Base(val) {}

      /// @brief Constructs a DiffusionCoefficient object from another DiffusionCoefficient object.
      /// @tparam OtherDiffusionCoefficient The other DiffusionCoefficient type.
      /// @param base The base DiffusionCoefficient object.
      template<typename OtherDiffusionCoefficient>
      requires IsDiffusionCoefficientType<OtherDiffusionCoefficient>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr DiffusionCoefficient(const OtherDiffusionCoefficient& base)
         : Base(base.template GetVal<typename NamedDiffusionCoefficient::NumTuple, typename NamedDiffusionCoefficient::DenTuple>()) {}

      /// @brief Deprecated function to get the value of DiffusionCoefficient.
      /// @details Prefer using the free function `getDiffusionCoefficient()` instead.
      /// @return The value of the DiffusionCoefficient.
      template<typename Length1T, typename Length2T,typename Time1T>
      requires IsDiffusionCoefficientUnits<Length1T, Length2T,Time1T>
      [[deprecated("Use the free function getDiffusionCoefficient() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDiffusionCoefficient() const
      {
         return getDiffusionCoefficient<Length1T, Length2T,Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of DiffusionCoefficient.
      /// @details Prefer using the free function `getDiffusionCoefficient()` instead.
      /// @return The value of the DiffusionCoefficient.
      template<typename NamedDiffusionCoefficientUnit>
      requires IsNamedDiffusionCoefficientUnit<NamedDiffusionCoefficientUnit>
      [[deprecated("Use the free function getDiffusionCoefficient() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDiffusionCoefficient() const
      {
         return getDiffusionCoefficient<NamedDiffusionCoefficientUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for DiffusionCoefficient.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Length2,typename Time1>
   requires IsDiffusionCoefficientUnits<Length1, Length2,Time1>
   DiffusionCoefficient(Length1, Length2,Time1) -> DiffusionCoefficient<Length1, Length2,Time1>;

   /// @brief Template deduction guide for DiffusionCoefficient.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   template<typename NamedDiffusionCoefficient>
   requires IsNamedDiffusionCoefficientUnit<NamedDiffusionCoefficient>
   DiffusionCoefficient(NamedDiffusionCoefficient) -> DiffusionCoefficient<NamedDiffusionCoefficient>;

   /// @brief Template deduction guide for DiffusionCoefficient.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Length1, typename Length2,typename Time1>
   requires IsDiffusionCoefficientUnits<Length1, Length2,Time1>
   DiffusionCoefficient(BaseDimension<std::tuple<Length1, Length2>, std::tuple<Time1>>) -> DiffusionCoefficient<Length1, Length2,Time1>;

}

#endif // STATIC_DIMENSION_DIFFUSIONCOEFFICIENT_IMPL_H