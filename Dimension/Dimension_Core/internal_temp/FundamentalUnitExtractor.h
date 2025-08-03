#ifndef DIMENSION_UNIT_EXTRACTOR_H
#define DIMENSION_UNIT_EXTRACTOR_H

#include "TupleHandling.h"
#include "unit_exponent.h"

namespace dimension
{

   template<typename... Units>
   struct FundamentalUnitExtractor;

   template<>
   struct FundamentalUnitExtractor<std::tuple<>>
   {
      using units = std::tuple<>;
   };

   template<typename Unit>
   struct FundamentalUnitExtractor<std::tuple<Unit>>
   {
      using units = typename raise_unit_exponent<typename Unit::exponent, typename Unit::unit::units>::units;
   };

   template<typename Unit, typename... Units>
   struct FundamentalUnitExtractor<std::tuple<Unit, Units...>>
   {
      using units = tuple_cat_t<typename FundamentalUnitExtractor<std::tuple<Unit>>::units,
         typename FundamentalUnitExtractor<std::tuple<Units...>>::units>;
   };

   // -- Dimension tag check
   template<typename T, typename ExpectedTag>
   constexpr bool matches_tag = std::is_same_v<typename T::Dim, ExpectedTag>;

   template<typename ExpectedTag, typename... Units>
   struct Extractor;
   
   template<typename ExpectedTag, typename First, typename... Rest>
   struct Extractor<ExpectedTag, First, Rest...> {
   private:
       static constexpr bool is_match = matches_tag<First, ExpectedTag>;
   
   public:
       using type = std::conditional_t<
           is_match,
           First,
           typename Extractor<ExpectedTag, Rest...>::type
       >;
   };
   
   template<typename ExpectedTag>
   struct Extractor<ExpectedTag> {
      using type = void;
       //static_assert(sizeof(ExpectedTag) == 0, "No matching unit found for this dimension.");
   };

} // end Dimension

#endif // DIMENSION_UNIT_EXTRACTOR_H