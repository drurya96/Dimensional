#ifndef STATIC_DIMENSION_AMOUNT_IMPL_H
#define STATIC_DIMENSION_AMOUNT_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   struct AmountType {};
   struct Moles;

   template<typename T>
   concept IsAmountUnit = IsNonQuantityUnitDimension<T, AmountType> || IsQuantityUnitDimension<T, AmountType>;

   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation>
   struct AmountUnit : public BaseUnit<Unit, Name, Abbreviation, "Amount">
   { 
   public: 
      using Dim = AmountType;
      using Primary = Moles;
   };

   template<typename T>
   struct is_amount : std::false_type {};

   template<typename T>
   struct is_amount<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsAmountUnit<T>> {};

   template<typename T>
   constexpr bool is_amount_v = is_amount<T>::value;

   template<typename T>
   concept amount_type = is_amount_v<T>;

   template<IsAmountUnit T>
   PrecisionType getAmount(amount_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   template<IsAmountUnit Unit>
   class Amount : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Amount() : BaseDimension<std::tuple<Unit>, std::tuple<>>(0.0) {}

      Amount(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>(val) {}

      template<IsAmountUnit T>
      Amount(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>(base.template GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<IsAmountUnit T>
      [[deprecated("Use the free function getAmount() instead.")]]
      double GetAmount() const
      {
         return getAmount<T>(*this);
      }
   };

   template<IsAmountUnit T>
   Amount(T) -> Amount<T>;

   template<IsAmountUnit AmountUnit>
   Amount(BaseDimension<std::tuple<AmountUnit>, std::tuple<>>) -> Amount<AmountUnit>;

   template<typename T>
   struct is_amount<Amount<T>> : std::bool_constant<IsAmountUnit<T>> {};

}

#endif //STATIC_DIMENSION_AMOUNT_IMPL_H
