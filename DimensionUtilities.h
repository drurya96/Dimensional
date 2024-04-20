#ifndef DIMENSION_UTILITIES_H
#define DIMENSION_UTILITIES_H

#include <type_traits>
#include <tuple>

namespace Dimension
{
   // Forward declarations
   template<typename NumTuple, typename DenTuple>
   class BaseDimension;
   class BaseUnit;

   /// @brief Convenience alias for retrieving the type of a tuple of types
   /// @tparam Ts Parameter pack to types to concatenate
   template<typename...Ts>
   using tuple_cat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

   /// @brief Remove one instance of a type from a type tuple
   /// @details Recursive base-case
   template<typename...>
   struct RemoveOneInstance {
      using type = std::tuple<>;
   };

   /// @brief Remove one instance of a type from a type tuple
   /// @details Primary usage
   /// @tparam T The type to remove
   /// @tparam Head The first type in the tuple
   /// @tparam Tail The remaining types in the tuple
   template<typename T, typename Head, typename... Tail>
   struct RemoveOneInstance<T, std::tuple<Head, Tail...>> {
      using type = std::conditional_t<std::is_same_v<T, Head>,
         std::tuple<Tail...>,
         decltype(std::tuple_cat(std::tuple<Head>(), typename RemoveOneInstance<T, std::tuple<Tail...>>::type()))>;
   };

   /// @brief Check if a tuple contains a type
   /// @details Recursive base-case
   template <typename T, typename Tuple>
   struct has_type;

   /// @brief Check if a tuple contains a type
   /// @details Primary usage
   /// @tparam T The type to check for
   /// @tparam Us The types contained in the given tuple
   template <typename T, typename... Us>
   struct has_type<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...> {};

   /// @brief Find the difference between two type-tuples
   template<typename T, typename ...>
   struct tuple_diff;

   /// @brief Find the difference between two type-tuples
   /// @details Recursive base-case
   template<typename ... subtrahendTypes>
   struct tuple_diff<std::tuple<>, std::tuple<subtrahendTypes...>>
   {
      using type = std::tuple<>;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Computes the multiset difference between two tuples, where the multiset
   ///    difference of the minuend and subtrahend contains all elements from the
   ///    minuend except those that also appear in the subtrahend.The multiplicity 
   ///    of elements is taken into account.
   /// @tparam T The first type in the minuend tuple
   /// @tparam restMinuendTypes The remaining types in the minuend tuple
   /// @tparam subtrahendTypes The types in the subtrahend tuple
   template<typename T, typename... restMinuendTypes, typename... subtrahendTypes>
   struct tuple_diff<std::tuple<T, restMinuendTypes...>, std::tuple<subtrahendTypes...>> {
      using type = std::conditional_t<
         has_type<T, std::tuple<subtrahendTypes...>>::value,
         typename tuple_diff<std::tuple<restMinuendTypes...>, typename RemoveOneInstance<T, std::tuple<subtrahendTypes...>>::type>::type,
         tuple_cat_t<std::tuple<T>, typename tuple_diff<std::tuple<restMinuendTypes...>, std::tuple<subtrahendTypes...>>::type>
      >;
   };

   /// @brief Concatenates two vectors of BaseUnit pointers
   /// @param a The first vector
   /// @param b The second vector
   /// @return The concatenated vector
   inline std::vector<BaseUnit*> ConcatenateUnitVectors(std::vector<BaseUnit*> a, std::vector<BaseUnit*> b)
   {
      std::vector<BaseUnit*> result;
      result.reserve(a.size() + b.size());

      result.insert(result.end(), a.begin(), a.end());
      result.insert(result.end(), b.begin(), b.end());
      return result;
   }


   /// @brief Return a base dimension templated on types within the given tuple
   /// @tparam Ts The types within the given tuple
   /// @tparam Args The template parameters of the input BaseDimension
   /// @param[in] obj An object of type BaseDimension
   /// @return An object of type BaseDimension templated on Ts
   /// @todo Consider ways to improve effeciency
   template<typename NumTuple, typename DenTuple, typename ... Args>
   auto TupleToBaseDimension(const NumTuple&, const DenTuple&, const BaseDimension<Args...>& obj) {

      std::vector<BaseUnit*> newNumList = obj.numList;
      std::vector<BaseUnit*> newDenList = obj.denList;
      double newValue = obj.GetRawValue();

      for (auto numIter = newNumList.begin(); numIter != newNumList.end();)
      {
         bool erased = false; // Flag to track if an element was erased

         for (auto denIter = newDenList.begin(); denIter != newDenList.end(); ++denIter)
         {
            if ((*numIter)->GetPrimaryUnit() == (*denIter)->GetPrimaryUnit())
            {
               if (*numIter != *denIter)
               {
                  newValue = (*numIter)->ConvertValueNumerator(newValue, **denIter);
               }
               // Remove the current items from both lists
               numIter = newNumList.erase(numIter);
               denIter = newDenList.erase(denIter);
               erased = true; // Set erased flag to true
               // Break out of the inner loop
               break;
            }
         }

         // Check if an element was erased and if we need to break out of the outer loop
         if (!erased)
         {
            ++numIter;
         }

         if (numIter == newNumList.end())
         {
            break;
         }
      }


      return BaseDimension<NumTuple, DenTuple>(newValue, newNumList, newDenList);
   }

}

#endif // DIMENSION_UTILITIES_H