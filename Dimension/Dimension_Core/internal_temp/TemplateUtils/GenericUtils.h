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

   template<typename Tuple, template<typename> class Predicate>
   struct filter_tuple;

   template<template<typename> class Predicate, typename... Ts>
   struct filter_tuple<std::tuple<Ts...>, Predicate> {
      using type = tuple_cat_t<
         std::conditional_t<Predicate<Ts>::value, std::tuple<Ts>, std::tuple<>>...
      >;
   };

   template<typename Tuple, typename F>
   constexpr auto call_unpack(F&& f) {
      return [&f]<typename... Units>(std::tuple<Units...>*) {
         return f.template operator()<Units...>();
      }(static_cast<Tuple*>(nullptr));
   }

   // Primary template – never used directly
   template<typename Tuple>
   struct apply;

   // Partial specialisation that actually contains the parameter pack
   template<typename... Ts>
   struct apply<std::tuple<Ts...>>
   {
      // ─── 1.  Re-use the pack to build a *type* ──────────────────────────
      template<template<typename...> class Template>
      using to = Template<Ts...>;

      // ─── 2.  Invoke a callable that itself needs the pack ──────────────
      template<typename F, typename... Args>
      static constexpr decltype(auto) call(F&& f, Args&&... args)
      {
         // F must be a generic callable with a templated operator():
         //   auto f = []<typename... Us>(Args...){ /*…*/ };
         return std::forward<F>(f)
                  .template operator()<Ts...>(std::forward<Args>(args)...);
      }
   };

   template<typename... Us>
   constexpr void ignore_unused(Us&&...) noexcept {}

   template<template<class> class Pred, class... Ts>
   using filter_pack_tuple_t =
      decltype(std::tuple_cat(
         std::conditional_t<Pred<Ts>::value,
                              std::tuple<std::remove_cvref_t<Ts>>,
                              std::tuple<>>{}...
      ));

}

#endif // DIMENSIONAL_GENERIC_UTILS_H