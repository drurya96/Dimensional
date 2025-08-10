#ifndef DIMENSIONAL_CORE_SYMBOL_H
#define DIMENSIONAL_CORE_SYMBOL_H

#include <concepts>
#include <type_traits>

namespace dimension
{
   struct symbol{};

   template<typename T>
   concept is_symbol = std::derived_from<std::remove_cvref_t<T>, symbol>;

   // TODO: Consider allowing a std::ratio API in addition to integrals
   template<is_symbol sym, int Num, int Den>
   struct symbol_exponent;

   template<typename> inline constexpr bool is_symbol_exponent_v = false;

   template<typename Sym, int Num, int Den>
   inline constexpr bool
   is_symbol_exponent_v<symbol_exponent<Sym, Num, Den>> = true;

   template<typename T>
   concept is_symbol_exponent = is_symbol_exponent_v<std::remove_cvref_t<T>>;
}

#endif // DIMENSIONAL_CORE_SYMBOL_H