


#ifndef DIMENSION_BASE_H
#define DIMENSION_BASE_H

#include <vector>
#include <type_traits>
#include <tuple>
#include <string>
#include <unordered_map>
#include <functional>

#include <typeindex>

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

      virtual std::string GetDimName() {
         return "Base";
      };

      virtual std::string GetUnitName() {
         return unitName;
      }

      virtual BaseUnit<>* GetBaseUnit() {
         return new BaseUnit<>(); // TODO: This is only a placeholder and IS A MEMORY LEAK. Instead just make this abstract.
      }

      std::string unitName = "";

      std::unordered_map<std::string, std::function<double(double)>> conversions;
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

      std::vector<BaseUnit<>*>* baseUnitVector;

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

      
      // TODO: This method is very inefficient and will likely be bottleneck.
      // Optimize as soon as possible, but other features take priority.
      // Also consider breaking some of this functionality up
      double GetVal(const std::vector<BaseUnit<>*>& i_numList, const std::vector<BaseUnit<>*>& i_denList) const
      {
         auto temp_InputNumList = i_numList;
         auto temp_InputDenList = i_denList;
         auto temp_MyNumList = numList;
         auto temp_MyDenList = denList;

         double result = value;


         for (auto it = temp_MyNumList.begin(); it != temp_MyNumList.end(); ) {
            auto duplicateIt = std::find(temp_InputNumList.begin(), temp_InputNumList.end(), *it);
            if (duplicateIt != temp_InputNumList.end()) {
               it = temp_MyNumList.erase(it);
               temp_InputNumList.erase(duplicateIt);
            }
            else {
               ++it;
            }
         }

         for (auto it = temp_MyDenList.begin(); it != temp_MyDenList.end(); ) {
            auto duplicateIt = std::find(temp_InputDenList.begin(), temp_InputDenList.end(), *it);
            if (duplicateIt != temp_InputDenList.end()) {
               it = temp_MyDenList.erase(it);
               temp_InputDenList.erase(duplicateIt);
            }
            else {
               ++it;
            }
         }


         // Handle Numerator

         if (temp_MyNumList.size() != temp_InputNumList.size()) {
            throw std::runtime_error("Vectors must have the same size.");
         }

         bool found = false;
         for (BaseUnit<>* myUnit : temp_MyNumList)
         {
            found = false;
            for (auto it = temp_InputNumList.begin(); it != temp_InputNumList.end();)
            {
               if (myUnit->baseUnitVector == (*it)->baseUnitVector)
               {
                  if (myUnit->conversions.find((*it)->GetUnitName()) != myUnit->conversions.end())
                  {
                     result = myUnit->conversions[(*it)->GetUnitName()](result);
                  }
                  else
                  {
                     result = myUnit->conversions[myUnit->GetBaseUnit()->GetUnitName()](result);
                     result = myUnit->GetBaseUnit()->conversions[(*it)->GetUnitName()](result);
                  }

                  temp_InputNumList.erase(it);
                  found = true;
                  break;
               }
               else { ++it; }
            }

            if (!found)
            {
               // Not found, raise an exception
            }
         }


         // Handle Denominator

         if (temp_MyDenList.size() != temp_InputDenList.size()) {
            throw std::runtime_error("Vectors must have the same size.");
         }

         //bool found = false;
         for (BaseUnit<>* myUnit : temp_MyDenList)
         {
            found = false;
            for (auto it = temp_InputDenList.begin(); it != temp_InputDenList.end();)
            {
               if (myUnit->baseUnitVector == (*it)->baseUnitVector)
               {
                  if (myUnit->conversions.find((*it)->GetUnitName()) != myUnit->conversions.end())
                  {
                     result = (*it)->conversions[myUnit->GetUnitName()](result);
                  }
                  else
                  {
                     result = (*it)->conversions[myUnit->GetBaseUnit()->GetUnitName()](result);
                     result = myUnit->GetBaseUnit()->conversions[myUnit->GetUnitName()](result);
                  }

                  temp_InputDenList.erase(it);
                  found = true;
                  break;
               }
               else { ++it; }
            }

            if (!found)
            {
               // Not found, raise an exception
            }
         }
         return result;
      }
      
      BaseDimension<UnitType...>& operator+=(const BaseDimension<UnitType...>& rhs)
      {
         value += rhs.GetVal(numList, denList);
         return *this;
      }
      BaseDimension<UnitType...>& operator-=(const BaseDimension<UnitType...>& rhs)
      {
         value -= rhs.GetVal(numList, denList);
         return *this;
      }

      BaseDimension<UnitType...>& operator*=(double rhs)
      {
         value *= rhs;
         return *this;
      }

      BaseDimension<UnitType...>& operator/=(double rhs)
      {
         value /= rhs;
         return *this;
      }

      // The following operators are explicitly deleted
      BaseDimension<UnitType...>& operator*=(const BaseDimension<UnitType...>& rhs) = delete; // Multiplication results in a different type
      BaseDimension<UnitType...>& operator/=(const BaseDimension<UnitType...>& rhs) = delete; // Division results in a different type
      BaseDimension<UnitType...>& operator+=(double rhs) = delete; // Addition cannot be performed between a Dimension and a scalar
      BaseDimension<UnitType...>& operator-=(double rhs) = delete; // Subtraction cannot be performed between a Dimension and a scalar




      // Comparison Operators
      bool operator>(const BaseDimension<UnitType...>& rhs) const { return value > rhs.GetValue(numList, denList); }
      bool operator<(const BaseDimension<UnitType...>& rhs) const { return value < rhs.GetValue(numList, denList); }
      bool operator>=(const BaseDimension<UnitType...>& rhs) const { return value >= rhs.GetValue(numList, denList); }
      bool operator<=(const BaseDimension<UnitType...>& rhs) const { return value <= rhs.GetValue(numList, denList); }
      bool operator==(const BaseDimension<UnitType...>& rhs) const { return value == rhs.GetValue(numList, denList); } // I need to add a small tolerance to this
      bool operator!=(const BaseDimension<UnitType...>& rhs) const { return !(*this == rhs); }

      // TODO: Define a NearlyEqual method with custom tolerance


   };


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



   template <int N>
   struct AbsHelper {
      static constexpr int value = N >= 0 ? N : -N;
   };


// Simplify function for any Simplifier type
   template <typename... Ts>
   struct SimplifierInterface {};


   // ==========================================
   // Time Simplification

   // Simplify function for TimeUnit types
   
   template <typename... Ts>
   struct TimeUnitSimplifier : SimplifierInterface<Ts...> {
  
      static constexpr size_t TimeCount = count_type<TimeUnit<>, Ts...>();
      static constexpr size_t InverseTimeCount = count_type<TimeUnit<Inverse>, Ts...>();
      
      using type = std::conditional_t<(TimeCount > InverseTimeCount),
         typename Repeat<TimeCount - InverseTimeCount, TimeUnit<>>::type,
         std::conditional_t<(TimeCount < InverseTimeCount),
         typename Repeat<InverseTimeCount - TimeCount, TimeUnit<Inverse>>::type,
         std::tuple<>>>;
   };
   


   // ==========================================
   // Length Simplification

   // Simplify function for LengthUnit types
   template <typename... Ts>
   struct LengthUnitSimplifier : SimplifierInterface<Ts...> {

      static constexpr size_t LengthCount = count_type<LengthUnit<>, Ts...>();
      static constexpr size_t InverseLengthCount = count_type<LengthUnit<Inverse>, Ts...>();

      using type = std::conditional_t<(LengthCount > InverseLengthCount),
         typename Repeat<LengthCount - InverseLengthCount, LengthUnit<>>::type,
         std::conditional_t<(LengthCount < InverseLengthCount),
         typename Repeat<InverseLengthCount - LengthCount, LengthUnit<Inverse>>::type,
         std::tuple<>>>;

   };

   // =========================================
   // Generic Unit Simplification
   // This will need updates to handle user-defined units
   // Consider using some equavalent to lambda/functor registration
   // Must be compile-time aware



   // Commenting out to test
   // THE FOLLOWING WORKS, DO NOT DELETE!!!
   
   template <typename ... Ts>
   struct AllUnitSimplifier
   {
      using type = tuple_cat_t<typename LengthUnitSimplifier<Ts...>::type, typename TimeUnitSimplifier<Ts...>::type>;
   };


   
   /*
   template<typename...>
   struct AllUnitSimplifier;

   template<typename T, typename... Ts>
   struct AllUnitSimplifier<std::tuple<T, Ts...>> {

      // Ensure T is non-inverse

      using firstTypes = BreakTypes<T, T, Ts...>;

      using rest = remove_t<T<>, T<Inverse>, T, Ts...>;

      using type = tuple_cat_t<typename LengthUnitSimplifier<Ts...>::type, AllUnitSimplifier<>>;
   };
   */

   // Function to check if two pointers in the vector belong to the same derived class
   inline bool areSameDerivedClass(const BaseUnit<>* unit1, const BaseUnit<>* unit2) {
      const std::type_index& type1 = typeid(*unit1);
      const std::type_index& type2 = typeid(*unit2);

      bool areSame = type1 == type2;

      return areSame;
      //return type1 == type2;
   }





   // Helper function to convert tuple types to BaseDimension
   template<typename... Ts, std::size_t... Is, typename ... Args>
   auto TupleToBaseDimension(const std::tuple<Ts...>&, std::index_sequence<Is...>, const BaseDimension<Args...>& obj) {

      std::vector<BaseUnit<>*> newNumList = obj.numList;
      std::vector<BaseUnit<>*> newDenList = obj.denList;
      double newValue = obj.value;

      // Consider ways to improve effeciency of this section

      for (auto numIter = newNumList.begin(); numIter != newNumList.end(); ++numIter)
      {
         for (auto denIter = newDenList.begin(); denIter != newDenList.end(); ++denIter)
         {
            //if (typeid(*numIter) == typeid(*denIter))
            //if (dynamic_cast<BaseUnit*>(*numIter)->getClassType() == dynamic_cast<BaseUnit*>(*denIter)->getClassType())
            //if(areSameDerivedClass(*numIter, *denIter))
            if((*numIter)->GetDimName() == (*denIter)->GetDimName())
            {
               if (*numIter != *denIter)
               {
                  //newValue = (*denIter)->conversions[(*numIter)->GetUnitName()](newValue);
                  newValue = (*numIter)->conversions[(*denIter)->GetUnitName()](newValue);
               }
               // Remove the current items from both lists
               numIter = newNumList.erase(numIter);
               denIter = newDenList.erase(denIter);

               // Break out of the inner loop
               break;
            }
            
         }
         // Check if we need to break out of the outer loop
         if (numIter == newNumList.end())
            break;
      }

      return BaseDimension<Ts...>(newValue, newNumList, newDenList);
   }


   // Simplify function for BaseDimension
   // If problems arise, consider specifying return type
   template<typename... Ts>
   auto SimplifyBaseDimension(const BaseDimension<Ts...>& obj)// -> decltype(TupleToBaseDimension(typename AllUnitSimplifier<Ts...>::type{}, std::index_sequence_for<Ts...>{}, std::declval<BaseDimension<Ts...>>()))
   {
      using TupleType = typename AllUnitSimplifier<Ts...>::type;
      return TupleToBaseDimension(TupleType{}, std::index_sequence_for<Ts...>{}, obj);
   }


 


   // Division operator for two Dimensions
   template<typename... T_Classes1, typename... T_Classes2>
   auto operator/(const BaseDimension<T_Classes1...>& obj1, const BaseDimension<T_Classes2...>& obj2)
      -> decltype(SimplifyBaseDimension(std::declval<BaseDimension<T_Classes1..., typename InvertReturnType<T_Classes2>::type...>>()))
   {
      using ResultType = BaseDimension<T_Classes1..., typename InvertReturnType<T_Classes2>::type...>;
      auto result = ResultType(
         obj1.value / obj2.value,
         ConcatenateUnitVectors(obj1.numList, obj2.denList),
         ConcatenateUnitVectors(obj1.denList, obj2.numList));
      return SimplifyBaseDimension(result);
   }

   template<typename ... Ts>
   BaseDimension<Ts...>& operator/=(BaseDimension<Ts...>& lhs, const BaseDimension<Ts...>& rhs) {
      lhs.value += rhs.GetVal(lhs.numList, lhs.denList);
      return lhs;
   }

   // Multiplication operator for two Dimensions
   template<typename ... T_Classes1, typename ... T_Classes2>
   auto operator*(const BaseDimension<T_Classes1...>& obj1, const BaseDimension<T_Classes2...>& obj2) 
      -> decltype(SimplifyBaseDimension(std::declval<BaseDimension < T_Classes1..., T_Classes2... >>()))
   {
      using ResultType = BaseDimension<T_Classes1..., T_Classes2...>;
      auto result = ResultType(
         obj1.value * obj2.value,
         ConcatenateUnitVectors(obj1.numList, obj2.numList),
         ConcatenateUnitVectors(obj1.denList, obj2.denList)
      );
      return SimplifyBaseDimension(result);
   }




   // Scalar Math
   template<typename ... Ts>
   BaseDimension<Ts...> operator*(const BaseDimension<Ts...>& obj, double scalar)
   {
      return BaseDimension<Ts...>(obj.value * scalar, obj.numList, obj.denList);
   }

   template<typename ... Ts>
   BaseDimension<Ts...> operator*(double scalar, const BaseDimension<Ts...>& obj)
   {
      return obj * scalar;
   }

   template<typename ... Ts>
   BaseDimension<Ts...> operator/(const BaseDimension<Ts...>& obj, double scalar)
   {
      return BaseDimension<Ts...>(obj.value / scalar, obj.numList, obj.denList);
   }

   template<typename ... Ts>
   auto operator/(double scalar, const BaseDimension<Ts...>& obj) -> BaseDimension<typename InvertReturnType<Ts>::type...>
   {
      using ResultType = BaseDimension<typename InvertReturnType<Ts>::type...>;

      return ResultType(scalar / obj.value, obj.denList, obj.numList);
   }



   // Additive Math
   template<typename ... Ts>
   BaseDimension<Ts...> operator+(const BaseDimension<Ts...>& obj1, const BaseDimension<Ts...>& obj2)
   {
      return BaseDimension<Ts...>(obj1.value + obj2.GetVal(obj1.numList, obj1.denList), obj1.numList, obj1.denList);
   }

   template<typename ... Ts>
   BaseDimension<Ts...> operator-(const BaseDimension<Ts...>& obj1, const BaseDimension<Ts...>& obj2)
   {
      return BaseDimension<Ts...>(obj1.value - obj2.GetVal(obj1.numList, obj1.denList), obj1.numList, obj1.denList);
   }

}
#endif // DIMENSION_BASE_H