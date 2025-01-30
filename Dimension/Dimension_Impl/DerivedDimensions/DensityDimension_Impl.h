#ifndef STATIC_DIMENSION_DENSITY_IMPL_H
#define STATIC_DIMENSION_DENSITY_IMPL_H

#include "../../LengthDimension.h"
#include "../../MassDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Density unit.
   /// @tparam NamedDensity The type to be checked as a named Density unit.
   template<typename NamedDensity>
   concept IsNamedDensityUnit = requires {
      typename NamedDensity::NumTuple;
      typename NamedDensity::DenTuple;
   };

   /// @brief Concept for a Density dimension.
   /// @details Checks if the provided types satisfy the Density dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   /// @tparam Length3 Denominator Length3 type
   template<typename Mass1,typename Length1, typename Length2, typename Length3>
   concept IsDensityUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Length3::Dim, LengthType>;

   /// @brief Concept for a Density type.
   /// @details Ensures that the type meets Density type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsDensityType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsDensityUnits<typename std::tuple_element_t<0, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Density object.
   /// @details Provides access to the underlying value represented by a Density object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   /// @tparam Length3 Denominator Length3 type
   /// @tparam DensityType The type of the object being accessed.
   /// @param obj The Density object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1,typename Length1, typename Length2, typename Length3, typename DensityType>
   requires IsDensityUnits<Mass1,Length1, Length2, Length3> && IsDensityType<DensityType>
   constexpr PrecisionType getDensity(const DensityType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1>, std::tuple<Length1, Length2, Length3>>();
   }

   /// @brief Retrieves the value of a named Density object.
   /// @details Provides access to the value represented by a named Density object.
   /// @tparam NamedDensity The named unit type.
   /// @tparam DensityType The type of the object being accessed.
   /// @param obj The Density object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedDensity, typename DensityType>
   requires IsNamedDensityUnit<NamedDensity> && IsDensityType<DensityType>
   constexpr PrecisionType getDensity(const DensityType& obj)
   {
      return obj.template GetVal<typename NamedDensity::NumTuple, typename NamedDensity::DenTuple>();
   }

   template<typename... Ts>
   class Density;

   /// @brief Represents a default Density.
   /// @details This Density is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "Density" type, without regard for the underlying units.
   template<>
   class Density<> : public BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryLength, PrimaryLength, PrimaryLength>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryLength, PrimaryLength, PrimaryLength>>;
      using Base::Base;

      /// @brief Constructs a Density object with a value.
      /// @param val The value of the Density.
      explicit constexpr Density(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Density object from another Density object.
      /// @tparam OtherDensity The other Density type.
      /// @param base The base Density object.
      template<typename OtherDensity>
      requires IsDensityType<OtherDensity>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Density(const OtherDensity& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass>, std::tuple<PrimaryLength, PrimaryLength, PrimaryLength>>()) {}
   };

   /// @brief Represents a Density.
   /// @details Defines operations and data storage for Density dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   /// @tparam Length3 Denominator Length3 type
   template<typename Mass1,typename Length1, typename Length2, typename Length3>
   requires IsDensityUnits<Mass1,Length1, Length2, Length3>
   class Density<Mass1,Length1, Length2, Length3> : public BaseDimension<std::tuple<Mass1>, std::tuple<Length1, Length2, Length3>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1>, std::tuple<Length1, Length2, Length3>>;
      using Base::Base;

      /// @brief Constructs a Density object with a value.
      /// @param val The value of the Density.
      explicit constexpr Density(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Density object from a named unit.
      /// @tparam NamedDensity The named unit type.
      /// @param base The base unit object.
      template<typename NamedDensity>
      requires IsNamedDensityUnit<NamedDensity>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Density(const NamedDensity& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Density.
      /// @details Prefer using the free function `getDensity()` instead.
      /// @return The value of the Density.
      template<typename Mass1T,typename Length1T, typename Length2T, typename Length3T>
      requires IsDensityUnits<Mass1T,Length1T, Length2T, Length3T>
      [[deprecated("Use the free function getDensity() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDensity() const
      {
         return getDensity<Mass1T,Length1T, Length2T, Length3T>(*this);
      }

      /// @brief Deprecated function to get the value of Density.
      /// @details Prefer using the free function `getDensity()` instead.
      /// @return The value of the Density.
      template<typename NamedDensity>
      requires IsNamedDensityUnit<NamedDensity>
      [[deprecated("Use the free function getDensity() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDensity() const
      {
         return getDensity<NamedDensity>(*this);
      }
   };

   /// @brief Represents a named Density class.
   /// @details Provides functionality for named Density units.
   /// @tparam NamedDensity The named unit type.
   template<typename NamedDensity>
   requires IsNamedDensityUnit<NamedDensity>
   class Density<NamedDensity> : public BaseDimension<typename NamedDensity::NumTuple, typename NamedDensity::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedDensity::NumTuple, typename NamedDensity::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Density object with a value.
      /// @param val The value of the Density.
      explicit constexpr Density(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Density object from another Density object.
      /// @tparam OtherDensity The other Density type.
      /// @param base The base Density object.
      template<typename OtherDensity>
      requires IsDensityType<OtherDensity>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Density(const OtherDensity& base)
         : Base(base.template GetVal<typename NamedDensity::NumTuple, typename NamedDensity::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Density.
      /// @details Prefer using the free function `getDensity()` instead.
      /// @return The value of the Density.
      template<typename Mass1T,typename Length1T, typename Length2T, typename Length3T>
      requires IsDensityUnits<Mass1T,Length1T, Length2T, Length3T>
      [[deprecated("Use the free function getDensity() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDensity() const
      {
         return getDensity<Mass1T,Length1T, Length2T, Length3T>(*this);
      }

      /// @brief Deprecated function to get the value of Density.
      /// @details Prefer using the free function `getDensity()` instead.
      /// @return The value of the Density.
      template<typename NamedDensityUnit>
      requires IsNamedDensityUnit<NamedDensityUnit>
      [[deprecated("Use the free function getDensity() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetDensity() const
      {
         return getDensity<NamedDensityUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Density.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   /// @tparam Length3 Denominator Length3 type
   template<typename Mass1,typename Length1, typename Length2, typename Length3>
   requires IsDensityUnits<Mass1,Length1, Length2, Length3>
   Density(Mass1,Length1, Length2, Length3) -> Density<Mass1,Length1, Length2, Length3>;

   /// @brief Template deduction guide for Density.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   /// @tparam Length3 Denominator Length3 type
   template<typename NamedDensity>
   requires IsNamedDensityUnit<NamedDensity>
   Density(NamedDensity) -> Density<NamedDensity>;

   /// @brief Template deduction guide for Density.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Denominator Length1 type
   /// @tparam Length2 Denominator Length2 type
   /// @tparam Length3 Denominator Length3 type
   template<typename Mass1,typename Length1, typename Length2, typename Length3>
   requires IsDensityUnits<Mass1,Length1, Length2, Length3>
   Density(BaseDimension<std::tuple<Mass1>, std::tuple<Length1, Length2, Length3>>) -> Density<Mass1,Length1, Length2, Length3>;

}

#endif // STATIC_DIMENSION_DENSITY_IMPL_H