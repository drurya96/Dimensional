#ifndef DIMENSIONAL_TUPLE_HANDLING_H
#define DIMENSIONAL_TUPLE_HANDLING_H

#include <tuple> // For std::tuple and related functions
#include <utility> // For std::declval

namespace dimension
{
   /// @brief Convenience alias for retrieving the type of a tuple of types
   /// @tparam Ts Parameter pack to types to concatenate
   template<typename...Ts>
   using tuple_cat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

   template<class Accum, class New>
   using append_t = tuple_cat_t<Accum, std::tuple<New>>;

   // Primary template: defaults to false
   template<typename T>
   struct is_tuple : std::false_type {};

   // Specialization for std::tuple<...>
   template<typename... Ts>
   struct is_tuple<std::tuple<Ts...>> : std::true_type {};

   // Convenience variable template
   template<typename T>
   inline constexpr bool is_tuple_v = is_tuple<T>::value;


} // end Dimension

#endif // DIMENSIONAL_TUPLE_HANDLING_H
