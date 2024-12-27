#ifndef STATIC_DIMENSION_BASE_H
#define STATIC_DIMENSION_BASE_H

#include <tuple> // For std::tuple and related functions
#include <cmath> // For std::hypot, std::modf, std::fmod // @todo move this to Utilities
#include <stdexcept> // For std::invalid_argument
//#include <algorithm>

#include "Dimension_Core/PrecisionType.h"
#include "Dimension_Core/UnitValidation.h"
#include "Dimension_Core/UnitSimplifier.h"
#include "Dimension_Core/FundamentalUnitExtractor.h"
#include "Dimension_Core/Conversion.h"
#include "Dimension_Core/SI_Macro.h"
#include "Dimension_Core/Hashing.h"
#include "Dimension_Core/StringLiteral.h"
#include "Dimension_Core/Stream.h"
#include "Dimension_Core/Serialization.h"

namespace Dimension
{

   /// @brief A base class representing a unit
   /// @details This abstract class represents a Unit,
   ///    such as Meters, Seconds, Grams, etc.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, StringLiteral DimName>
   struct BaseUnit
   {
   public:
      /// @brief No constructor
      BaseUnit() = delete;

      using NumTuple = std::tuple<Unit>;
      using DenTuple = std::tuple<>;

      /// @brief Used to handle subscripting
      /// @details Units only cancel if this value is the same.
      ///    This means, creating units with different IDs and combining them
      ///    into one dimension will prevent them from canelling out.
      constexpr static int ID = 0;

      static constexpr StringLiteral<Name.size> name = Name;
      static constexpr StringLiteral<Abbreviation.size> abbr = Abbreviation;
      static constexpr StringLiteral<DimName.size> dimName = DimName;

      static constexpr StringLiteral<3> delim = "::"; // Size three due to null terminator

      static constexpr StringLiteral<DimName.size + delim.size - 1> test = concat(dimName, delim); // size - 1 to account for removed null terminator from first param
      static constexpr StringLiteral<test.size + name.size - 1> qualifiedName = concat(test, name); // size - 1 to account for removed null terminator from first param

      using name_type = decltype(Name);
      using abbr_type = decltype(Abbreviation);
      using dimName_type = decltype(DimName);
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
   requires IsUnitTuplePair<NumTupleT, DenTupleT>
   class BaseDimension
   {
   public:
      
      // Expand any derived units to their fundamental constiuents
      using Extractor = FundamentalUnitExtractor<NumTupleT, DenTupleT>;
      using NumTuple = typename Extractor::Num;
      using DenTuple = typename Extractor::Den; 

   private:

      using simplified = UnitSimplifier<NumTuple, std::tuple<>, std::tuple<>, DenTuple>;

   public:

      using simplifiedNumTuple = typename simplified::newNum;
      using simplifiedDenTuple = typename simplified::newDen;

      /// @brief Default constructor
      constexpr BaseDimension() : 
         scalar(1.0)
      {
      }

      /// @brief Constructor given value
      constexpr BaseDimension(PrecisionType val) :
         scalar(val)
      {
      }

      template<typename T, typename U>
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<T, U>>
      constexpr BaseDimension(BaseDimension<T, U> obj) :
         BaseDimension(obj.template GetVal<NumTuple, DenTuple>())
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

      PrecisionType SimplifiedScalar() const
      {
         using simplified = UnitSimplifier<NumTuple, std::tuple<>, std::tuple<>, DenTuple>;
         PrecisionType newScalar = scalar;

         CancelUnits<typename simplified::numSimple, typename simplified::denSimple, typename simplified::newNum, typename simplified::newDen, simplified::isDelta>(newScalar);

         return newScalar;
      }

      /// @brief Return the internal value as a double in terms of the provided units
      /// @tparam NumTuple tuple of Unit types to convert numerator to
      /// @tparam DenTuple tuple of Unit types to convert denominator to
      /// @return A PrecisionType representing the value in terms of the given units
      template<typename ToNumTuple, typename ToDenTuple>
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<ToNumTuple, ToDenTuple>>
      PrecisionType GetVal() const
      {
         PrecisionType result = this->SimplifiedScalar();

         constexpr bool isDelta = true;

         ConvertDimension<0, false, ToNumTuple, simplifiedNumTuple, isDelta>(result);
         ConvertDimension<0, true, ToDenTuple, simplifiedDenTuple, isDelta>(result);

         return result;
      }

      /// @brief Set the value using given units
      /// @details Set value of scalar for an existing object using given dimension.
      /// @tparam FromNumTuple Numerator tuple corresponding to the value to set
      /// @tparam FromDenTuple Denominator tuple corresponding to the value to set
      /// @param newVal Value to set
      template<typename FromNumTuple, typename FromDenTuple>
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<FromNumTuple, FromDenTuple>>
      void SetVal(PrecisionType newVal)
      {
         constexpr bool isDelta = !((std::tuple_size_v<FromNumTuple> == 1) && (std::tuple_size_v<FromDenTuple> == 0));

         ConvertDimension<0, false, NumTuple, FromNumTuple, isDelta>(newVal);
         ConvertDimension<0, true, DenTuple, FromDenTuple, isDelta>(newVal);

         scalar = newVal;
      }
      
      /// @brief Cast to double operator overload for Scalar types
      /// @details Cast the dimension to a double if unitless (i.e. scalar type) 
      template<typename T = NumTupleT, typename U = DenTupleT>
      requires (std::tuple_size_v<T> == 0 && std::tuple_size_v<U> == 0)
      operator double() const
      {
         return scalar;
      }

      /// @brief Negative unary operator
      /// @return BaseDimension of the same type with opposite sign
      BaseDimension<NumTuple, DenTuple> operator-() const
      {
         return BaseDimension<NumTuple, DenTuple>(-scalar);
      }

      /// @brief += operator overload for another Dimension
      /// @tparam NumTuple2 Tuple of numerator types of object being added
      /// @tparam DenTuple2 Tuple of denominator types of object being added
      /// @param[in] rhs The object being added
      template<typename NumTuple2, typename DenTuple2>
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<NumTuple2, DenTuple2>>
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
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<NumTuple2, DenTuple2>>
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
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<CompNumTuple, CompDenTuple>>
      bool operator>(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() > rhs.template GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<CompNumTuple, CompDenTuple>>
      bool operator<(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() < rhs.template GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<CompNumTuple, CompDenTuple>>
      bool operator>=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() >= rhs.template GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<CompNumTuple, CompDenTuple>>
      bool operator<=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() <= rhs.template GetVal<NumTuple, DenTuple>(); }

      template<typename CompNumTuple, typename CompDenTuple>
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<CompNumTuple, CompDenTuple>>
      bool operator==(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return GetVal<NumTuple, DenTuple>() == rhs.template GetVal<NumTuple, DenTuple>(); }
      
      template<typename CompNumTuple, typename CompDenTuple>
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<CompNumTuple, CompDenTuple>>
      bool operator!=(const BaseDimension<CompNumTuple, CompDenTuple>& rhs) const { return !(*this == rhs); }
      
      /// @brief Check if this object is nearly equal to input object
      /// @param[in] rhs The object to compare to
      /// @param[in] Epsilon The acceptable difference between values, in terms of this object's dimension
      /// @return Bool indicating equality
      template<typename CompNumTuple, typename CompDenTuple>
      requires MatchingDimensionsNew<BaseDimension<NumTuple, DenTuple>, BaseDimension<CompNumTuple, CompDenTuple>>
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
      requires MatchingDimensionsNew<BaseDimension<NumTuple1, DenTuple1>, BaseDimension<NumTuple2, DenTuple2>>
      friend auto operator+(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2);

      template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
      requires MatchingDimensionsNew<BaseDimension<NumTuple1, DenTuple1>, BaseDimension<NumTuple2, DenTuple2>>
      friend auto operator-(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2);


      // Making all BaseDimensions friends of one-another for access to scalars and
      //    unit list information during construction of new objects
      template<typename NumTupleOther, typename DenTupleOther>
      requires IsUnitTuplePair<NumTupleOther, DenTupleOther>
      friend class BaseDimension;
   };

/*
   /// @brief Return the internal value as a double in terms of the object unit templates
   /// @return A PrecisionType representing the value in terms of the given units
   template<typename NumTupleT, typename DenTupleT>
   template<>
   //requires MatchingDimensions<NumTupleT, NumTupleT> && MatchingDimensions<DenTupleT, DenTupleT>
   requires true
   PrecisionType BaseDimension<NumTupleT, DenTupleT>::GetVal<NumTupleT, DenTupleT>() const
   {
      return scalar;
   }
*/
/*
      template<>
      void SetVal<NumTuple, DenTuple>(PrecisionType newVal)
      {
         scalar = newVal;
      }
*/

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
      using absolute_quantity_tuple1 = make_tuple_of_single_absolute_quantity<NumTuple1>;
      using Num1 = std::conditional_t<absolute_quantity_tuple1::value, typename absolute_quantity_tuple1::type, typename BaseDimension<NumTuple1, DenTuple1>::NumTuple>;

      using absolute_quantity_tuple2 = make_tuple_of_single_absolute_quantity<NumTuple2>;
      using Num2 = std::conditional_t<absolute_quantity_tuple2::value, typename absolute_quantity_tuple2::type, typename BaseDimension<NumTuple2, DenTuple2>::NumTuple>;

      using Den1 = typename BaseDimension<NumTuple1, DenTuple1>::DenTuple;
      using Den2 = typename BaseDimension<NumTuple2, DenTuple2>::DenTuple;

      using simplified = UnitSimplifier<Num1, Den2, Den1, Num2>;

      PrecisionType scalar1 = obj1.scalar;
      PrecisionType scalar2 = obj2.scalar;

      if constexpr (absolute_quantity_tuple1::value)
      {
         // This emits a compiler warning (deprecation warning) if an implicit cast occurs and persists in the resulting dimension.
         // Generally, users should make these casts explicitly if the goal is to maintain the unit.
         // However, if the implicit cast occurs only to "cancel" an existing unit, this is typical and does NOT emit a warning.
         implicit_cast_to_build_warning<has_same_dim<std::tuple_element_t<0, Num1>, typename simplified::newNum>::value>::call();
         scalar1 = Convert<std::tuple_element_t<0, NumTuple1>, std::tuple_element_t<0, Num1>>(scalar1);
      }

      if constexpr (absolute_quantity_tuple2::value)
      {
         // This emits a compiler warning (deprecation warning) if an implicit cast occurs and persists in the resulting dimension.
         // Generally, users should make these casts explicitly if the goal is to maintain the unit.
         // However, if the implicit cast occurs only to "cancel" an existing unit, this is typical and does NOT emit a warning.
         implicit_cast_to_build_warning<has_same_dim<std::tuple_element_t<0, Num2>, typename simplified::newDen>::value>::call();
         scalar2 = Convert<std::tuple_element_t<0, NumTuple2>, std::tuple_element_t<0, Num2>>(scalar2);
      }

      PrecisionType newScalar = scalar1 / scalar2;

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
      using absolute_quantity_tuple1 = make_tuple_of_single_absolute_quantity<NumTuple1>;
      using Num1 = std::conditional_t<absolute_quantity_tuple1::value, typename absolute_quantity_tuple1::type, typename BaseDimension<NumTuple1, DenTuple1>::NumTuple>;

      using absolute_quantity_tuple2 = make_tuple_of_single_absolute_quantity<NumTuple2>;
      using Num2 = std::conditional_t<absolute_quantity_tuple2::value, typename absolute_quantity_tuple2::type, typename BaseDimension<NumTuple2, DenTuple2>::NumTuple>;

      using Den1 = typename BaseDimension<NumTuple1, DenTuple1>::DenTuple;
      using Den2 = typename BaseDimension<NumTuple2, DenTuple2>::DenTuple;

      using simplified = UnitSimplifier<Num1, Num2, Den1, Den2>;

      PrecisionType scalar1 = obj1.scalar;
      PrecisionType scalar2 = obj2.scalar;

      // Use knowledge of the simplified type to determine whether units where cancelled.
      if constexpr (absolute_quantity_tuple1::value)
      {
         // This emits a compiler warning (deprecation warning) if an implicit cast occurs and persists in the resulting dimension.
         // Generally, users should make these casts explicitly if the goal is to maintain the unit.
         // However, if the implicit cast occurs only to "cancel" an existing unit, this is typical and does NOT emit a warning.
         implicit_cast_to_build_warning<has_same_dim<std::tuple_element_t<0, Num1>, typename simplified::newNum>::value>::call();
         scalar1 = Convert<std::tuple_element_t<0, NumTuple1>, std::tuple_element_t<0, Num1>>(scalar1);
      }

      if constexpr (absolute_quantity_tuple2::value)
      {
         // This emits a compiler warning (deprecation warning) if an implicit cast occurs and persists in the resulting dimension.
         // Generally, users should make these casts explicitly if the goal is to maintain the unit.
         // However, if the implicit cast occurs only to "cancel" an existing unit, this is typical and does NOT emit a warning.
         implicit_cast_to_build_warning<has_same_dim<std::tuple_element_t<0, Num2>, typename simplified::newNum>::value>::call();
         scalar2 = Convert<std::tuple_element_t<0, NumTuple2>, std::tuple_element_t<0, Num2>>(scalar2);
      }

      PrecisionType newScalar = scalar1 * scalar2;

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
   requires MatchingDimensionsNew<BaseDimension<NumTuple1, DenTuple1>, BaseDimension<NumTuple2, DenTuple2>>
   auto operator+(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      using NumType = add_all<NumTuple1, NumTuple2>;
      using DenType = add_all<DenTuple1, DenTuple2>;

      return BaseDimension<typename NumType::type, typename DenType::type>{
         obj1.template GetVal<typename NumType::lhsTypeOrder, typename DenType::lhsTypeOrder>() + 
         obj2.template GetVal<typename NumType::rhsTypeOrder, typename DenType::rhsTypeOrder>()
      };

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
   requires MatchingDimensionsNew<BaseDimension<NumTuple1, DenTuple1>, BaseDimension<NumTuple2, DenTuple2>>
   auto operator-(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      using NumType = subtract_all<NumTuple1, NumTuple2>;
      using DenType = subtract_all<DenTuple1, DenTuple2>;

      return BaseDimension<typename NumType::type, typename DenType::type>{
         obj1.template GetVal<typename NumType::lhsTypeOrder, typename DenType::lhsTypeOrder>() - 
         obj2.template GetVal<typename NumType::rhsTypeOrder, typename DenType::rhsTypeOrder>()
      };
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

   /// @brief Calculate hypotenuse from two sides of a right triangle
   /// @tparam T Side type, must be a BaseDimension
   /// @param obj1 Side of right triangle
   /// @param obj2 Side of right triangle
   /// @return Hypotenuse of right triangle
   template<typename T>
   T hypot(const T& obj1, const T& obj2)
   {
      return T(std::hypot(obj1.template GetVal<typename T::NumTuple, typename T::DenTuple>(), obj2.template GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

   /// @brief Calculate absolute value of a dimension
   /// @tparam T Dimension type
   /// @param obj Dimension object
   /// @return Absolute value of the dimension
   /// @todo consider how this should work for units with offset != 0
   template<typename T>
   T abs(const T& obj)
   {
      return T(std::abs(obj.template GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

   /// @brief Round dimension down to nearest whole number
   /// @tparam T Dimension type
   /// @param obj Dimension object
   /// @return Dimension object rounded down to nearest whole number
   template<typename T>
   T floor(const T& obj)
   {
      return T(std::floor(obj.template GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

   /// @brief Round dimension up to nearest whole number
   /// @tparam T Dimension type
   /// @param obj Dimension object
   /// @return Dimension object rounded up to nearest whole number
   template<typename T>
   T ceil(const T& obj)
   {
      return T(std::ceil(obj.template GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

   /// @brief Round dimension to nearest whole number
   /// @tparam T Dimension type
   /// @param obj Dimension object
   /// @return Dimension object rounded to nearest whole number
   template<typename T>
   T round(const T& obj)
   {
      return T(std::round(obj.template GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

   /// @brief Decompose dimension into integer and floating point type
   /// @tparam T Dimension type
   /// @param obj Dimension object to decompose
   /// @param[out] intPart A pointer to a Dimension object which will store the integer part
   /// @return The fractional component of the dimension
   template<typename T>
   T modf(const T& obj, T* intPart = nullptr)
   {
      double intPartDouble;
      double fracPart = std::modf(obj.template GetVal<typename T::NumTuple, typename T::DenTuple>(), &intPartDouble);

      if (intPart) {
         *intPart = T(intPartDouble);
      }

      return T(fracPart);
   }

   /// @brief Floating point remainder division of dividend / divisor
   /// @tparam T Dimension type
   /// @param dividend Dimension object to be divided
   /// @param divisor Dimension object to divide by
   /// @return Dimension object of floating point remainder division (modulus)
   template<typename T>
   T fmod(const T& dividend, const T& divisor)
   {
      if (divisor.template GetVal<typename T::NumTuple, typename T::DenTuple>() == 0) {
         throw std::invalid_argument("Divisor cannot be zero.");
      }

      return T(std::fmod(dividend.template GetVal<typename T::NumTuple, typename T::DenTuple>(), divisor.template GetVal<typename T::NumTuple, typename T::DenTuple>()));
   }

}

#endif // STATIC_DIMENSION_BASE_H
