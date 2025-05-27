#ifndef STATIC_DIMENSION_Mass_IMPL_H
#define STATIC_DIMENSION_Mass_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Mass type tag.
   struct MassType {};

   /// @brief Represents the primary unit for Mass.
   struct Grams;

   using PrimaryMass = Grams;

   /// @brief Concept to check if a type is a valid Mass unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsMassUnit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, MassType>;//IsNonQuantityUnitDimension<T, MassType> || IsQuantityUnitDimension<T, MassType>;

   /// @brief Base class for Mass units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct MassUnit : public BaseUnit<Unit, Name, Abbreviation, "Mass", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = MassType;

      /// @brief The primary unit type for this dimension.
      using Primary = PrimaryMass;
   };

   /// @brief Trait to check if a type is a Mass dimension.
   /// @tparam T The type to check.
   //template<typename T>
   //struct is_Mass : std::false_type {};

   //template<typename Dim>
   //struct is_Mass_Impl
   //{
   //   constexpr bool value = IsMassUnit<std::tuple_element_t<0, decltype(FullSimplify(Dim{}))>>;
   //};


   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   //template<typename T>
   //struct is_Mass<BaseDimension<T>> : std::bool_constant<IsMassUnit<T>> {};
   template<typename Dim>
   struct is_Mass : std::bool_constant<IsMassUnit<typename std::tuple_element_t<0, typename decltype(FullSimplify(Dim{}))::units>::unit>> {};

   /// @brief Helper variable template for is_Mass.
   /// @tparam T The type to check.
   template<typename T>
   constexpr bool is_Mass_v = is_Mass<T>::value;

   /// @brief Concept to verify if a type is a valid Mass type.
   /// @tparam T The type to check.
   template<typename T>
   concept Mass_type = is_Mass_v<T>;

   /// @brief Retrieves the value of a Mass object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The Mass object.
   /// @return The value in the specified unit.
   template<IsMassUnit T>
   constexpr PrecisionType get_mass_as(/*Mass_type*/ auto obj)
   {
      return get_dimension_as<UnitExponent<T>>(obj);
   }

   template<IsMassUnit Unit = PrimaryMass, bool isQuantity = false>
   class Mass;

   /// @brief Represents a dimension type for Mass.
   /// @tparam Unit The primary unit type.
   template<IsMassUnit Unit>
   class Mass<Unit, false> : public BaseDimension<UnitExponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Mass() : BaseDimension<UnitExponent<Unit>>::BaseDimension(0.0) {}

      /// @brief Constructs a Mass object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Mass(double val) : BaseDimension<UnitExponent<Unit>>::BaseDimension(val) {}

      /// @brief Constructs a Mass object from another BaseDimension.
      /// @tparam Ts The units of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<BaseDimension<UnitExponent<Unit>>, BaseDimension<Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Mass(const BaseDimension<Ts...>& base) : BaseDimension<UnitExponent<Unit>>::BaseDimension(get_dimension_as<UnitExponent<Unit>>(base)) {}
   };

   /// @brief Specialization for is_Mass trait for Mass types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Mass<Mass<T>> : std::bool_constant<IsMassUnit<T>> {};

   /// @brief Deduction guide for Mass constructor.
   /// @tparam T The unit type.
   //template<IsMassUnit T>
   //Mass(T) -> Mass<T>;

   /// @brief Deduction guide for Mass constructor with BaseDimension.
   /// @tparam MassUnit The unit type.
   //template<IsMassUnit MassUnit>
   //Mass(BaseDimension<UnitExponent<MassUnit>>) -> Mass<MassUnit>;

   /// @brief Deduction guide for Mass constructor with BaseDimension.
   /// @tparam MassUnit The unit type.
   template<Mass_type Dim>
   Mass(Dim) -> Mass<DimExtractor<MassType, Dim>>;
}

#endif // STATIC_DIMENSION_Mass_IMPL_H