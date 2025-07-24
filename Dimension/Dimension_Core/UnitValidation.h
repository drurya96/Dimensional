#ifndef DIMENSION_UNIT_VALIDATION_H
#define DIMENSION_UNIT_VALIDATION_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction

#include "Conversion.h" // TODO: Remove this by moving PrimaryConvertible and friends to a separate header

namespace dimension
{

   static constexpr bool delta = false;
   static constexpr bool quantity = true;

   // TODO: No idea where this should live...
   struct qname_builder
   {
         static constexpr StringLiteral<2> delim = "|";

         /* base case : one unit */
         template<typename UE>
         static constexpr auto build()
         {
            return UE::qualifiedName;
         }

         /* recursive : First | build(Rest…) */
         template<typename First, typename Second, typename... Rest>
         static constexpr auto build()
         {
            return concat(
                        concat(First::qualifiedName, delim),
                        build<Second, Rest...>());
         }
   };

   // ──────────────────────────────────────────────────────────────
   //  2.  Tuple interface
   //      make_units_qname<Tuple>()
   //      where Tuple = std::tuple<unit_exponent<…>, …>
   // ──────────────────────────────────────────────────────────────
   template<typename Tuple, std::size_t... Is>
   constexpr auto make_units_qname_impl(std::index_sequence<Is...>)
   {
      return qname_builder::build<
                  std::tuple_element_t<Is, Tuple>...>();
   }

   template<typename Tuple>
   constexpr auto make_units_qname_tuple()
   {
      static_assert(std::tuple_size_v<Tuple> > 0,
                     "make_units_qname<Tuple> : Tuple must contain units");
      return make_units_qname_impl<Tuple>(
                  std::make_index_sequence<std::tuple_size_v<Tuple>>{});
   }

   // TODO: This should probably live inside StringLiteral...
   //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
   //  helpers that manufacture StringLiteral<K> directly
   //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
   template<char C>
   constexpr auto lit()                      // "x"
   {
      return dimension::StringLiteral<2>({ C, '\0' });
   }

   /* decimal digit sequence of a positive integer, in reverse order */
   template<std::size_t N>
   constexpr auto digits_rev()
   {
      if constexpr (N < 10)
         return lit<'0' + N>();
      else
      {
         constexpr auto tail = digits_rev<N / 10>();
         return dimension::concat(lit<'0' + (N % 10)>(), tail);
      }
   }

   /* forward-order decimal of a positive integer */
   template<std::size_t N>
   constexpr auto digits_fwd()
   {
      constexpr auto rev = digits_rev<N>();
      // reverse the character array except null
      constexpr std::size_t len = rev.size - 1;
      std::array<char, len + 1> tmp{};
      for (std::size_t i = 0; i < len; ++i)
         tmp[i] = rev.value[len - 1 - i];
      return dimension::StringLiteral<len + 1>(tmp);
   }

   /* signed integer to StringLiteral */
   template<int V>
   constexpr auto int_literal()
   {
      if constexpr (V < 0)
         return dimension::concat(lit<'-'>(), digits_fwd<-V>());
      else
         return digits_fwd<V>();
   }

   /* ratio to StringLiteral  (Num / Den) */
   template<int Num, int Den>
   constexpr auto ratio_literal()
   {
      if constexpr (Den == 1)
         return int_literal<Num>();
      else
         return dimension::concat(
                     dimension::concat(int_literal<Num>(), lit<'/' >()),
                     int_literal<Den>());
   }

   struct FundamentalUnitTag;

   // TODO: MOVE THIS
   template<typename Unit, int Num = 1, int Den = 1, bool isQuantity = delta>
   struct unit_exponent
   {
      using unit = Unit;
      using exponent = std::ratio<Num, Den>;

      static constexpr auto exponentString = ratio_literal<Num, Den>();

      static constexpr StringLiteral<3> delim = "::"; // Size three due to null terminator

      static constexpr auto unit_qname = []{
            if constexpr (std::is_base_of_v<FundamentalUnitTag, Unit>)
                  return Unit::qualifiedName;                        // e.g. "length::meters"
            else
                  return dimension::make_units_qname_tuple<typename Unit::units>(); // e.g. "length::feet|length::feet"
         }();

      static constexpr StringLiteral<unit_qname.size + delim.size - 1> test = concat(unit_qname, delim); // size - 1 to account for removed null terminator from first param
      
      static constexpr StringLiteral<test.size + exponentString.size - 1> qualifiedName = concat(test, exponentString); // size - 1 to account for removed null terminator from first param

      static constexpr bool quantity = isQuantity;
   };

   template <typename> struct is_tuple: std::false_type {};

   template <typename ...T> struct is_tuple<std::tuple<T...>>: std::true_type {};

   template<typename T>
   concept IsBasicUnitType = requires
   {
      typename T::Dim;
      typename T::Primary;
      requires std::is_same_v<typename T::Dim, typename T::Primary::Dim>;
      typename T::NumTuple;
      typename T::DenTuple;
      requires is_tuple<typename T::NumTuple>::value;
      requires is_tuple<typename T::DenTuple>::value;
      { T::ID } -> std::convertible_to<int>;
      requires PrimaryConvertible<T>;
      requires is_absolute<typename T::Primary>::value;
   };

   template<typename Tuple, std::size_t... Is>
   // cppcheck-suppress unusedFunction // Only used within concept
   constexpr bool all_satisfy_unit_constraints(std::index_sequence<Is...>)
   {
      return (IsBasicUnitType<std::tuple_element_t<Is, Tuple>> && ...);
   }

   // The IsUnitTuple concept that ensures all types in a tuple satisfy Has_Dim
   // TODO: REMOVE
   template<typename Tuple>
   concept IsUnitTuple = 
      std::tuple_size_v<Tuple> == 0 || 
      all_satisfy_unit_constraints<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});


   template<typename T>
   concept is_unit_exponent = requires {
         typename T::unit;
         typename T::exponent;
   };

   template<typename... Ts>
   concept are_unit_exponents = (is_unit_exponent<Ts> && ...);
}

#endif // DIMENSION_UNIT_VALIDATION_H
