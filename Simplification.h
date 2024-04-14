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
   template <typename... Ts>
   struct SimplifierInterface {};


   // ==========================================
   // Time Simplification

   // Simplify function for TimeUnit types

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



   // ==========================================
   // Length Simplification

   // Simplify function for LengthUnit types
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

   // =========================================
   // Generic Unit Simplification
   // This will need updates to handle user-defined units
   // Consider using some equavalent to lambda/functor registration
   // Must be compile-time aware


   template <typename ... Ts>
   struct AllUnitSimplifier
   {
      using type = tuple_cat_t<typename LengthUnitSimplifier<Ts...>::type, typename TimeUnitSimplifier<Ts...>::type>;
   };

   // Simplify function for BaseDimension
   // If problems arise, consider specifying return type
   template<typename... Ts>
   auto SimplifyBaseDimension(const BaseDimension<Ts...>& obj)// -> decltype(TupleToBaseDimension(typename AllUnitSimplifier<Ts...>::type{}, std::index_sequence_for<Ts...>{}, std::declval<BaseDimension<Ts...>>()))
   {
      using TupleType = typename AllUnitSimplifier<Ts...>::type;
      return TupleToBaseDimension(TupleType{}, std::index_sequence_for<Ts...>{}, obj);
   }

}

#endif //SIMPLIFICATION_H