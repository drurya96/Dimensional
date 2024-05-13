#ifndef DIMENSION_LENGTH_H
#define DIMENSION_LENGTH_H

#include "BaseDimension.h"
#include <type_traits>
#include <tuple>

#define SINGLE_SPECIALIZE(From, To) \
   template <> \
   struct convertible<From, To> : std::true_type {}; \
   template <> \
   struct convertible<To, From> : std::true_type {};





namespace Dimension
{

   struct Length {};
   class Meters;

   template<typename Unit>
   class LengthUnit : public BaseUnit<LengthUnit<Unit>> 
   { 
   public: 
      using BaseUnit::BaseUnit;

      using Dim = Length;


      Meters GetPrimary();
      /*
      Meters GetPrimary()
      {
         return ConvertValue<Unit, Meters>(*this);
         //return value;
      }
      */

   };

   class Meters : public LengthUnit<Meters> { public: using LengthUnit::LengthUnit; };
   class Feet : public LengthUnit<Feet> { public: using LengthUnit::LengthUnit; };
   class Inches : public LengthUnit<Inches> { public: using LengthUnit::LengthUnit; };

   template<typename Unit>
   inline Meters LengthUnit<Unit>::GetPrimary()
   {
      if constexpr (std::is_same_v<Unit, Meters>)
      {
         return *(static_cast<Meters*>(this));
      }
      else
      {
         return ConvertValue<Unit, Meters>(*(static_cast<Unit*>(this)));
      }


      //return ConvertValue<Unit, Meters>(*(static_cast<Unit*>(this)));
      //return value;
   }


   // Base template for convertible trait
   template <typename From, typename To>
   struct convertible : std::false_type {}; // By default, no conversion is assumed

   //SINGLE_SPECIALIZE(Meters, Feet);
   //SINGLE_SPECIALIZE(Meters, Inches);
   //SINGLE_SPECIALIZE(Feet, Inches);

   //SPECIALIZE(Meters, Feet, Inches);



   /*

   // Helper template to generate multiple specializations recursively
   template <typename...>
   struct GenerateConvertibles;

   // Base case: no types left to specialize
   template <>
   struct GenerateConvertibles<> {
      static void generate() {}
   };

   // Recursive case: specialize convertible for each type pair
   template <typename From, typename To, typename... Rest>
   struct GenerateConvertibles<From, To, Rest...> {
      static void generate() {
         // Specialize convertible for current From and To types
         template <>
         struct convertible<From, To> : std::true_type {};

         // Recur to specialize for the rest of the type pairs
         GenerateConvertibles<Rest...>::generate();
      }
   };

   // Define a global tuple of type pairs to specialize convertible for
   using TypePairs = std::tuple<
      std::pair<int, double>,
      std::pair<float, char>,
      std::pair<long, short>
   >;

   // Instantiate a helper struct to trigger specialization generation at file scope
   struct ConvertibleInitializer {
      ConvertibleInitializer() {
         GenerateConvertibles<int, double, float, char, long, short>::generate();
      }
   };

   // Instantiate the ConvertibleInitializer as a static variable at file scope
   static ConvertibleInitializer initializer;
   */




















   /*

   // Base template for convertible trait
   template <typename From, typename To>
   struct convertible : std::false_type {}; // By default, no conversion is assumed

   // Helper template to force instantiation of specific specializations
   template <typename... Ts>
   struct ForceInstantiation {};

   // Create a variadic template to generate convertible specializations
   template <typename From, typename... To>
   struct GenerateSpecializations {
      // Helper struct to generate ConvertibleHelper specializations
      template <typename T>
      struct ConvertibleHelper;

      // Partial specialization of ConvertibleHelper for each To type
      template <typename... Ts>
      struct ConvertibleHelper<ForceInstantiation<Ts...>> : Ts... {};

      // Ensure instantiation of ConvertibleHelper specializations
      using Instantiator = ConvertibleHelper<ForceInstantiation<convertible<From, To>...>>;
   };

   constexpr auto specializations = GenerateSpecializations<Meters, Feet, Inches>::Instantiator{};
   */






   /*
   // Base template for convertible trait
   template <typename From, typename To>
   struct convertible : std::false_type {}; // By default, no conversion is assumed

   // Helper template to generate convertible specializations
   template <typename From, typename... To>
   struct GenerateSpecializations {
      // Ensure instantiation of ConvertibleHelper and set conversions to true
      static constexpr auto instantiator = [] {
         struct ConvertibleHelper : convertible<From, To>... {};
         return ConvertibleHelper{};
         }();
   };

   // Instantiate the necessary specializations using a constexpr variable
   constexpr auto specializations = GenerateSpecializations<Meters, Feet>::instantiator;

   */
















   template<>
   Feet ConvertValue<Meters, Feet>(Meters& obj)
   {
      return Feet(obj.GetValue() * 3.28084);
   }
   
   template<>
   Meters ConvertValue<Feet, Meters>(Feet& obj)
   {
      return Meters(obj.GetValue() / 3.28084);
   }
   
   
}

#endif //DIMENSION_LENGTH_H