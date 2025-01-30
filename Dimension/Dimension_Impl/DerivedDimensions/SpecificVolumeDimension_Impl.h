#ifndef STATIC_DIMENSION_SPECIFICVOLUME_IMPL_H
#define STATIC_DIMENSION_SPECIFICVOLUME_IMPL_H

#include "../../MassDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept for a named SpecificVolume unit.
   /// @tparam NamedSpecificVolume The type to be checked as a named SpecificVolume unit.
   template<typename NamedSpecificVolume>
   concept IsNamedSpecificVolumeUnit = requires {
      typename NamedSpecificVolume::NumTuple;
      typename NamedSpecificVolume::DenTuple;
   };

   /// @brief Concept for a SpecificVolume dimension.
   /// @details Checks if the provided types satisfy the SpecificVolume dimension requirements.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Mass1 Denominator Mass1 type
   template<typename Length1, typename Length2, typename Length3,typename Mass1>
   concept IsSpecificVolumeUnits = 
      std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Length3::Dim, LengthType> &&
        std::is_same_v<typename Mass1::Dim, MassType>;

   /// @brief Concept for a SpecificVolume type.
   /// @details Ensures that the type meets SpecificVolume type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsSpecificVolumeType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 1 &&
   IsSpecificVolumeUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>>;

   /// @brief Retrieves the value of a SpecificVolume object.
   /// @details Provides access to the underlying value represented by a SpecificVolume object.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Mass1 Denominator Mass1 type
   /// @tparam SpecificVolumeType The type of the object being accessed.
   /// @param obj The SpecificVolume object.
   /// @return The underlying value as `PrecisionType`
   template<typename Length1, typename Length2, typename Length3,typename Mass1, typename SpecificVolumeType>
   requires IsSpecificVolumeUnits<Length1, Length2, Length3,Mass1> && IsSpecificVolumeType<SpecificVolumeType>
   constexpr PrecisionType getSpecificVolume(const SpecificVolumeType& obj)
   {
      return obj.template GetVal<std::tuple<Length1, Length2, Length3>, std::tuple<Mass1>>();
   }

   /// @brief Retrieves the value of a named SpecificVolume object.
   /// @details Provides access to the value represented by a named SpecificVolume object.
   /// @tparam NamedSpecificVolume The named unit type.
   /// @tparam SpecificVolumeType The type of the object being accessed.
   /// @param obj The SpecificVolume object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedSpecificVolume, typename SpecificVolumeType>
   requires IsNamedSpecificVolumeUnit<NamedSpecificVolume> && IsSpecificVolumeType<SpecificVolumeType>
   constexpr PrecisionType getSpecificVolume(const SpecificVolumeType& obj)
   {
      return obj.template GetVal<typename NamedSpecificVolume::NumTuple, typename NamedSpecificVolume::DenTuple>();
   }

   template<typename... Ts>
   class SpecificVolume;

   /// @brief Represents a default SpecificVolume.
   /// @details This SpecificVolume is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "SpecificVolume" type, without regard for the underlying units.
   template<>
   class SpecificVolume<> : public BaseDimension<std::tuple<PrimaryLength, PrimaryLength, PrimaryLength>, std::tuple<PrimaryMass>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryLength, PrimaryLength, PrimaryLength>, std::tuple<PrimaryMass>>;
      using Base::Base;

      /// @brief Constructs a SpecificVolume object with a value.
      /// @param val The value of the SpecificVolume.
      explicit constexpr SpecificVolume(PrecisionType val) : Base(val) {}

      /// @brief Constructs a SpecificVolume object from another SpecificVolume object.
      /// @tparam OtherSpecificVolume The other SpecificVolume type.
      /// @param base The base SpecificVolume object.
      template<typename OtherSpecificVolume>
      requires IsSpecificVolumeType<OtherSpecificVolume>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr SpecificVolume(const OtherSpecificVolume& base)
         : Base(base.template GetVal<std::tuple<PrimaryLength, PrimaryLength, PrimaryLength>, std::tuple<PrimaryMass>>()) {}
   };

   /// @brief Represents a SpecificVolume.
   /// @details Defines operations and data storage for SpecificVolume dimensions.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Mass1 Denominator Mass1 type
   template<typename Length1, typename Length2, typename Length3,typename Mass1>
   requires IsSpecificVolumeUnits<Length1, Length2, Length3,Mass1>
   class SpecificVolume<Length1, Length2, Length3,Mass1> : public BaseDimension<std::tuple<Length1, Length2, Length3>, std::tuple<Mass1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Length1, Length2, Length3>, std::tuple<Mass1>>;
      using Base::Base;

      /// @brief Constructs a SpecificVolume object with a value.
      /// @param val The value of the SpecificVolume.
      explicit constexpr SpecificVolume(PrecisionType val) : Base(val) {}

      /// @brief Constructs a SpecificVolume object from a named unit.
      /// @tparam NamedSpecificVolume The named unit type.
      /// @param base The base unit object.
      template<typename NamedSpecificVolume>
      requires IsNamedSpecificVolumeUnit<NamedSpecificVolume>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr SpecificVolume(const NamedSpecificVolume& base) : Base(base) {}

      /// @brief Deprecated function to get the value of SpecificVolume.
      /// @details Prefer using the free function `getSpecificVolume()` instead.
      /// @return The value of the SpecificVolume.
      template<typename Length1T, typename Length2T, typename Length3T,typename Mass1T>
      requires IsSpecificVolumeUnits<Length1T, Length2T, Length3T,Mass1T>
      [[deprecated("Use the free function getSpecificVolume() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetSpecificVolume() const
      {
         return getSpecificVolume<Length1T, Length2T, Length3T,Mass1T>(*this);
      }

      /// @brief Deprecated function to get the value of SpecificVolume.
      /// @details Prefer using the free function `getSpecificVolume()` instead.
      /// @return The value of the SpecificVolume.
      template<typename NamedSpecificVolume>
      requires IsNamedSpecificVolumeUnit<NamedSpecificVolume>
      [[deprecated("Use the free function getSpecificVolume() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetSpecificVolume() const
      {
         return getSpecificVolume<NamedSpecificVolume>(*this);
      }
   };

   /// @brief Represents a named SpecificVolume class.
   /// @details Provides functionality for named SpecificVolume units.
   /// @tparam NamedSpecificVolume The named unit type.
   template<typename NamedSpecificVolume>
   requires IsNamedSpecificVolumeUnit<NamedSpecificVolume>
   class SpecificVolume<NamedSpecificVolume> : public BaseDimension<typename NamedSpecificVolume::NumTuple, typename NamedSpecificVolume::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedSpecificVolume::NumTuple, typename NamedSpecificVolume::DenTuple>;
      using Base::Base;

      /// @brief Constructs a SpecificVolume object with a value.
      /// @param val The value of the SpecificVolume.
      explicit constexpr SpecificVolume(PrecisionType val) : Base(val) {}

      /// @brief Constructs a SpecificVolume object from another SpecificVolume object.
      /// @tparam OtherSpecificVolume The other SpecificVolume type.
      /// @param base The base SpecificVolume object.
      template<typename OtherSpecificVolume>
      requires IsSpecificVolumeType<OtherSpecificVolume>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr SpecificVolume(const OtherSpecificVolume& base)
         : Base(base.template GetVal<typename NamedSpecificVolume::NumTuple, typename NamedSpecificVolume::DenTuple>()) {}

      /// @brief Deprecated function to get the value of SpecificVolume.
      /// @details Prefer using the free function `getSpecificVolume()` instead.
      /// @return The value of the SpecificVolume.
      template<typename Length1T, typename Length2T, typename Length3T,typename Mass1T>
      requires IsSpecificVolumeUnits<Length1T, Length2T, Length3T,Mass1T>
      [[deprecated("Use the free function getSpecificVolume() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetSpecificVolume() const
      {
         return getSpecificVolume<Length1T, Length2T, Length3T,Mass1T>(*this);
      }

      /// @brief Deprecated function to get the value of SpecificVolume.
      /// @details Prefer using the free function `getSpecificVolume()` instead.
      /// @return The value of the SpecificVolume.
      template<typename NamedSpecificVolumeUnit>
      requires IsNamedSpecificVolumeUnit<NamedSpecificVolumeUnit>
      [[deprecated("Use the free function getSpecificVolume() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetSpecificVolume() const
      {
         return getSpecificVolume<NamedSpecificVolumeUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for SpecificVolume.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Mass1 Denominator Mass1 type
   template<typename Length1, typename Length2, typename Length3,typename Mass1>
   requires IsSpecificVolumeUnits<Length1, Length2, Length3,Mass1>
   SpecificVolume(Length1, Length2, Length3,Mass1) -> SpecificVolume<Length1, Length2, Length3,Mass1>;

   /// @brief Template deduction guide for SpecificVolume.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Mass1 Denominator Mass1 type
   template<typename NamedSpecificVolume>
   requires IsNamedSpecificVolumeUnit<NamedSpecificVolume>
   SpecificVolume(NamedSpecificVolume) -> SpecificVolume<NamedSpecificVolume>;

   /// @brief Template deduction guide for SpecificVolume.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam Mass1 Denominator Mass1 type
   template<typename Length1, typename Length2, typename Length3,typename Mass1>
   requires IsSpecificVolumeUnits<Length1, Length2, Length3,Mass1>
   SpecificVolume(BaseDimension<std::tuple<Length1, Length2, Length3>, std::tuple<Mass1>>) -> SpecificVolume<Length1, Length2, Length3,Mass1>;

}

#endif // STATIC_DIMENSION_SPECIFICVOLUME_IMPL_H