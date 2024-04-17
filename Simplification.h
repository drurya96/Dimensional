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

   /// @brief Interface for simplifer.
   /// @details Interface each simplifier must adhere to.
   ///    This has no current value-add, but may be useful for
   ///    type erasure when attempt to resolve the below @todo
   template <typename... Ts>
   struct SimplifierInterface {};

   template <typename...>
   struct TimeUnitSimplifier;

   template <typename...>
   struct LengthUnitSimplifier;

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
      return TupleToBaseDimension(TupleType{}, obj);
   }

}

#endif //SIMPLIFICATION_H