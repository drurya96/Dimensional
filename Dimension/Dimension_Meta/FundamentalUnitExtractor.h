#ifndef DIMENSION_UNIT_EXTRACTOR_H
#define DIMENSION_UNIT_EXTRACTOR_H

#include "GenericTypeTraits.h"
#include "Concepts.h"

namespace Dimension
{

   /// @brief Extracts all fundamental units from a given numerator and denominator.
   /// @details Directly concatenates the `NumTuple` and `DenTuple` of all units in the provided tuples.
   /// @tparam NumTuple The tuple of units in the numerator.
   /// @tparam DenTuple The tuple of units in the denominator.
   template<typename NumTuple, typename DenTuple>
   struct FundamentalUnitExtractor;

   /// @brief Specialization for when `NumTuple` and `DenTuple` are tuples of units.
   /// @tparam Units The unit types contained within the numerator tuple.
   /// @tparam DenUnits The unit types contained within the denominator tuple.
   template<typename... Units, typename... DenUnits>
   struct FundamentalUnitExtractor<std::tuple<Units...>, std::tuple<DenUnits...>> {
      using Num = tuple_cat_t<typename Units::NumTuple..., typename DenUnits::DenTuple...>;
      using Den = tuple_cat_t<typename DenUnits::NumTuple..., typename Units::DenTuple...>;
   };

   
   template<typename NumTuple, typename DenTuple>
   concept ExtractedUnitsAreValid = 
      IsUnitTuple<typename FundamentalUnitExtractor<NumTuple, DenTuple>::Num> &&
      IsUnitTuple<typename FundamentalUnitExtractor<NumTuple, DenTuple>::Den>;
   

} // end Dimension

#endif // DIMENSION_UNIT_EXTRACTOR_H