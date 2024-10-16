#ifndef STATIC_DIMENSION_BASE_H
#define STATIC_DIMENSION_BASE_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same
#include <utility> // For std::make_index_sequence // @todo move this to Utilities
#include <cmath> // For std::hypot, std::modf, std::fmod // @todo move this to Utilities

#include "DimensionUtilities.h"

namespace Dimension
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
   template<typename NumTupleT, typename DenTupleT>
   class BaseDimension
   {
   public:

      // Expand any derived units to their fundamental constiuents
      using Extractor = FundamentalUnitExtractor<NumTupleT, DenTupleT>;
      using NumTuple = typename Extractor::Num;
      using DenTuple = typename Extractor::Den;

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

         CancelUnits<typename simplified::numSimple, typename simplified::denSimple, typename simplified::newNum, typename simplified::newDen, simplified::isDelta>(newScalar);

         return typename simplified::dimType(newScalar);
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
      
      /// @brief Cast to double operator overload for Scalar types
      /// @details Cast the dimension to a double if unitless (i.e. scalar type) 
      template<typename T = NumTupleT, typename U = DenTupleT>
      requires (std::tuple_size_v<T> == 0 && std::tuple_size_v<U> == 0)
      operator double() const
      {
         return scalar;
      }

      /// @brief += operator overload for another Dimension
      /// @tparam NumTuple2 Tuple of numerator types of object being added
      /// @tparam DenTuple2 Tuple of denominator types of object being added
      /// @param[in] rhs The object being added
      template<typename NumTuple2, typename DenTuple2>
      BaseDimension<NumTuple, DenTuple>& operator+=(const BaseDimension<NumTuple2, DenTuple2>& rhs)
      {
         scalar += rhs.template GetVal<NumTuple, DenTuple>();
         return *this;
      }
      
      /// @brief -= operator overload for another Dimension
      /// @tparam NumTuple2 Tuple of numerator types of object being subtracted
      /// @tparam DenTuple2 Tuple of denominator types of object being substracted
      /// @param[in] rhs The object being substracted
      template<typename NumTuple2, typename DenTuple2>
      BaseDimension<NumTuple, DenTuple>& operator-=(const BaseDimension<NumTuple2, DenTuple2>& rhs)
      {
         scalar -= rhs.template GetVal<NumTuple, DenTuple>();
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
      bool operator>(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() > rhs.template GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator<(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() < rhs.template GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator>=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() >= rhs.template GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator<=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() <= rhs.template GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      bool operator==(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() == rhs.template GetVal<NumTuple, DenTuple>(); }
      
      template<typename CompNumTuple, typename CompDenTuple>
      bool operator!=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return !(*this == rhs); }
      
      /// @brief Check if this object is nearly equal to input object
      /// @param[in] rhs The object to compare to
      /// @param[in] Epsilon The acceptable difference between values, in terms of this object's dimension
      /// @return Bool indicating equality
      template<typename CompNumTuple, typename CompDenTuple>
      bool NearlyEqual(const BaseDimension<CompNumTuple, CompDenTuple>& rhs, PrecisionType Epsilon) const { return fabs(GetVal<NumTuple, DenTuple>() - rhs.template GetVal<NumTuple, DenTuple>()) < Epsilon; }

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

      template<typename NumTup, typename DenTup>
      friend BaseDimension<NumTup, DenTup> operator*(const BaseDimension<NumTup, DenTup>& obj, PrecisionType scalar);

      template<typename NumTup, typename DenTup>
      friend BaseDimension<NumTup, DenTup> operator*(PrecisionType scalar, const BaseDimension<NumTup, DenTup>& obj);

      template<typename NumTup, typename DenTup>
      friend BaseDimension<NumTup, DenTup> operator/(const BaseDimension<NumTup, DenTup>& obj, PrecisionType scalar);

      template<typename NumTup, typename DenTup>
      friend auto operator/(PrecisionType scalar, const BaseDimension<NumTup, DenTup>& obj)->BaseDimension<DenTup, NumTup>;

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
      using Num1 = typename BaseDimension<NumTuple1, DenTuple1>::NumTuple;
      using Num2 = typename BaseDimension<NumTuple2, DenTuple2>::NumTuple;

      using Den1 = typename BaseDimension<NumTuple1, DenTuple1>::DenTuple;
      using Den2 = typename BaseDimension<NumTuple2, DenTuple2>::DenTuple;

      using simplified = UnitSimplifier<Num1, Den2, Den1, Num2>;
      PrecisionType newScalar = obj1.scalar / obj2.scalar;

      CancelUnits<typename simplified::numSimple, typename simplified::denSimple, typename simplified::newNum, typename simplified::newDen, simplified::isDelta>(newScalar);

      return typename simplified::dimType(newScalar);
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
      using Num1 = typename BaseDimension<NumTuple1, DenTuple1>::NumTuple;
      using Num2 = typename BaseDimension<NumTuple2, DenTuple2>::NumTuple;

      using Den1 = typename BaseDimension<NumTuple1, DenTuple1>::DenTuple;
      using Den2 = typename BaseDimension<NumTuple2, DenTuple2>::DenTuple;

      using simplified = UnitSimplifier<Num1, Num2, Den1, Den2>;
      PrecisionType newScalar = obj1.scalar * obj2.scalar;

      CancelUnits<typename simplified::numSimple, typename simplified::denSimple, typename simplified::newNum, typename simplified::newDen, simplified::isDelta>(newScalar);

      return typename simplified::dimType(newScalar);
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
      return BaseDimension<NumTuple1, DenTuple1>{ obj1.template GetVal<NumTuple1, DenTuple1>() + obj2.template GetVal<NumTuple1, DenTuple1>() };
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
      return BaseDimension<NumTuple1, DenTuple1>{ obj1.template GetVal<NumTuple1, DenTuple1>() - obj2.template GetVal<NumTuple1, DenTuple1>() };
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

   using Scalar = BaseDimension<std::tuple<>, std::tuple<>>;

   template<typename T>
   T hypot(const T& obj1, const T& obj2)
   {
      return T(std::hypot(obj1.GetVal<typename T::NumTuple, typename T::DenTuple>(), obj2.GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

   template<typename T>
   T abs(const T& obj)
   {
      return T(std::abs(obj.GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

   template<typename T>
   T floor(const T& obj)
   {
      return T(std::floor(obj.GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

   template<typename T>
   T ceil(const T& obj)
   {
      return T(std::ceil(obj.GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

   template<typename T>
   T round(const T& obj)
   {
      return T(std::round(obj.GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

   template<typename T>
   T modf(const T& obj, T* intPart = nullptr)
   {
      double intPartDouble;
      double fracPart = std::modf(obj.GetVal<typename T::NumTuple, typename T::DenTuple>(), &intPartDouble);

      if (intPart) {
         *intPart = T(intPartDouble);
      }

      return T(fracPart);
   }

   template<typename T>
   T fmod(const T& dividend, const T& divisor)
   {
      if (divisor.GetVal<typename T::NumTuple, typename T::DenTuple>() == 0) {
         throw std::invalid_argument("Divisor cannot be zero.");
      }

      return T(std::fmod(dividend.GetVal<typename T::NumTuple, typename T::DenTuple>(), divisor.GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

}

// Macro for SI prefixes

struct Pico {};
struct Nano {};
struct Micro {};
struct Milli {};
struct Centi {};
struct Deci {};
struct Deca {};
struct Hecto {};
struct Kilo {};
struct Mega {};
struct Giga {};
struct Tera {};

// SI prefix factors
template <typename Prefix>
struct SIFactor;

#define DEFINE_SI_FACTOR(Prefix, Factor) \
template <> struct SIFactor<Prefix> { \
    static constexpr double value = Factor; \
};

// Define the conversion factors for SI prefixes
DEFINE_SI_FACTOR(Pico, 1e-12)
DEFINE_SI_FACTOR(Nano, 1e-9)
DEFINE_SI_FACTOR(Micro, 1e-6)
DEFINE_SI_FACTOR(Milli, 1e-3)
DEFINE_SI_FACTOR(Centi, 1e-2)
DEFINE_SI_FACTOR(Deci, 1e-1)
DEFINE_SI_FACTOR(Deca, 1e1)
DEFINE_SI_FACTOR(Hecto, 1e2)
DEFINE_SI_FACTOR(Kilo, 1e3)
DEFINE_SI_FACTOR(Mega, 1e6)
DEFINE_SI_FACTOR(Giga, 1e9)
DEFINE_SI_FACTOR(Tera, 1e12)

// Macro definition to create a prefixed factory function
#define SI_PREFIX(baseName, UnitType, Prefix) \
   struct Prefix##baseName : public UnitType<Prefix##baseName> { public: using UnitType::UnitType; }; \
   template<> struct Conversion<baseName, Prefix##baseName> { static constexpr PrecisionType slope = 1.0 / SIFactor<Prefix>::value; }; \
   template<> struct Conversion<Prefix##baseName, baseName> { static constexpr PrecisionType slope = SIFactor<Prefix>::value; };

#define ALL_SI_PREFIXES(baseName, UnitType) \
SI_PREFIX(baseName, UnitType, Pico); \
SI_PREFIX(baseName, UnitType, Nano); \
SI_PREFIX(baseName, UnitType, Micro); \
SI_PREFIX(baseName, UnitType, Milli); \
SI_PREFIX(baseName, UnitType, Centi); \
SI_PREFIX(baseName, UnitType, Deci); \
SI_PREFIX(baseName, UnitType, Deca); \
SI_PREFIX(baseName, UnitType, Hecto); \
SI_PREFIX(baseName, UnitType, Kilo); \
SI_PREFIX(baseName, UnitType, Mega); \
SI_PREFIX(baseName, UnitType, Giga); \
SI_PREFIX(baseName, UnitType, Tera);



#endif // STATIC_DIMENSION_BASE_H
