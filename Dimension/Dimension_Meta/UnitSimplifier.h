#ifndef DIMENSION_UNIT_SIMPLIFIER_H
#define DIMENSION_UNIT_SIMPLIFIER_H

#include "GenericTypeTraits.h"

namespace Dimension
{
   // Forward declarations
   template<typename NumTuple, typename DenTuple>
   class BaseDimension;

   /// @brief Struct to simplify units by cancelling out as necessary
   template<typename NumTypes1, typename NumTypes2, typename DenTypes1, typename DenTypes2>
   struct UnitSimplifier;

   /// @brief Struct to simplify units by cancelling out as necessary
   /// @tparam NumTypes1... The types in the first group of numerator types
   /// @tparam NumTypes2... The types in the second group of numerator types
   /// @tparam DenTypes1... The types in the first group of denominator types
   /// @tparam DenTypes2... The types in the second group of denominator types
   /// @typedef newNum The simpliefied numerator types
   /// @typedef newDen The simpliefied denominator types
   /// @typedef dimType A BaseDimension templated on the simplified types
   template<typename ... NumTypes1, typename ... NumTypes2, typename ... DenTypes1, typename ... DenTypes2>
   struct UnitSimplifier<std::tuple<NumTypes1...>, std::tuple<NumTypes2...>, std::tuple<DenTypes1...>, std::tuple<DenTypes2...>>
   {
   private:
      using num1AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<NumTypes1...>, std::tuple<DenTypes2...>>;
      using num2AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<NumTypes2...>, std::tuple<DenTypes1...>>;

      using den1AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<DenTypes1...>, std::tuple<NumTypes2...>>;
      using den2AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<DenTypes2...>, std::tuple<NumTypes1...>>;

      using remainingNum1 = tuple_diff<has_same_dim, typename num1AfterSimpleCancel::type, typename den2AfterSimpleCancel::type>;
      using remainingNum2 = tuple_diff<has_same_dim, typename num2AfterSimpleCancel::type, typename den1AfterSimpleCancel::type>;

      using remainingDen1 = tuple_diff<has_same_dim, typename den1AfterSimpleCancel::type, typename num2AfterSimpleCancel::type>;
      using remainingDen2 = tuple_diff<has_same_dim, typename den2AfterSimpleCancel::type, typename num1AfterSimpleCancel::type>;
      

   public:
      using newNum = tuple_cat_t<typename remainingNum1::type, typename remainingNum2::type>;
      using newDen = tuple_cat_t<typename remainingDen1::type, typename remainingDen2::type>;

      using numSimple = tuple_cat_t<typename num1AfterSimpleCancel::type, typename num2AfterSimpleCancel::type>;
      using denSimple = tuple_cat_t<typename den1AfterSimpleCancel::type, typename den2AfterSimpleCancel::type>;

      using dimType = BaseDimension<newNum, newDen>;

      constexpr static bool isDelta = !((std::tuple_size_v<newNum> == 1) && (std::tuple_size_v<newDen> == 0));
   };
} // end Dimension

#endif // DIMENSION_UNIT_SIMPLIFIER_H