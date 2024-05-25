#ifndef STATIC_DIMENSION_BASE_H
#define STATIC_DIMENSION_BASE_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same
#include <utility> // For std::make_index_sequence // @todo move this to Utilities

#include "StaticDimensionUtilities.h"

namespace StaticDimension
{
   /// @brief An arbitrary value to use when determining if two dimensions are equal
   /// @details When checking if two units are equal, they are considered equal if the difference
   ///    is less than this arbitrary value
   /// @todo IMPORTANT! Find a better way.. this is only a placeholder to continue development.
   const PrecisionType PLACEHOLDER_EPSILON = 0.001;

   /// @brief A base class representing a unit
   /// @details This abstract class represents a Unit,
   ///    such as Meters, Seconds, Grams, etc.
   struct BaseUnit
   {
   public:

      /// @brief Default constructor
      /// @todo delete this function so units cannot be constructed
      ///    For now, some template metaprogramming relying on decltype
      ///    requires a default constructor.
      BaseUnit() {}
   };

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
   ///    Finally, the dimension itself contains a scalar value which is typically
   ///    used when constructing an object without existing units, or when simplifying.
   /// @tparam NumTuple A tuple of BaseUnits describing the dimension's numerator.
   ///    Note all types in NumTuple must derive from BaseUnit
   /// @tparam DenTuple A tuple of BaseUnits describing the dimension's denominator.
   ///    Note all types in DenTuple must derive from BaseUnit
   template<typename NumTuple, typename DenTuple>
   class BaseDimension
   {
   public:
      // Enforce units deriving from BaseUnit
      static_assert(is_unit_tuple<NumTuple>::value, "NumTuple contains a type not derived from BaseUnit");
      static_assert(is_unit_tuple<DenTuple>::value, "DenTuple contains a type not derived from BaseUnit");

      /// @brief Default constructor
      BaseDimension() : 
         scalar(1.0)
      {
      }

      /// @brief Constructor given value
      BaseDimension(PrecisionType val) :
         scalar(val)
      {
      }

      /// @brief Return an equivalent dimension with all units simplified
      /// @return Simplified dimension
      auto Simplify()
      {
         using simplified = UnitSimplifier<NumTuple, std::tuple<>, std::tuple<>, DenTuple>;
         PrecisionType newScalar = scalar;

         CancelUnits<NumTuple, DenTuple, simplified::newNum, simplified::newDen>(newScalar);

         return simplified::dimType(newScalar);
      }

      /// @brief Return the internal value as a double in terms of the provided units
      /// @tparam NumTuple tuple of Unit types to convert numerator to
      /// @tparam DenTuple tuple of Unit types to convert denominator to
      /// @return A PrecisionType representing the value in terms of the given units
      template<typename ToNumTuple, typename ToDenTuple>
      PrecisionType GetVal() const
      {
         PrecisionType result = scalar;

         ConvertDimension<0, false, ToNumTuple, NumTuple>(result);
         ConvertDimension<0, true, ToDenTuple, DenTuple>(result);

         return result;
      }
      
      /// @brief += operator overload for another Dimension
      /// @tparam NumTuple2 Tuple of numerator types of object being added
      /// @tparam DenTuple2 Tuple of denominator types of object being added
      /// @param[in] rhs The object being added
      template<typename NumTuple2, typename DenTuple2>
      BaseDimension<NumTuple, DenTuple>& operator+=(const BaseDimension<NumTuple2, DenTuple2>& rhs)
      {
         scalar += rhs.GetVal<NumTuple, DenTuple>();
         return *this;
      }
      
      /// @brief -= operator overload for another Dimension
      /// @tparam NumTuple2 Tuple of numerator types of object being subtracted
      /// @tparam DenTuple2 Tuple of denominator types of object being substracted
      /// @param[in] rhs The object being substracted
      template<typename NumTuple2, typename DenTuple2>
      BaseDimension<NumTuple, DenTuple>& operator-=(const BaseDimension<NumTuple2, DenTuple2>& rhs)
      {
         scalar -= rhs.GetVal<NumTuple, DenTuple>();
         return *this;
      }

      /// @brief *= operator overload for a scalar
      /// @param[in] rhs scalar value to multiply by
      BaseDimension<NumTuple, DenTuple>& operator*=(PrecisionType rhs)
      {
         scalar *= rhs;
         return *this;
      }

      /// @brief /= operator overload for a scalar
      /// @param[in] rhs scalar value to divide by
      BaseDimension<NumTuple, DenTuple>& operator/=(PrecisionType rhs)
      {
         scalar /= rhs;
         return *this;
      }
      
      // The following operators are explicitly deleted
      BaseDimension<NumTuple, DenTuple>& operator*=(const BaseDimension<NumTuple, DenTuple>& rhs) = delete; // Multiplication results in a different type
      BaseDimension<NumTuple, DenTuple>& operator/=(const BaseDimension<NumTuple, DenTuple>& rhs) = delete; // Division results in a different type
      BaseDimension<NumTuple, DenTuple>& operator+=(PrecisionType rhs) = delete; // Addition cannot be performed between a Dimension and a scalar
      BaseDimension<NumTuple, DenTuple>& operator-=(PrecisionType rhs) = delete; // Subtraction cannot be performed between a Dimension and a scalar

      // Comparison Operators
      /// @todo Replace these with <==> operator after switching to C++20
      template<typename CompNumTuple, typename CompDenTuple>
      bool operator>(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() > rhs.GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator<(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() < rhs.GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator>=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() >= rhs.GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator<=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() <= rhs.GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator==(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return fabs(GetVal<NumTuple, DenTuple>() - rhs.GetVal<NumTuple, DenTuple>()) < PLACEHOLDER_EPSILON; }
      
      template<typename CompNumTuple, typename CompDenTuple>
      bool operator!=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return !(*this == rhs); }
      
      // TODO: Define a NearlyEqual method with custom tolerance

   protected:
      /// @brief The scalar value of this dimension
      PrecisionType scalar;

   private:
      // Declare operator overloads as friends of this class
      // This is to directly access the raw value for efficiency

      template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
      friend auto operator/(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2);

      template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
      friend auto operator*(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator*(const BaseDimension<NumTuple, DenTuple>& obj, PrecisionType scalar);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator*(PrecisionType scalar, const BaseDimension<NumTuple, DenTuple>& obj);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator/(const BaseDimension<NumTuple, DenTuple>& obj, PrecisionType scalar);

      template<typename NumTuple, typename DenTuple>
      friend auto operator/(PrecisionType scalar, const BaseDimension<NumTuple, DenTuple>& obj)->BaseDimension<DenTuple, NumTuple>;

      template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
      friend BaseDimension<NumTuple1, DenTuple1> operator+(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2);

      template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
      friend BaseDimension<NumTuple1, DenTuple1> operator-(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2);


      // Making all BaseDimensions friends of one-another for access to scalars and
      //    unit list information during construction of new objects
      template<typename NumTupleOther, typename DenTupleOther>
      friend class BaseDimension;
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
      using simplified = UnitSimplifier<NumTuple1, DenTuple2, DenTuple1, NumTuple2>;
      PrecisionType newScalar = obj1.scalar / obj2.scalar;

      CancelUnits<NumTuple1, DenTuple2, simplified::newNum, simplified::newDen>(newScalar);
      CancelUnits<DenTuple1, NumTuple2, simplified::newNum, simplified::newDen>(newScalar);

      return simplified::dimType(newScalar);
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
      using simplified = UnitSimplifier<NumTuple1, NumTuple2, DenTuple1, DenTuple2>;
      PrecisionType newScalar = obj1.scalar * obj2.scalar;

      CancelUnits<NumTuple1, DenTuple1, simplified::newNum, simplified::newDen>(newScalar);
      CancelUnits<NumTuple2, DenTuple2, simplified::newNum, simplified::newDen>(newScalar);

      return simplified::dimType(newScalar);
   }
 
   // Scalar Math

   /// @brief Multiplication operator for a Dimension and scalar
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj The BaseDimension object
   /// @param[in] scalar The scalar value as a double
   /// @return A BaseDimension object of type matching obj, with value multiplied by scalar
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator*(const BaseDimension<NumTuple, DenTuple>& obj, PrecisionType scalar)
   {
      return BaseDimension<NumTuple, DenTuple>(obj.scalar * scalar);
   }
   
   /// @brief Multiplication operator for a scalar and Dimension
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] scalar The scalar value as a double
   /// @param[in] obj The BaseDimension object
   /// @return A BaseDimension object of type matching obj, with value multiplied by scalar
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator*(PrecisionType scalar, const BaseDimension<NumTuple, DenTuple>& obj)
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
   BaseDimension<NumTuple, DenTuple> operator/(const BaseDimension<NumTuple, DenTuple>& obj, PrecisionType scalar)
   {
      return BaseDimension<NumTuple, DenTuple>(obj.scalar / scalar);
   }

   /// @brief Division operator for a scalar and Dimension
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] scalar The scalar value as a double
   /// @param[in] obj The BaseDimension object
   /// @return A BaseDimension object with Unit parameters inverted relative
   ///    to obj, and with scalar divided by obj value as the new value
   template<typename NumTuple, typename DenTuple>
   auto operator/(PrecisionType scalar, const BaseDimension<NumTuple, DenTuple>& obj) -> BaseDimension<DenTuple, NumTuple>
   {
      return BaseDimension<DenTuple, NumTuple>(scalar / obj.scalar);
   }

   /// @brief Addition operator for two Dimensions
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object of type matching the inputs.
   ///    The value is the values of obj1 and obj2 added, after converting
   ///    obj2 to the same units as obj1
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   BaseDimension<NumTuple1, DenTuple1> operator+(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      return BaseDimension<NumTuple1, DenTuple1>{ obj1.GetVal<NumTuple1, DenTuple1>() + obj2.GetVal<NumTuple1, DenTuple1>() };
   }

   /// @brief Subtraction operator for two Dimensions
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object of type matching the inputs.
   ///    The value is the difference of values of obj1 and obj2, after 
   ///    converting obj2 to the same units as obj1
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   BaseDimension<NumTuple1, DenTuple1> operator-(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      return BaseDimension<NumTuple1, DenTuple1>{ obj1.GetVal<NumTuple1, DenTuple1>() - obj2.GetVal<NumTuple1, DenTuple1>() };
   }

   /// @brief Convert one unit to another TODO: Check this doxygen
   /// @details Return a toUnit type equivalent to the given
   ///    object of type fromUnit. This will fail at compile-time
   ///    if the to units do not have a common Dim field.
   ///    Further, attempting to convert to the same type will simply 
   ///    return a copy of the same object.
   /// @tparam fromUnit Unit to convert from, deduced from obj
   /// @tparam toUnit Unit to convert to, must be provided explicitly
   /// @param[in] obj Object to convert
   /// @todo Return the object by reference, if possible.
   /// @todo After switching to C++20, use a concept to inforce Dim field
   /// @todo Attempt to print a more meaningful compile-time error when
   ///    conversion is not possible, including the units being converted.
   ///    Avoid RTTI for this task.
   template<typename fromUnit, typename toUnit>
   PrecisionType Convert(PrecisionType input)
   {
      if constexpr (std::is_same_v<fromUnit, toUnit>)
      {
         return input;
      }
      else if constexpr (std::is_same_v<fromUnit::Dim, toUnit::Dim>)
      {
         return Convert<fromUnit::Primary, toUnit>(Convert<fromUnit, fromUnit::Primary>(input));
      }
      else
      {
         static_assert(false, "No possible conversion for between these types.");
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

#endif // STATIC_DIMENSION_BASE_H