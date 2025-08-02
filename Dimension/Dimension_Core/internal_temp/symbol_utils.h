#ifndef DIMENSIONAL_SYMBOL_UTILS_H
#define DIMENSIONAL_SYMBOL_UTILS_H

#include <concepts>
#include <ratio>
#include <type_traits>

namespace dimension
{
   struct symbol{};

   template<typename T>
   concept is_symbol = std::derived_from<std::remove_cvref_t<T>, symbol>;

   template<is_symbol sym, int Num, int Den>
   struct symbol_exponent
   {
      using symbol = sym;
      using exponent = std::ratio<Num, Den>;
   };

   template<typename> inline constexpr bool is_symbol_exponent_v = false;

   template<typename Sym, int Num, int Den>
   inline constexpr bool
   is_symbol_exponent_v<symbol_exponent<Sym, Num, Den>> = true;

   template<typename T>
   concept is_symbol_exponent = is_symbol_exponent_v<std::remove_cvref_t<T>>;

}

#endif //DIMENSIONAL_SYMBOL_UTILS_H
