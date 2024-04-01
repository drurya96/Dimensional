


#ifndef DIMENSION_BASE_H
#define DIMENSION_BASE_H

#include <vector>
#include <type_traits>
#include <tuple>

#include "DimensionUtilities.h"

namespace Dimension
{

   template<typename ... is_inverse>
   class BaseUnit
   {
   public:
      BaseUnit() {}

      //BaseUnit(std::vector<BaseUnit*> num, std::vector<BaseUnit*> den):
         //numList(num),
         //denList(den)
      //{}
      virtual ~BaseUnit() {}


      /*
      //template<typename... Args>
       constexpr double GetID() {
         if constexpr (sizeof...(is_inverse) == 0) {
            return GetStandardID();
         }
         else {
            return GetInverseID();
         }
      }
      */

      //std::vector<BaseUnit*> numList;
      //std::vector<BaseUnit*> denList;

   private:

      // Prefer making these pure virtual, but this is fine for now
        virtual constexpr double GetStandardID() { return 0.0; };
        virtual constexpr double GetInverseID() { return 0.0; };

   };

   template<typename ... UnitType>
   class BaseDimension
   {
   public:
      BaseDimension() : 
         value(0.0)
      {
      }

      BaseDimension(double newValue) :
         value(newValue)
      {
      }

      template<typename T>
      explicit BaseDimension(const T& dimension) :
         value(dimension.value),
         numList(dimension.numList.begin(), dimension.numList.end()),
         denList(dimension.denList.begin(), dimension.denList.end())
      {
      }

      BaseDimension(double newValue, std::vector<BaseUnit<>*> newNumList, std::vector<BaseUnit<>*> newDenList) :
         value(newValue),
         numList(newNumList.begin(), newNumList.end()),
         denList(newDenList.begin(), newDenList.end())
      {
      }


      double value;
      //myUnitTypes<UnitType...> units;

      // These are pointers for simplicity for now, but may change.
      // Consider using a tuple for immutability
      // Consider use a raw array (not std::array) to allow storage of references
      // Note a raw array requires compile-time knowledge of size
      // While this is possible since the templates hold that information,
      //    it may be challenging.
      std::vector<BaseUnit<>*> numList;
      std::vector<BaseUnit<>*> denList;

   };

   // Division operator for two Dimensions
   template<typename... T_Classes1, typename... T_Classes2>
   auto operator/(const BaseDimension<T_Classes1...>& obj1, const BaseDimension<T_Classes2...>& obj2)
      -> BaseDimension<T_Classes1..., typename InvertReturnType<T_Classes2>::type...>
   {
      return BaseDimension<T_Classes1..., typename InvertReturnType<T_Classes2>::type...>(
         obj1.value / obj2.value, // Consider how to deal with divide-by-zero
         ConcatenateUnitVectors(obj1.numList, obj2.denList),
         ConcatenateUnitVectors(obj1.denList, obj2.numList));
   }
   
   // Multiplication operator for two Dimensions
   template<typename ... T_Classes1, typename ... T_Classes2>
   auto operator*(const BaseDimension<T_Classes1...>& obj1, const BaseDimension<T_Classes2...>& obj2) -> BaseDimension < T_Classes1..., T_Classes2... > {
      return BaseDimension < T_Classes1..., T_Classes2... >(
         obj1.value * obj2.value, // Consider how to deal with divide-by-zero
         ConcatenateUnitVectors(obj1.numList, obj2.numList),
         ConcatenateUnitVectors(obj1.denList, obj2.denList));
   }


   // Forward declare TimeUnit
   template <typename...>
   class TimeUnit;

   // Forward declare LengthUnit
   template <typename...>
   class LengthUnit;


   // ==========================================
   // Simplification logic
   // Only for Time for now
   // This needs to be moved around
   // Some portion should live with the Time class


   // ==========================================
   // Time Simplification

   // Simplify function for TimeUnit types
   template <typename... Ts>
   struct TimeUnitSimplifier {
  
      static constexpr size_t TimeCount = count_type<TimeUnit<>, Ts...>();
      static constexpr size_t InverseTimeCount = count_type<TimeUnit<Inverse>, Ts...>();
      static constexpr int TimeTotal = TimeCount - InverseTimeCount;

      using type = std::conditional_t<(TimeTotal > 0), typename Repeat<TimeTotal, TimeUnit<>>::type,
         std::conditional_t<(TimeTotal < 0), typename Repeat<TimeTotal, TimeUnit<Inverse>>::type, std::tuple<>>>;

   };


   // ==========================================
   // Length Simplification

   // Simplify function for LengthUnit types
   template <typename... Ts>
   struct LengthUnitSimplifier {

      static constexpr size_t LengthCount = count_type<LengthUnit<>, Ts...>();
      static constexpr size_t InverseLengthCount = count_type<LengthUnit<Inverse>, Ts...>();
      static constexpr int LengthTotal = LengthCount - InverseLengthCount;

      static_assert(LengthCount == 2, "Bad Length Count");
      static_assert(InverseLengthCount == 0, "Bad LengthInverted Count");
      static_assert(LengthTotal == 2, "Bad Total Length");

      using type = std::conditional_t<(LengthTotal > 0), typename Repeat<LengthTotal, LengthUnit<>>::type,
         std::conditional_t<(LengthTotal < 0), typename Repeat<LengthTotal, LengthUnit<Inverse>>::type, std::tuple<>>>;

   };

   // =========================================
   // Generic Unit Simplification
   // This will need updates to handle user-defined units
   // Consider using some equavalent to lambda/functor registration
   // Must be compile-time aware

   template <typename ... Ts>
   struct AllUnitSimplifier
   {
      using type = tuple_cat_t<typename LengthUnitSimplifier<Ts...>::type, typename TimeUnitSimplifier<Ts...>::type>;
   };


   // Simplify function for BaseDimension
   // If problems arise, consider specifying return type
   template<typename... Ts>
   auto SimplifyBaseDimension(const BaseDimension<Ts...>&) {
      using TupleType = typename AllUnitSimplifier<Ts...>::type;
      return TupleToBaseDimension(TupleType{}, std::index_sequence_for<Ts...>{});
   }

}
#endif // DIMENSION_BASE_H