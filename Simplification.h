#ifndef SIMPLIFICATION_H
#define SIMPLIFICATION_H

#include "DimensionUtilities.h"
#include <typeindex>

namespace Dimension{

   // Forward declarations
   template<typename NumTuple, typename DenTuple>
   class BaseDimension;
   class BaseUnit;

   /// @brief Simplify dimensions
   /// @details Given two tuples of dimensions, produce
   ///    two new tuples that simplified after "cancelling out"
   ///    dimensions as appropriate.
   /// @tparam NumTuple Tuple representing numerator dimensions
   /// @tparam DenTuple Tuple representing denominator dimensions
   template<typename NumTuple, typename DenTuple>
   struct AllUnitSimplifier
   {
      using numTuple = typename tuple_diff<NumTuple, DenTuple>::type;
      using denTuple = typename tuple_diff<DenTuple, NumTuple>::type;
   };

   /// @brief Function to construct a simplified BaseDimension from a given BaseDimension
   /// @details Call AllUnitSimplifier on the given BaseDimension to simplify the units.
   ///    The resulting BaseDimension has simplified units (i.e. no instances of the same
   ///    type in both the numerator and denominator). The value will be adjusted accordingly.
   /// @tparam Ts The template parameters of the input BaseDimension
   /// @param[in] obj The BaseDimension object to simplify
   /// @return The new BaseDimension object with simplified units
   template<typename NumTuple, typename DenTuple>
   auto SimplifyBaseDimension(const BaseDimension<NumTuple, DenTuple>& obj)
   {
      using Simplified = typename AllUnitSimplifier<NumTuple, DenTuple>;
      return TupleToBaseDimension(Simplified::numTuple{}, Simplified::denTuple{}, obj);
   }

}

#endif //SIMPLIFICATION_H