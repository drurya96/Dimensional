#ifndef DIMENSION_UNIT_AHHHHH_H
#define DIMENSION_UNIT_AHHHHH_H

#include <string_view>
#include <tuple>
#include <type_traits>

#include "unit_exponent.h"

namespace dimension {

// Your dimension tags
//struct length_dimension {};
//struct time_dimension   {};
//struct mass_dimension   {};
//struct energy_dimension {};
// ...

// Primary trait (users specialize this *once* per unit)
template<class Unit>
struct unit_traits; // no definition; must be specialized

// ---- Identity accessors
template<class U>
using unit_dimension_t = typename unit_traits<U>::dimension;

template<class U>
inline constexpr std::string_view unit_name_v = unit_traits<U>::name;

template<class U>
inline constexpr std::string_view unit_abbr_v = unit_traits<U>::abbr;

template<class U>
inline constexpr int unit_id_v = unit_traits<U>::id;

// ---- Composition accessor
// If the specialization defines `using units = std::tuple<...>;` we use it;
// otherwise we treat U as fundamental: tuple<unit_exponent<U,1,1>>.

template<class U, class = void>
struct unit_units { using type = std::tuple<unit_exponent<U, 1, 1>>; };

template<class U>
struct unit_units<U, std::void_t<typename unit_traits<U>::units>> {
  using type = typename unit_traits<U>::units;
};

template<class U>
using unit_units_t = typename unit_units<U>::type;

template<class T, class = void>
struct registered_unit_impl : std::false_type {};

template<class T>
struct registered_unit_impl<T, std::void_t<typename unit_traits<T>::units>>
    : std::true_type {};

template<class T>
inline constexpr bool registered_unit = registered_unit_impl<T>::value;

// (Optional) helper: is U fundamental by composition?
template<class U>
inline constexpr bool is_fundamental_unit_v = []{
  using T = unit_units_t<U>;
  if constexpr (std::tuple_size_v<T> != 1) return false;
  else {
    using E = std::tuple_element_t<0, T>;
    return std::is_same_v<typename E::unit, U> &&
           (E::exponent::num == 1) && (E::exponent::den == 1);
  }
}();

template<class T>
concept is_unit = requires {
    typename unit_units_t<T>; // must be decomposable into exponents
};

} // namespace dimension


#endif // DIMENSION_UNIT_AHHHHH_H