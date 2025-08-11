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

} // end Dimension

#endif // DIMENSIONAL_TUPLE_HANDLING_H
