#ifndef DIMENSION_BASE_H
#define DIMENSION_BASE_H

#include <vector>
#include <type_traits>
#include <tuple>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include "DimensionUtilities.h"

namespace Dimension
{
   /// @brief A base class representing a unit
   /// @details This abstract class represents a Unit,
   ///    such as Meters, Seconds, Grams, etc.
   /// @tparam Unit The derived unit, using CRTP
   template<typename Unit>
   class BaseUnit
   {
   public:
      /// @brief Constructor setting name
      BaseUnit(double val) : value(val){}

      BaseUnit() : value(std::numeric_limits<double>::quiet_NaN()) {}

      /// @brief Pure virtual destructor
      virtual ~BaseUnit() = 0;

      double GetValue() const { return value; }

      void SetValue(double val) { value = val; }

      //double GetPrimary()
      //{
         // TODO: Implement this to retrieve the value converted to the primary unit
         //return value;
      //}
      
   protected:
      double value = 0;
   };
   
   template<typename fromUnit, typename toUnit>
   toUnit ConvertValue(const fromUnit& obj)
   {
      if constexpr (std::is_same_v<fromUnit, toUnit>)
      {
         return obj;
      }
      else if constexpr (std::is_same_v<fromUnit::Dim, toUnit::Dim>)
      {
         //return toUnit{}; // Implement this

         //auto primary = obj.GetPrimary();

         //return ConvertValue<typename decltype(primary), toUnit>(primary);
         //return ConvertValue<decltype(obj.GetPrimary()), toUnit>(obj.GetPrimary()); // This is almost there, but not quite working.
         return ConvertValue<fromUnit::Primary, toUnit>(obj.GetPrimary()); // This is almost there, but not quite working.
      }
      else
      {
         static_assert(false, "No possible conversion for between these types."); // Try to print the type names, if possible without RTTI
      }
   }

   /*
   template<typename Unit>
   inline Unit ConvertValue(const Unit& obj) {
      return obj;
   }
   */

   /*
   template<>
   inline Feet ConvertValue<Meters, Feet>(const Meters& obj)
   {
      return Feet(obj.GetValue() * 3.28084);
   }
   */


   /// @brief Destructor implementation
   template<typename Unit>
   BaseUnit<Unit>::~BaseUnit() {}





   /// @brief A generic Dimension class
   /// @details This class represents a Dimension,
   ///    such as Length, Time, Speed, etc.
   ///    Its templates are used to set the dimensions of the numerator
   ///    and denominator, respectively. For example, a BaseDimension
   ///    may be templated on a tuple<LengthUnit> and a tuple<TimeUnit>.
   ///    This would still be a BaseDimension, but can be treated as "Speed".
   ///    This library may be used with BaseDimension alone, but Dimensions
   ///    may also be derived from BaseDimension for more readible code,
   ///    for example the Length, Time, and Speed classes provided.
   /// @tparam NumTuple A tuple of BaseUnits describing the dimension's numerator.
   /// @tparam DenTuple A tuple of BaseUnits describing the dimension's denominator.
   template<typename NumTuple, typename DenTuple>
   class BaseDimension
   {
   public:
      /// @brief Default constructor
      BaseDimension() : 
         value(1.0)
      {
      }

      /// @brief Constructor using existing dimension
      /// @details This constructor creates a new object matching an existing dimension
      /// @tparam T Type of given Dimension
      /// @param[in] dimension A Dimension object to copy
      /// @return A new Dimension object of type T
      template<typename T>
      explicit BaseDimension(const T& dimension) :
         value(dimension.value),
         numList(std::move(dimension.numList)),
         denList(std::move(dimension.denList))
      {
      }

      /// @brief Constructor explicitly given all values
      /// @details A constructor given all needed information.
      ///    This constructor should typically be used for creating new objects.
      /// @param[in] newValue The value to set
      /// @param[in] newNumList A tuple of BaseUnits to set as numerators
      /// @param[in] newDenList A tuple of BaseUnits to set as denominators
      BaseDimension(double newValue, NumTuple newNumList, DenTuple newDenList) :
         value(newValue),
         numList(newNumList),
         denList(newDenList)
      {
      }




      /// Constructor for doing arithemetic
      /// Make sure to make these const ref eventually
      template<typename ... NumTypes1, typename ... NumTypes2, typename ... DenTypes1, typename ... DenTypes2>
      BaseDimension(std::tuple<NumTypes1...> NumTuple1, std::tuple<NumTypes2...> NumTuple2, std::tuple<DenTypes1...> DenTuple1, std::tuple<DenTypes2...> DenTuple2) :
         value(1.0),
         //numList(StaticUnitSimplifier<std::tuple<NumTypes1...>, std::tuple<NumTypes2...>, std::tuple<DenTypes1...>, std::tuple<DenTypes2...>>::newNum{}),
         //denList(StaticUnitSimplifier<std::tuple<NumTypes1...>, std::tuple<NumTypes2...>, std::tuple<DenTypes1...>, std::tuple<DenTypes2...>>::newDen{})
         numList(),
         denList()
      {
         // Constructor logic
         

         /// Need to modify numList and denList to have the appropriate values, along with the scalar value for cancelled units
         StaticCancelUnits<0, false>(NumTuple1, numList, value);
         StaticCancelUnits<0, false>(NumTuple2, numList, value);
         StaticCancelUnits<0, true>(DenTuple1, denList, value);
         StaticCancelUnits<0, true>(DenTuple2, denList, value);
      }










      // TODO: Consider copy operator

      /// @brief Tuple of units corresponding to the Dimension numerator
      NumTuple numList;

      /// @brief Tuple of units corresponding to the Dimension denominator
      DenTuple denList;

      // TODO: UPDATE THIS DOXY
      /// @brief Return the internal value as a double in terms of the provided units
      /// @details Return the internal value after converting to the provided units.
      /// @param[in] i_numList tuple of BaseUnit pointers to convert to for the numerator
      /// @param[in] i_denList tuple of BaseUnit pointers to convert to for the denominator
      /// @return A double representing the value in terms of the given units
      template<typename NumTuple, typename DenTuple>
      double GetVal() const
      {
         double result = value;

         GetConvertedValue<0, false, NumTuple>(numList, result);
         GetConvertedValue<0, true, DenTuple>(denList, result);

         return result;
      }
      
      

      /*
      /// @brief += operator overload for another Dimension
      BaseDimension<NumTuple, DenTuple>& operator+=(const BaseDimension<NumTuple, DenTuple>& rhs)
      {
         value += rhs.GetVal(numList, denList);
         return *this;
      }

      /// @brief -= operator overload for another Dimension
      BaseDimension<NumTuple, DenTuple>& operator-=(const BaseDimension<NumTuple, DenTuple>& rhs)
      {
         value -= rhs.GetVal(numList, denList);
         return *this;
      }
      */

      /// @brief *= operator overload for a scalar
      BaseDimension<NumTuple, DenTuple>& operator*=(double rhs)
      {
         value *= rhs;
         return *this;
      }

      /// @brief /= operator overload for a scalar
      BaseDimension<NumTuple, DenTuple>& operator/=(double rhs)
      {
         value /= rhs;
         return *this;
      }
      


      
      
      // The following operators are explicitly deleted
      BaseDimension<NumTuple, DenTuple>& operator*=(const BaseDimension<NumTuple, DenTuple>& rhs) = delete; // Multiplication results in a different type
      BaseDimension<NumTuple, DenTuple>& operator/=(const BaseDimension<NumTuple, DenTuple>& rhs) = delete; // Division results in a different type
      BaseDimension<NumTuple, DenTuple>& operator+=(double rhs) = delete; // Addition cannot be performed between a Dimension and a scalar
      BaseDimension<NumTuple, DenTuple>& operator-=(double rhs) = delete; // Subtraction cannot be performed between a Dimension and a scalar

      // Comparison Operators
      template<typename CompNumTuple, typename CompDenTuple>
      bool operator>(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() > rhs.GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator<(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() < rhs.GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator>=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() >= rhs.GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator<=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() <= rhs.GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator==(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() == rhs.GetVal<NumTuple, DenTuple>(); } // I need to add a small tolerance to this
      
      template<typename CompNumTuple, typename CompDenTuple>
      bool operator!=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return !(*this == rhs); }
      
      

      // TODO: Define a NearlyEqual method with custom tolerance

   protected:
      /// @brief The value of the given object
      /// @details UNUSED - Keeping around while fixing implementation
      double value;
   private:


      /// @brief Private getter to retrieve the raw value
      /// @return The raw value
      double GetRawValue() const { return value; }

      // Declare operator overloads as friends of this class
      // This is to directly access the raw value for efficiency


      
      // TODO: Check later if these are still needed in the Static library...

      template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
      friend auto operator/(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2);

      template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
      friend auto operator*(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator*(const BaseDimension<NumTuple, DenTuple>& obj, double scalar);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator*(double scalar, const BaseDimension<NumTuple, DenTuple>& obj);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator/(const BaseDimension<NumTuple, DenTuple>& obj, double scalar);

      template<typename NumTuple, typename DenTuple>
      friend auto operator/(double scalar, const BaseDimension<NumTuple, DenTuple>& obj)->BaseDimension<DenTuple, NumTuple>;

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator+(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator-(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2);

      
      

   };
   

   /// @brief Division operator for two Dimensions
   /// @tparam NumTuple1 Tuple of numerator units of obj1
   /// @tparam DenTuple1 Tuple of denominator units of obj1
   /// @tparam NumTuple2 Tuple of numerator units of obj2
   /// @tparam DenTuple2 Tuple of denominator units of obj2
   /// @param[in] obj1 The numerator BaseDimension object
   /// @param[in] obj2 The denominator BaseDimension object
   /// @return A base dimension object templated on the numerator types and
   ///    the denominator types, then simplified.
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   auto operator/(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      using simplified = StaticUnitSimplifier<NumTuple1, DenTuple2, DenTuple1, NumTuple2>;
      return simplified::dimType(obj1.numList, obj2.denList, obj1.denList, obj2.numList);
   }
  
   /// @brief Multiplication operator for two Dimensions
   /// @tparam NumTuple1 Tuple of numerator units of obj1
   /// @tparam DenTuple1 Tuple of denominator units of obj1
   /// @tparam NumTuple2 Tuple of numerator units of obj2
   /// @tparam DenTuple2 Tuple of denominator units of obj2
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object templated on the types of both
   ///    input objects, then simplified.
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   auto operator*(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      using simplified = StaticUnitSimplifier<NumTuple1, NumTuple2, DenTuple1, DenTuple2>;
      return simplified::dimType(obj1.numList, obj2.numList, obj1.denList, obj2.denList);
   }
 
   // Scalar Math

   /// @brief Multiplication operator for a Dimension and scalar
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj The BaseDimension object
   /// @param[in] scalar The scalar value as a double
   /// @return A BaseDimension object of type matching obj, with value multiplied by scalar
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator*(const BaseDimension<NumTuple, DenTuple>& obj, double scalar)
   {
      return BaseDimension<NumTuple, DenTuple>(obj.GetRawValue() * scalar, obj.numList, obj.denList);
   }

   
   /// @brief Multiplication operator for a scalar and Dimension
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] scalar The scalar value as a double
   /// @param[in] obj The BaseDimension object
   /// @return A BaseDimension object of type matching obj, with value multiplied by scalar
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator*(double scalar, const BaseDimension<NumTuple, DenTuple>& obj)
   {
      return obj * scalar;
   }

   /// @brief Division operator for a Dimension and scalar
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj The BaseDimension object
   /// @param[in] scalar The scalar value as a double
   /// @return A BaseDimension object of type matching obj, with value divided by scalar
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator/(const BaseDimension<NumTuple, DenTuple>& obj, double scalar)
   {
      return BaseDimension<NumTuple, DenTuple>(obj.GetRawValue() / scalar, obj.numList, obj.denList);
   }

   /// @brief Division operator for a scalar and Dimension
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] scalar The scalar value as a double
   /// @param[in] obj The BaseDimension object
   /// @return A BaseDimension object with Unit parameters inverted relative
   ///    to obj, and with scalar divided by obj value as the new value
   template<typename NumTuple, typename DenTuple>
   auto operator/(double scalar, const BaseDimension<NumTuple, DenTuple>& obj) -> BaseDimension<DenTuple, NumTuple>
   {
      return BaseDimension<DenTuple, NumTuple>(scalar / obj.GetRawValue(), obj.denList, obj.numList);
   }
/*
   /// @brief Addition operator for two Dimensions
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object of type matching the inputs.
   ///    The value is the values of obj1 and obj2 added, after converting
   ///    obj2 to the same units as obj1
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator+(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2)
   {
      //return BaseDimension<NumTuple, DenTuple>(obj1.GetRawValue() + obj2.GetVal(obj1.numList, obj1.denList), obj1.numList, obj1.denList);


      BaseDimension<NumTuple, DenTuple>(1.0, // Need to add values within the units...

   }

   /// @brief Subtraction operator for two Dimensions
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object of type matching the inputs.
   ///    The value is the difference of values of obj1 and obj2, after 
   ///    converting obj2 to the same units as obj1
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator-(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2)
   {
      return BaseDimension<NumTuple, DenTuple>(obj1.GetRawValue() - obj2.GetVal(obj1.numList, obj1.denList), obj1.numList, obj1.denList);
   }
   */


   template<typename Unit, typename Primary>
   inline Primary GetPrimaryImpl(const Unit& obj)
   {
      if constexpr (std::is_same_v<Unit, Primary>)
      {
         return obj;
      }
      else
      {
         return ConvertValue<Unit, Primary>(obj);
      }
   }
}

// TODO: Implement SI conversions
// Keep this attempt as a starting point

/*
// Macro for SI prefixes

// Macro definition to create a prefixed factory function
#define SI_PREFIX(baseName, UnitType, Prefix) \
   inline static UnitType& Prefix##baseName() { \
      static std::string fullName = std::string(#Prefix) + #baseName; \
      UnitType& unit UnitTypeFactory::GetInstance(fullName); \
      return unit; \
    }

#define ALL_SI_PREFIXES(baseName, UnitType) \
SI_PREFIX(baseName, UnitType, Pico); \
SI_PREFIX(baseName, UnitType, Nano); \
SI_PREFIX(baseName, UnitType, Micro); \
SI_PREFIX(baseName, UnitType, Milli); \
SI_PREFIX(baseName, UnitType, Centi); \
SI_PREFIX(baseName, UnitType, Deci); \
SI_PREFIX(baseName, UnitType, Deca); \
SI_PREFIX(baseName, UnitType, Hepta); \
SI_PREFIX(baseName, UnitType, Kilo); \
SI_PREFIX(baseName, UnitType, Mega); \
SI_PREFIX(baseName, UnitType, Giga); \
SI_PREFIX(baseName, UnitType, Tera);

*/

#endif // DIMENSION_BASE_H