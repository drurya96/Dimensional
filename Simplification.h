#ifndef SIMPLIFICATION_H
#define SIMPLIFICATION_H

#include "DimensionUtilities.h"
#include <typeindex>

namespace Dimension{

   // Forward declarations

   template <typename...>
   class TimeUnit;

   template <typename...>
   class LengthUnit;

   template <typename...>
   class BaseUnit;

   template <typename...>
   class BaseDimension;

   class Inverse;

   // ==========================================
   // Simplification logic
   // This needs to be moved around
   // Some portion should live with the Time class

   // Simplify function for any Simplifier type
   /// @brief 
   template <typename... Ts>
   struct SimplifierInterface {};

   /// @brief Simplify TimeUnit types
   /// @details Given a set of types, simplify all TimeUnit types.
   ///    This means for each pair of TimeUnits (one templated on Inverse
   ///    and the other not), both will be removed from the type list,
   ///    and the value will be adjusted accordingly
   /// @tparam Ts The typelist to simplify. Note in the current implementation
   ///    this list can contain any BaseUnits, not just TimeUnits.
   /// @todo Move this logic to the Time declaration. Still trying to find
   ///    a solution that maintains compile-time type safety.
   template <typename... Ts>
   struct TimeUnitSimplifier : SimplifierInterface<Ts...> {

      static constexpr size_t TimeCount = count_type<TimeUnit<>, Ts...>();
      static constexpr size_t InverseTimeCount = count_type<TimeUnit<Inverse>, Ts...>();

      using type = std::conditional_t<(TimeCount > InverseTimeCount),
         typename Repeat<TimeCount - InverseTimeCount, TimeUnit<>>::type,
         std::conditional_t<(TimeCount < InverseTimeCount),
         typename Repeat<InverseTimeCount - TimeCount, TimeUnit<Inverse>>::type,
         std::tuple<>>>;
   };

   /// @brief Simplify LengthUnit types
   /// @details Given a set of types, simplify all LengthUnit types.
   ///    This means for each pair of LengthUnits (one templated on Inverse
   ///    and the other not), both will be removed from the type list,
   ///    and the value will be adjusted accordingly
   /// @tparam Ts The typelist to simplify. Note in the current implementation
   ///    this list can contain any BaseUnits, not just LengthUnits.
   /// @todo Move this logic to the Length declaration. Still trying to find
   ///    a solution that maintains compile-Length type safety.
   template <typename... Ts>
   struct LengthUnitSimplifier : SimplifierInterface<Ts...> {

      static constexpr size_t LengthCount = count_type<LengthUnit<>, Ts...>();
      static constexpr size_t InverseLengthCount = count_type<LengthUnit<Inverse>, Ts...>();

      using type = std::conditional_t<(LengthCount > InverseLengthCount),
         typename Repeat<LengthCount - InverseLengthCount, LengthUnit<>>::type,
         std::conditional_t<(LengthCount < InverseLengthCount),
         typename Repeat<InverseLengthCount - LengthCount, LengthUnit<Inverse>>::type,
         std::tuple<>>>;

   };

   /// @brief Concatenate the type outputs of all simplifiers
   /// @tparam Ts The types to simplify
   /// @todo Find a way to attach user-defined simplifiers to this
   ///    while maintaining compile-time type safety.
   ///    This is high priority, but has proven difficult.
   template <typename ... Ts>
   struct AllUnitSimplifier
   {
      using type = tuple_cat_t<typename LengthUnitSimplifier<Ts...>::type, typename TimeUnitSimplifier<Ts...>::type>;
   };

   // Simplify function for BaseDimension
   // If problems arise, consider specifying return type
   /// @brief Function to construct a simplified BaseDimension from a given BaseDimension
   /// @details Call AllUnitSimplifier on the given BaseDimension to simplify the units.
   ///    The resulting BaseDimension has simplified units (i.e. no instances of the same
   ///    type in both the numerator and denominator). The value will be adjusted accordingly.
   /// @tparam Ts The template parameters of the input BaseDimension
   /// @param[in] obj The BaseDimension object to simplify
   /// @return The new BaseDimension object with simplified units
   template<typename... Ts>
   auto SimplifyBaseDimension(const BaseDimension<Ts...>& obj)// -> decltype(TupleToBaseDimension(typename AllUnitSimplifier<Ts...>::type{}, std::index_sequence_for<Ts...>{}, std::declval<BaseDimension<Ts...>>()))
   {
      using TupleType = typename AllUnitSimplifier<Ts...>::type;
      return TupleToBaseDimension(TupleType{}, std::index_sequence_for<Ts...>{}, obj);
   }

}

#endif //SIMPLIFICATION_H