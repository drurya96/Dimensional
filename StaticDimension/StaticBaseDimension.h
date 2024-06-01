#ifndef STATIC_DIMENSION_BASE_H
#define STATIC_DIMENSION_BASE_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same
#include <utility> // For std::make_index_sequence // @todo move this to Utilities

#include "StaticDimensionUtilities.h"

namespace StaticDimension
{
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
      BaseUnit() = delete;

      /// @brief Used to handle subscripting
      /// @details Units only cancel if this value is the same.
      ///    This means, creating units with different IDs and combining them
      ///    into one dimension will prevent them from canelling out.
      constexpr static int ID = 0;
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

         CancelUnits<NumTuple, DenTuple, simplified::newNum, simplified::newDen, simplified::isDelta>(newScalar);

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

         constexpr bool isDelta = !((std::tuple_size_v<ToNumTuple> == 1) && (std::tuple_size_v<ToDenTuple> == 0));

         ConvertDimension<0, false, ToNumTuple, NumTuple, isDelta>(result);
         ConvertDimension<0, true, ToDenTuple, DenTuple, isDelta>(result);

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
      bool operator==(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() == rhs.GetVal<NumTuple, DenTuple>(); }
      
      template<typename CompNumTuple, typename CompDenTuple>
      bool operator!=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return !(*this == rhs); }
      
      /// @brief Check if this object is nearly equal to input object
      /// @param[in] rhs The object to compare to
      /// @param[in] Epsilon The acceptable difference between values, in terms of this object's dimension
      /// @return Bool indicating equality
      template<typename CompNumTuple, typename CompDenTuple>
      bool NearlyEqual(const BaseDimension<CompNumTuple, CompDenTuple>& rhs, PrecisionType Epsilon) const { return fabs(GetVal<NumTuple, DenTuple>() - rhs.GetVal<NumTuple, DenTuple>()) < Epsilon; }

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

      CancelUnits<NumTuple1, DenTuple2, simplified::newNum, simplified::newDen, simplified::isDelta>(newScalar);
      CancelUnits<DenTuple1, NumTuple2, simplified::newNum, simplified::newDen, simplified::isDelta>(newScalar);

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

      CancelUnits<NumTuple1, DenTuple1, simplified::newNum, simplified::newDen, simplified::isDelta>(newScalar);
      CancelUnits<NumTuple2, DenTuple2, simplified::newNum, simplified::newDen, simplified::isDelta>(newScalar);

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

   /// @brief Implementation for Pow (exponential)
   /// @details Uses exponentiation by squares method
   /// @tparam exponent the exponent to raise to. This must be a positive integer
   /// @tparam Dim The input dimension type
   /// @tparam TransitiveDim The dimension type returned from this recursive call of PowImpl
   /// @param[in] obj The input dimension object
   /// @param[in] transitiveDim The transitive object from the previous recursive call
   /// @return transitiveDim multiplied by obj either one or two times
   ///    for even or odd exponents, respectively.
   template<unsigned int exponent, typename Dim, typename TransitiveDim>
   auto PowImpl(const Dim& obj, const TransitiveDim& transitiveDim)
   {
      if constexpr (exponent == 0)
      {
         return TransitiveDim{0.0};
      }
      else if constexpr (exponent == 1)
      {
         return transitiveDim;
      }
      else if constexpr (exponent % 2 == 0)
      {
         auto halfPower = PowImpl<exponent / 2>(obj, transitiveDim);
         return halfPower * halfPower; // Equivalent to squaring the result
      }
      else
      {
         auto halfPower = PowImpl<exponent / 2>(obj, transitiveDim); // Note this is truncation division
         return halfPower * halfPower * obj; // Adjust for odd exponents by multiplying with obj once more
      }
   }

   /// @brief Implementation for Pow (exponential)
   /// @tparam exponent the exponent to raise to. This must be a positive integer
   /// @tparam Dim The input dimension type
   /// @param[in] obj The object to raise to exponent power
   /// @return A dimension object of units raised to exponent power,
   ///    and a value raised to exponent power
   template<unsigned int exponent, typename Dim>
   auto Pow(const Dim& obj)
   {
      return PowImpl<exponent>(obj, obj);
   }

   /// @brief Type traid to check if a type has a T::slope attribute
   template <typename, typename = std::void_t<>>
   struct has_slope : std::false_type {};

   /// @brief Type traid to check if a type has a T::slope attribute
   template <typename T>
   struct has_slope<T, std::void_t<decltype(T::slope)>> : std::integral_constant<bool, std::is_same_v<decltype(T::slope), const double>> {};

   /// @brief Type traid to check if a type has a T::offset attribute
   template <typename, typename = std::void_t<>>
   struct has_offset : std::false_type {};

   /// @brief Type traid to check if a type has a T::offset attribute
   template <typename T>
   struct has_offset<T, std::void_t<decltype(T::offset)>> : std::integral_constant<bool, std::is_same_v<decltype(T::offset), const double>> {};

   /// @brief Return the slope as a constexpr if one exists,
   ///    otherwise return 1.0
   template<typename T>
   constexpr PrecisionType GetSlope()
   {
      if constexpr (has_slope<T>::value)
      {
         return T::slope;
      }
      else
      {
         return 1.0;
      }
   }

   /// @brief Return the offset as a constexpr if one exists,
   ///    otherwise return 0.0
   template<typename T>
   constexpr PrecisionType GetOffset()
   {
      if constexpr (has_offset<T>::value)
      {
         return T::offset;
      }
      else
      {
         return 0.0;
      }
   }
   
   /// @brief Conversion implementation
   /// @details base case when no conversion is defined, throws a compile-time error.
   template<typename fromUnit, typename toUnit, typename Enable = void>
   struct ConversionBase
   {
      static_assert(sizeof(fromUnit) == -1, "Conversion not defined for these units.");
   };

   /// @brief Struct defining the linear relationship between two units
   /// @details This relationship is meant to be specialized for each unit,
   ///    and by users when extensions are needed.
   template<typename fromUnit, typename toUnit>
   struct Conversion : ConversionBase<fromUnit, toUnit> {};

   /// @brief Struct defining the linear relationship between two units
   /// @details This specialization will be used when a final specialization is not provided, but the dimensions
   ///    are the same. This will step through the primary unit of this dimension.
   /// @typedef slope The linear relationship between the units
   /// @typedef offset The intercept between two units.
   ///    This will typically be 0.0, but there are some special cases, such as temparature.
   /// @todo Provide some insight to the user when this is used, so they may accurately
   ///    define useful specializations.
   template<typename fromUnit, typename toUnit>
   struct ConversionBase<fromUnit, toUnit, std::enable_if_t<std::is_same_v<typename fromUnit::Dim, typename toUnit::Dim>>>
   {
      using toPrimary = Conversion<fromUnit, typename fromUnit::Primary>;
      using fromPrimary = Conversion<typename fromUnit::Primary, toUnit>;

      static constexpr PrecisionType slope = GetSlope<toPrimary>() * GetSlope<fromPrimary>();
      static constexpr PrecisionType offset = GetOffset<toPrimary>() + GetOffset<fromPrimary>();

   };

   /// @brief Struct defining the linear relationship between two units
   /// @details This relationship is meant to be specialized for each unit,
   ///    and by users when extensions are needed.
   ///    This specialization is used when converting to the same unit.
   template<typename Unit>
   struct Conversion<Unit, Unit>
   {
      static constexpr PrecisionType slope = 1.0;
      static constexpr PrecisionType offset = 0.0;
   };
  
   /// @brief Method to convert a value of fromUnit to a value of toUnit
   /// @details This method relies on the slope and offset of the necessary conversion.
   ///    While this method can be specialized to bypass the slope-offset relationship,
   ///    this should be done with great care as other parts of this library make assumptions
   ///    about this relationship.
   /// @tparam fromUnit Unit to convert from
   /// @tparam toUnit Unit to convert to
   /// @tparam isDelata Bool indicating whether this conversion is of a single unit in the normator (false)
   ///    or not (true).
   /// @tparam inverse Indicates whether value is in denominator (true) or numerator (false)
   /// @param[in] input Value to convert as a floating-point type
   /// @return floating-point type after conversion
   template<typename fromUnit, typename toUnit, bool isDelta = false, bool inverse = false>
   PrecisionType Convert(PrecisionType input)
   {
      if constexpr (std::is_same_v<fromUnit, toUnit>)
      {
         return input;
      }
      else if constexpr (std::is_same_v<fromUnit::Dim, toUnit::Dim>)
      {         
         // Do conversion using Conversion struct
         using conv = Conversion<fromUnit, toUnit>;
         constexpr PrecisionType slope = GetSlope<conv>();
         constexpr PrecisionType offset = GetOffset<conv>();
         if constexpr (isDelta)
         {
            if constexpr (inverse)
            {
               return (input / slope);
            }
            else
            {
               return (input * slope);
            }
            
         }
         else
         {
            return (input * slope) + offset;
         }
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