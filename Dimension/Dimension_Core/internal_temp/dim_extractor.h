#ifndef DIMENSIONAL_DIM_EXTRACTOR_H
#define DIMENSIONAL_DIM_EXTRACTOR_H

#include "full_simplify.h"
#include "FundamentalUnitExtractor.h"

namespace dimension
{
   template<typename TargetTag, typename BaseDim>
   struct DimExtractorHelper;
   
   template<typename TargetTag, typename... UEs>
   struct DimExtractorHelper<TargetTag, std::tuple<UEs...>> {
       using type = typename Extractor<TargetTag, typename UEs::unit...>::type;
   };
   
   template<typename TargetTag, typename BaseDim>
   using DimExtractor = typename DimExtractorHelper<TargetTag, simplified_units_t<typename BaseDim::units>>::type;

} // end Dimension

#endif // DIMENSIONAL_DIM_EXTRACTOR_H