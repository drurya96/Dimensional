#ifndef DIMENSION_UTILITIES_H
#define DIMENSION_UTILITIES_H

#include <type_traits>
#include <tuple>

namespace Dimension
{
   // Forward declarations
   template <typename...>
   class BaseDimension;

   template <typename...>
   class BaseUnit;

   struct Inverse;

   /// @brief Template struct for checking if dimension is Inverse
   /// @details This struct has a true value, and is used when given
   ///    a non empty parameter pack.
   /// @tparam The given parameter pack, which must be non-empty to use specialization
   template<typename...>
   struct is_inverse_dimension : std::true_type {};

   /// @brief Template struct for checking if dimension is Inverse
   /// @details This struct has a false value, and is used when given
   ///    an empty parameter pack.
   /// @tparam empty to use this specialization
   template<>
   struct is_inverse_dimension<> : std::false_type {};

   /// @brief Convenience alias for retrieving the type of a tuple of types
   /// @tparam Ts Parameter pack to types to concatenate
   template<typename...Ts>
   using tuple_cat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

   /// @brief Template struct representing the type of a tuple of repeating types
   /// @details This struct's type field wil be that of a tuple of N copies
   ///    of the given type.
   /// @tparam N The number of copies to repeat
   /// @tparam T The type to repeat
   template <int N, typename T, typename = void>
   struct Repeat;

   /// @brief Recursive specialization of Repeat for positive N
   /// @details This specialization is used for positive N value.
   ///    It recursively calls Repeat with N-1, adding one instance of T
   ///    to the tuple each time.
   /// @tparam N The number of copies to repeat
   /// @tparam T The type to repeat
   template <int N, typename T>
   struct Repeat<N, T, std::enable_if_t<(N > 0)>> {
      using type = decltype(std::tuple_cat(std::tuple<T>(), typename Repeat<N - 1, T>::type()));
   };

   /// @brief Recursive specialization of Repeat for negative N
   /// @details This specialization is used for positive N value.
   ///    THIS SPECIALIZATION SHOULD NEVER BE USED!
   ///    This exists only so the compiler can resolve types within conditional_t
   ///    which is necessary even if the type cannot be reached.
   /// @todo Consider replacing this with alternate logic to resolve conditional_t
   /// @tparam N The number of copies to repeat
   /// @tparam T The type to repeat
   template <int N, typename T>
   struct Repeat<N, T, std::enable_if_t<(N < 0)>> {
      using type = std::tuple<>;
   };

   
   /// @brief Base case specialization of Repeat for N == 0
   /// @tparam T The given type, unused.
   template <typename T>
   struct Repeat<0, T> {
      using type = std::tuple<>;
   };


   /// @brief Count the number of occurances of a given type within a tuple
   constexpr size_t count_type();

   /// @brief Base-case specialization given only a single type, return 0
   /// @return 0
   template<typename T>
   constexpr size_t count_type() {
      return 0;
   }

   /// @brief Recursive specialization given to count type occurences in a tuple
   /// @details This method counts the number of instances of First within the
   ///    parameter pack <First, Rest...>.
   ///    This is achieved by recursively incrementing a counter if the first
   ///    item in the parameter pack matches the given type.
   /// @tparam T The type to count
   /// @tparam First The first item in the parameter pack
   /// @tparam Rest The remaining parameter pack
   /// @return The count of T in the given pack, as a size_t
   template<typename T, typename First, typename... Rest>
   constexpr size_t count_type() {
      return std::is_same_v<T, First> +count_type<T, Rest...>();
   }

   /// @brief Alias to remove types from a parameter pack
   /// @details remove_t will resolve to the type of a tuple
   ///    containing all items in the given parameter pack
   ///    except those that match either T1 or T2.
   /// @tparam T1 The first type to remove
   /// @tparam T2 The second type to remove
   /// @tparam Ts The parameter pack to remove types from
   template<typename T1, typename T2, typename...Ts>
   using remove_t = tuple_cat_t<
      typename std::conditional<
      std::is_same<T1, Ts>::value || std::is_same<T2, Ts>::value,
      std::tuple<>,
      std::tuple<Ts>
      >::type...
   >;

   /// @brief Concatenates two vectors of BaseUnit pointers
   /// @param a The first vector
   /// @param b The second vector
   /// @return The concatenated vector
   inline std::vector<BaseUnit<>*> ConcatenateUnitVectors(std::vector<BaseUnit<>*> a, std::vector<BaseUnit<>*> b)
   {
      std::vector<BaseUnit<>*> result;
      result.reserve(a.size() + b.size());

      result.insert(result.end(), a.begin(), a.end());
      result.insert(result.end(), b.begin(), b.end());
      return result;
   }

   /// @brief Return the BaseUnit without the Inverted tag
   /// @details This specialize is enabled for BaseUnits that
   ///    have the Inverted template. Returns an object
   ///    of the same BaseUnit without the Inverted template
   /// @tparam T The BaseUnit to invert
   /// @tparam Args The template parameters of BaseUnit
   /// @return An object of type T<>
   template<template<typename...> class T, typename ... Args>
   typename std::enable_if<is_inverse_dimension<Args...>::value, T<>>::type
      invertDimension(const T<Args...>& obj) {
      return T<>();
   }

   /// @brief Return the BaseUnit with the Inverted tag
   /// @details This specialize is enabled for BaseUnits that
   ///    do not have the Inverted template. Returns an object
   ///    of the same BaseUnit with the Inverted template
   /// @tparam T The BaseUnit to invert
   /// @tparam Args The template parameters of BaseUnit
   /// @return An object of type T<Inverted>
   template<template<typename...> class T, typename ... Args>
   typename std::enable_if<!is_inverse_dimension<Args...>::value, T<Inverse>>::type
      invertDimension(const T<Args...>& obj) {
      return T<Inverse>();
   }

   /// @brief Helper struct to deduce return type of invertDimension
   /// @tparam T The BaseUnit type to invert
   template <typename T>
   struct InvertReturnType {
      using type = decltype(invertDimension(std::declval<T>()));
   };

   /// @brief Return a base dimension templated on types within the given tuple
   /// @tparam Ts The types within the given tuple
   /// @tparam Is Index sequence, currently unused
   /// @tparam Args The template parameters of the input BaseDimension
   /// @param[in] obj An object of type BaseDimension
   /// @return An object of type BaseDimension templated on Ts
   /// @todo Consider ways to improve effeciency
   template<typename... Ts, typename ... Args>
   auto TupleToBaseDimension(const std::tuple<Ts...>&, const BaseDimension<Args...>& obj) {

      std::vector<BaseUnit<>*> newNumList = obj.numList;
      std::vector<BaseUnit<>*> newDenList = obj.denList;
      double newValue = obj.GetRawValue();

      for (auto numIter = newNumList.begin(); numIter != newNumList.end();)
      {
         bool erased = false; // Flag to track if an element was erased

         for (auto denIter = newDenList.begin(); denIter != newDenList.end(); ++denIter)
         {
            if ((*numIter)->GetDimName() == (*denIter)->GetDimName())
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


      return BaseDimension<Ts...>(newValue, newNumList, newDenList);
   }
}

#endif // DIMENSION_UTILITIES_H