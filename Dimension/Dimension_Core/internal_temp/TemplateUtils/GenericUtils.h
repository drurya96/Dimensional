#ifndef DIMENSIONAL_GENERIC_UTILS_H
#define DIMENSIONAL_GENERIC_UTILS_H

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

   template<class Tuple, class State, template<class,class> class Func>
   using fold_over_tuple_with_state_t =
      typename fold_over_tuple_with_state<Tuple, State, Func>::type;

   template<typename Tuple, typename F>
   constexpr auto call_unpack(F&& f) {
      return [&f]<typename... Units>(std::tuple<Units...>*) {
         return f.template operator()<Units...>();
      }(static_cast<Tuple*>(nullptr));
   }

   template<typename... Us>
   constexpr void ignore_unused(Us&&...) noexcept {}

   template<template<class> class Pred, class... Ts>
   using filter_pack_tuple_t =
      decltype(std::tuple_cat(
         std::conditional_t<Pred<Ts>::value,
                              std::tuple<std::remove_cvref_t<Ts>>,
                              std::tuple<>>{}...
      ));

   template<template<class> class Pred, class Tuple>
   struct filter_tuple;

   template<template<class> class Pred, class... Ts>
   struct filter_tuple<Pred, std::tuple<Ts...>> {
      using type = filter_pack_tuple_t<Pred, Ts...>; // uses remove_cvref_t
   };

   template<template<class> class Pred, class Tuple>
   using filter_tuple_t = typename filter_tuple<Pred, Tuple>::type;



}

#endif // DIMENSIONAL_GENERIC_UTILS_H