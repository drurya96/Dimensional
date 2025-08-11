#ifndef DIMENSIONAL_UNIT_FILTER_H
#define DIMENSIONAL_UNIT_FILTER_H

#include <ratio>
#include <tuple>
#include <type_traits>

#include "../TupleHandling.h"
#include "unit_exponent.h"
#include "unit_dim.h"

namespace dimension
{

   // -- Dimension tag check
   template<typename T, typename ExpectedTag>
   constexpr bool matches_tag = std::is_same_v<unit_dim_t<T>, ExpectedTag>;

   template<typename ExpectedTag, typename... Units>
   struct unit_filter;
   
   template<typename ExpectedTag, typename First, typename... Rest>
   struct unit_filter<ExpectedTag, First, Rest...> {
   private:
       static constexpr bool is_match = matches_tag<First, ExpectedTag>;
   
   public:
       using type = std::conditional_t<
           is_match,
           First,
           typename unit_filter<ExpectedTag, Rest...>::type
       >;
   };
   
   template<typename ExpectedTag>
   struct unit_filter<ExpectedTag> {
      using type = void;
       //static_assert(sizeof(ExpectedTag) == 0, "No matching unit found for this dimension.");
   };

   // TODO: Reconsider whether both of these are really needed...

   template<typename TargetTag, typename UnitTuple>
   struct simplified_unit_filter_helper;
   
   template<typename TargetTag, typename... UEs>
   struct simplified_unit_filter_helper<TargetTag, std::tuple<UEs...>> {
       using type = typename unit_filter<TargetTag, typename UEs::unit...>::type;
   };
   
   template<typename TargetTag, typename UnitTuple>
   using simplified_unit_filter = typename simplified_unit_filter_helper<TargetTag, simplified_units_t<UnitTuple>>::type;








   // ============================================================
   // ============== Find Matching Unit by Dimension =============
   // ============================================================

   // THIS IS NAMED VERY WRONG AND IS CONFUSING
   // THIS IS REALLY FINDING MATCHING UNITS, NOT DIMENSIONS

   template<typename Target, typename Tuple>
   struct find_unit_by_dimension;

   // Empty case
   template<typename Target>
   struct find_unit_by_dimension<Target, std::tuple<>> {
      static constexpr bool found = false;

      struct Dummy
      {
         using exponent = std::ratio<0>;
      };
      using type = Dummy;
   };

   // Recursive case
   template<typename Target, typename Head, typename... Tail>
   struct find_unit_by_dimension<Target, std::tuple<Head, Tail...>> {
   private:
      static constexpr bool is_match = std::is_same_v<typename Target::unit, typename Head::unit>;

   public:
      static constexpr bool found = is_match || find_unit_by_dimension<Target, std::tuple<Tail...>>::found;
      using type = std::conditional_t<is_match, Head, typename find_unit_by_dimension<Target, std::tuple<Tail...>>::type>;
   };



   template<typename From, typename ToTuple>
   struct MatchUnit;
   

   template<typename From>
   struct MatchUnit<From, std::tuple<>>
   {
      using type = void; // Should never occur
   };

   template<typename From, typename Head, typename... Tail>
   struct MatchUnit<From, std::tuple<Head, Tail...>>
   {
      //using unit_type = typename From::unit;
   
      static constexpr bool match = is_same_dim<typename From::unit, typename Head::unit>::value;

      using type = std::conditional_t<
         match,
         Head,
         typename MatchUnit<From, std::tuple<Tail...>>::type
      >;
   };








} // end Dimension

#endif // DIMENSIONAL_UNIT_FILTER_H
