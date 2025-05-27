#ifndef STATIC_DIMENSION_Temperature_IMPL_H
#define STATIC_DIMENSION_Temperature_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Temperature type tag.
   struct TemperatureType {};

   /// @brief Represents the primary unit for Temperature.
   struct Kelvin;

   using PrimaryTemperature = Kelvin;

   /// @brief Concept to check if a type is a valid Temperature unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsTemperatureUnit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, TemperatureType>;//IsNonQuantityUnitDimension<T, TemperatureType> || IsQuantityUnitDimension<T, TemperatureType>;

   /// @brief Base class for Temperature units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct TemperatureUnit : public BaseUnit<Unit, Name, Abbreviation, "Temperature", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = TemperatureType;

      /// @brief The primary unit type for this dimension.
      using Primary = PrimaryTemperature;
   };

   /// @brief Trait to check if a type is a Temperature dimension.
   /// @tparam T The type to check.
   //template<typename T>
   //struct is_Temperature : std::false_type {};

   //template<typename Dim>
   //struct is_Temperature_Impl
   //{
   //   constexpr bool value = IsTemperatureUnit<std::tuple_element_t<0, decltype(FullSimplify(Dim{}))>>;
   //};


   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   //template<typename T>
   //struct is_Temperature<BaseDimension<T>> : std::bool_constant<IsTemperatureUnit<T>> {};
   template<typename Dim>
   struct is_Temperature : std::bool_constant<IsTemperatureUnit<typename std::tuple_element_t<0, typename decltype(FullSimplify(Dim{}))::units>::unit>> {};

   /// @brief Helper variable template for is_Temperature.
   /// @tparam T The type to check.
   template<typename T>
   constexpr bool is_Temperature_v = is_Temperature<T>::value;

   /// @brief Concept to verify if a type is a valid Temperature type.
   /// @tparam T The type to check.
   template<typename T>
   concept Temperature_type = is_Temperature_v<T>;

   /// @brief Retrieves the value of a Temperature object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The Temperature object.
   /// @return The value in the specified unit.
   template<IsTemperatureUnit T>
   constexpr PrecisionType get_temperature_as(/*Temperature_type*/ auto obj)
   {
      return get_dimension_as<UnitExponent<T>>(obj);
   }

   template<IsTemperatureUnit Unit, bool isQuantity>
   class Temperature;

   /// @brief Represents a dimension type for Temperature.
   /// @tparam Unit The primary unit type.
   template<IsTemperatureUnit Unit>
   class Temperature<Unit, delta> : public BaseDimension<UnitExponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Temperature() : BaseDimension<UnitExponent<Unit>>::BaseDimension(0.0) {}

      /// @brief Constructs a Temperature object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Temperature(double val) : BaseDimension<UnitExponent<Unit>>::BaseDimension(val) {}

      /// @brief Constructs a Temperature object from another BaseDimension.
      /// @tparam Ts The units of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<BaseDimension<UnitExponent<Unit>>, BaseDimension<Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Temperature(const BaseDimension<Ts...>& base) : BaseDimension<UnitExponent<Unit>>::BaseDimension(get_dimension_as<UnitExponent<Unit>>(base)) {}
   };

   template<IsTemperatureUnit Unit>
   class Temperature<Unit, quantity>
   {
   public:
      // Include quantity specific stuff
      constexpr Temperature() = delete;
      constexpr explicit Temperature(double val): scalar(val){}

      double GetRaw() { return scalar; }

   private:
      double scalar;
   };



   template<typename T, typename U>
   Temperature<T, delta> operator-(const Temperature<T, quantity>& lhs, const Temperature<U, quantity>& rhs) {
      return Temperature<T, delta>(lhs.GetRaw() - rhs.GetRaw());
   }

   template<typename T, typename U>
   Temperature<T, quantity> operator-(const Temperature<T, quantity>& lhs, const Temperature<U, delta>& rhs) {
      return Temperature<T, quantity>(lhs.GetRaw() - rhs.GetRaw());
   }

   template<typename T, typename U>
   Temperature<T, quantity> operator+(const Temperature<T, quantity>& lhs, const Temperature<U, delta>& rhs) {
      return Temperature<T, quantity>(lhs.GetRaw() + rhs.GetRaw());
   }











   /// @brief Specialization for is_Temperature trait for Temperature types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Temperature<Temperature<T>> : std::bool_constant<IsTemperatureUnit<T>> {};

   /// @brief Deduction guide for Temperature constructor.
   /// @tparam T The unit type.
   //template<IsTemperatureUnit T>
   //Temperature(T) -> Temperature<T>;

   /// @brief Deduction guide for Temperature constructor with BaseDimension.
   /// @tparam TemperatureUnit The unit type.
   //template<IsTemperatureUnit TemperatureUnit>
   //Temperature(BaseDimension<UnitExponent<TemperatureUnit>>) -> Temperature<TemperatureUnit>;

   /// @brief Deduction guide for Temperature constructor with BaseDimension.
   /// @tparam TemperatureUnit The unit type.
   template<Temperature_type Dim>
   Temperature(Dim) -> Temperature<DimExtractor<TemperatureType, Dim>>;
}

#endif // STATIC_DIMENSION_Temperature_IMPL_H