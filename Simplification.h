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

   //template <typename...>
   template<typename NumTuple, typename DenTuple>
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
   template<typename NumTuple, typename DenTuple>
   struct SimplifierInterface {};

   template<typename NumTuple, typename DenTuple>
   struct TimeUnitSimplifier;

   template<typename NumTuple, typename DenTuple>
   struct LengthUnitSimplifier;










   

   // Base case: empty parameter pack
   template<typename...>
   struct RemoveOneInstance {
      using type = std::tuple<>;
   };

   // Recursive case: remove one instance of T from the parameter pack
   template<typename T, typename Head, typename... Tail>
   struct RemoveOneInstance<T, std::tuple<Head, Tail...>> {
      using type = std::conditional_t<std::is_same_v<T, Head>,
         std::tuple<Tail...>,
         //typename RemoveOneInstance<T, Tail...>::type,
         decltype(std::tuple_cat(std::tuple<Head>(), typename RemoveOneInstance<T, std::tuple<Tail...>>::type()))>;
   };
   
   

   template <typename T, typename Tuple>
   struct has_type;

   template <typename T, typename... Us>
   struct has_type<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...> {};


   //template <class T0, class... Ts>
   //constexpr bool is_one_of = std::disjunction_v<std::is_same<T0, Ts>...>;

   //template<typename T1, typename... Ts>
   //constexpr bool is_one_of() noexcept {
   //   return (std::is_same_v<T1, Ts> || ...);
   //}



   // MINE
   

   
   template<typename T, typename ...>
   struct tuple_diff;

   template<typename ... DenTuple>
   struct tuple_diff<std::tuple<>, std::tuple<DenTuple...>>
   {
      using type = std::tuple<>;
   };

   template<typename T, typename ... RestNumTypes, typename ... DenTuple>
   struct tuple_diff<std::tuple<T, RestNumTypes...>, std::tuple<DenTuple...>>
   {
      using type =
         std::conditional_t 
         <
            has_type<T, std::tuple<DenTuple...>>(),


            std::tuple
            <
               tuple_diff
               <
                  std::tuple<RestNumTypes...>, RemoveOneInstance<T, std::tuple<DenTuple...>>::type
               >::type
            >,


            std::tuple
            <
               T, 
               tuple_diff
               <
                  std::tuple<RestNumTypes...>, std::tuple<DenTuple...>
               >::type
            >
         
         
         
         >
      ;
   };
   
   











   /*
   // Helper metafunction to check if a type is present in a tuple
   template<typename T, typename Tuple>
   struct tuple_contains;

   template<typename T, typename... Ts>
   struct tuple_contains<T, std::tuple<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

   // Helper metafunction to remove a type from a tuple
   template<typename T, typename Tuple>
   struct tuple_remove;

   template<typename T, typename... Ts>
   struct tuple_remove<T, std::tuple<Ts...>> {
      using type = std::tuple<Ts...>;
   };

   template<typename T, typename U, typename... Ts>
   struct tuple_remove<T, std::tuple<U, Ts...>> {
      using type = typename tuple_remove<T, std::tuple<Ts...>>::type;
   };

   template<typename T>
   struct tuple_remove<T, std::tuple<>> {
      using type = std::tuple<>;
   };

   // Helper metafunction to compute the set difference of two tuples
   template<typename Tuple1, typename Tuple2>
   struct tuple_diff;

   template<typename... Ts, typename... Us>
   struct tuple_diff<std::tuple<Ts...>, std::tuple<Us...>> {
      template<typename... Args>
      struct remove_types;

      template<typename... Args>
      using remove_types_t = typename remove_types<Args...>::type;

      template<typename T, typename... Args>
      struct remove_types<T, Args...> {
         using type = typename std::conditional<tuple_contains<T, std::tuple<Us...>>::value,
            remove_types_t<Args...>,
            typename tuple_remove<T, std::tuple<Args...>>::type>::type;
      };

      template<typename T>
      struct remove_types<T> {
         using type = typename std::conditional<tuple_contains<T, std::tuple<Us...>>::value,
            std::tuple<>,
            std::tuple<T>>::type;
      };

      using type = remove_types_t<Ts...>;
   };

   */






   /*
   template <class...> class set_diff;

   template <class... A, class... B>
   struct set_diff<std::tuple<A...>, std::tuple<B...>>
   {
      template <class J, class... Js>
      constexpr static bool exists_in = std::disjunction_v<std::is_same<J, Js>...>;

      using type = decltype(std::tuple_cat(std::declval<
         std::conditional_t<exists_in<A, B...>, std::tuple<>, std::tuple<A>>>()...));
   };
   */




   /*
   //template<typename NumTuple, typename DenTuple>
   //struct AllUnitSimplifier;

   template<typename NumTuple, typename DenTuple>
   struct AllUnitSimplifier
   {
      //using type = std::tuple<tuple_cat_t<typename LengthUnitSimplifier<NumTuple, DenTuple>::type, typename TimeUnitSimplifier<NumTuple, DenTuple>::type>, std::tuple<>>;
      using type = std::tuple<set_diff<NumTuple, DenTuple>::type, set_diff<DenTuple, NumTuple>::type>;
   };
   */













   /*

   /// @brief Concatenate the type outputs of all simplifiers
   /// @tparam Ts The types to simplify
   /// @todo Find a way to attach user-defined simplifiers to this
   ///    while maintaining compile-time type safety.
   ///    This is high priority, but has proven difficult.
   template<typename NumTuple, typename DenTuple>
   struct AllUnitSimplifier
   {
      using type = std::tuple<tuple_cat_t<typename LengthUnitSimplifier<NumTuple, DenTuple>::type, typename TimeUnitSimplifier<NumTuple, DenTuple>::type>, std::tuple<>>;
   };
   */

   // Simplify function for BaseDimension
   // If problems arise, consider specifying return type
   /// @brief Function to construct a simplified BaseDimension from a given BaseDimension
   /// @details Call AllUnitSimplifier on the given BaseDimension to simplify the units.
   ///    The resulting BaseDimension has simplified units (i.e. no instances of the same
   ///    type in both the numerator and denominator). The value will be adjusted accordingly.
   /// @tparam Ts The template parameters of the input BaseDimension
   /// @param[in] obj The BaseDimension object to simplify
   /// @return The new BaseDimension object with simplified units
   template<typename NumTuple, typename DenTuple>
   auto SimplifyBaseDimension(const BaseDimension<NumTuple, DenTuple>& obj)// -> decltype(TupleToBaseDimension(typename AllUnitSimplifier<Ts...>::type{}, std::index_sequence_for<Ts...>{}, std::declval<BaseDimension<Ts...>>()))
   {
      using TupleType = typename AllUnitSimplifier<NumTuple, DenTuple>::type;
      return TupleToBaseDimension(std::tuple_element_t<0,TupleType>{}, std::tuple_element_t<1, TupleType>{}, obj);
   }

}

#endif //SIMPLIFICATION_H