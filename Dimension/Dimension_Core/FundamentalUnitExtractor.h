#ifndef DIMENSION_UNIT_EXTRACTOR_H
#define DIMENSION_UNIT_EXTRACTOR_H

#include "TupleHandling.h"

namespace dimension
{


   // TODO: This should live somewhere else
   // Make power a ratio type
   // Make Units a BaseUnit maybe?
   template<typename Power, typename... Units>
   struct ApplyPower;

   template<typename Power>
   struct ApplyPower<Power, std::tuple<>>
   {
      using units = std::tuple<>;
   };

   template<typename Power, typename Unit>
   struct ApplyPower<Power, std::tuple<Unit>>
   {
      using units = std::tuple<unit_exponent<typename Unit::unit, std::ratio_multiply<typename Unit::exponent, Power>::num, std::ratio_multiply<typename Unit::exponent, Power>::den>>;
   };

   template<typename Power, typename Unit, typename... Units>
   struct ApplyPower<Power, std::tuple<Unit, Units...>>
   {
      using units = tuple_cat_t<typename ApplyPower<Power, std::tuple<Unit>>::units, 
         typename ApplyPower<Power, std::tuple<Units...>>::units>;
   };

   /// @brief 
   /// @details 
   /// @tparam 
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
      using units = typename ApplyPower<typename Unit::exponent, typename Unit::unit::units>::units;
   };

   template<typename Unit, typename... Units>
   struct FundamentalUnitExtractor<std::tuple<Unit, Units...>>
   {
      using units = tuple_cat_t<typename FundamentalUnitExtractor<std::tuple<Unit>>::units,
         typename FundamentalUnitExtractor<std::tuple<Units...>>::units>;
   };




   // ==============
   // Necessary for Variadic template...
   // Consider refactoring a bit
   // ==============

   // -- Tag type
   template<typename T>
   concept HasDimTag = requires { typename T::Dim; };

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

   template<typename TargetTag, typename BaseDim>
   struct DimExtractorHelper;
   
   template<typename TargetTag, typename... UEs>
   struct DimExtractorHelper<TargetTag, std::tuple<UEs...>> {
       using type = typename Extractor<TargetTag, typename UEs::unit...>::type;
   };
   
   template<typename TargetTag, typename BaseDim>
   using DimExtractor = typename DimExtractorHelper<TargetTag, typename decltype(FullSimplify(BaseDim{}))::units>::type;

} // end Dimension

#endif // DIMENSION_UNIT_EXTRACTOR_H