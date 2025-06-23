#ifndef DIMENSION_GENERIC_UTILS_H
#define DIMENSION_GENERIC_UTILS_H

#include <tuple>

namespace dimension
{
   template<typename Tuple, typename State, template<typename, typename> class Func>
   struct fold_over_tuple_with_state;
   
   template<typename State, template<typename, typename> class Func>
   struct fold_over_tuple_with_state<std::tuple<>, State, Func> {
       using type = State;
   };
   
   template<typename T, typename... Ts, typename State, template<typename, typename> class Func>
   struct fold_over_tuple_with_state<std::tuple<T, Ts...>, State, Func> {
       using next = typename Func<State, T>::type;
       using type = typename fold_over_tuple_with_state<std::tuple<Ts...>, next, Func>::type;
   };

   template<typename Tuple, template<typename> class Predicate>
   struct filter_tuple;

   template<template<typename> class Predicate, typename... Ts>
   struct filter_tuple<std::tuple<Ts...>, Predicate> {
      using type = tuple_cat_t<
         std::conditional_t<Predicate<Ts>::value, std::tuple<Ts>, std::tuple<>>...
      >;
   };


   template<typename Tuple, typename Func>
   constexpr double fold_tuple(double value, Func func)
   {
      return std::apply(
         [&](auto... units) {
               return (func.template operator()<decltype(units)>(value), ...);
         },
         Tuple{}
      );
   }

/*
   template<typename Tuple>
   struct expand_tuple;
   
   template<typename... Ts>
   struct expand_tuple<std::tuple<Ts...>> {
       template<template<typename...> typename F>
       using into = F<Ts...>;
   };
*/

   template<typename Tuple, typename F>
   constexpr auto call_unpack(F&& f) {
      return [&f]<typename... Units>(std::tuple<Units...>*) {
         return f.template operator()<Units...>();
      }(static_cast<Tuple*>(nullptr));
   }
}

#endif // DIMENSION_GENERIC_UTILS_H