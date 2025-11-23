// ========= Prime-factor introspection (constexpr, no runtime) =========
#include "DimensionTest.h"
using namespace dimension;

// Helper: size of a tuple type
template <class T> struct tuple_size_helper;
template <class... Ts> struct tuple_size_helper<std::tuple<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};
template <class T> inline constexpr std::size_t tuple_size_v = tuple_size_helper<T>::value;

// Helper: get exponent (std::ratio) for a given prime P from a ratio_exponent tuple; default 0/1 if absent.
template <int P, class Tup> struct prime_exp;
template <int P> struct prime_exp<P, std::tuple<>> { using type = std::ratio<0,1>; };

template <int P, class RE, class... Rest>
struct prime_exp<P, std::tuple<RE, Rest...>> {
    // ratio_exponent< ratio<base_num, base_den>, ratio<exp_num, exp_den> >
    using this_match = std::conditional_t<(RE::ratio::num == P && RE::ratio::den == 1), typename RE::exponent, std::ratio<0,1>>;
    using rest_match = typename prime_exp<P, std::tuple<Rest...>>::type;
    using type = std::conditional_t<(this_match::num != 0), this_match, rest_match>;
};
template <int P, class Tup>
using prime_exp_t = typename prime_exp<P, Tup>::type;

// Convenience: extract the result type of an expression (no evaluation)
template <class L, class R>
using expr_div_t = decltype(std::declval<L>() / std::declval<R>());
/*
// --- inch ↔ meter: 1 in = 127/5000 m  => (in/m) = 127 / (2^3 * 5^4) ---
TEST(PrimeFactors, InchPerMeter_TypeCarriesExpectedPrimes)
{
    using Q  = expr_div_t<length<inches>, speed<meters, seconds>>;   // timespan scaled by (in/m)
    using RX = typename Q::ratio_exponents;
    static_assert(tuple_size_v<RX> == 3, "Expected {2,5,127} in inch/m");
    using E2   = prime_exp_t<2,   RX>;
    using E5   = prime_exp_t<5,   RX>;
    using E127 = prime_exp_t<127, RX>;
    static_assert(E2::num == -3 && E2::den == 1,   "2 exponent should be -3");
    static_assert(E5::num == -4 && E5::den == 1,   "5 exponent should be -4");
    static_assert(E127::num ==  1 && E127::den == 1, "127 exponent should be +1");
    SUCCEED();
}

// --- foot ↔ meter: 1 ft = 381/1250 m => (ft/m) = (3 * 127) / (2 * 5^4) ---
TEST(PrimeFactors, FootPerMeter_TypeCarriesExpectedPrimes)
{
    using Q  = expr_div_t<length<feet>, speed<meters, seconds>>;     // timespan scaled by (ft/m)
    using RX = typename Q::ratios;
    static_assert(tuple_size_v<RX> == 4, "Expected {2,3,5,127} in foot/m");
    using E2   = prime_exp_t<2,   RX>;
    using E3   = prime_exp_t<3,   RX>;
    using E5   = prime_exp_t<5,   RX>;
    using E127 = prime_exp_t<127, RX>;
    static_assert(E2::num == -1 && E2::den == 1,   "2 exponent should be -1");
    static_assert(E3::num ==  1 && E3::den == 1,   "3 exponent should be +1");
    static_assert(E5::num == -4 && E5::den == 1,   "5 exponent should be -4");
    static_assert(E127::num == 1 && E127::den == 1, "127 exponent should be +1");
    SUCCEED();
}
*/