#ifndef DIMENSION_QUANTITY_H
#define DIMENSION_QUANTITY_H

#include "UnitSimplifier.h"
#include "Conversion.h"

namespace Dimension
{

    // Forward declaration
   template<typename NumTupleT, typename DenTupleT>
   requires IsUnitTuplePair<NumTupleT, DenTupleT>
   class BaseDimension;

   /// @brief Quantity wrapper for dimensions
   /// @details Wrap any dimension type (as a template parameter) in Quantity
   ///   to treat it as a quantity, or point.
   ///   Quantities behave differently from BaseDimension types (deltas) in that
   ///   subtracting quantities yields a delta, and many operations cannot be performed
   ///   on quantities as the operations are not physically meaningful.
   ///   Further, "offsets", such as those seen in Temperature units, only apply to Quantities.
   /// @tparam DimensionT Dimension to wrap as a Qauntity
   template<typename DimensionT>
   class Quantity
   {
   public:
      Quantity(PrecisionType val) : value(DimensionT{val}){}

      template<typename T>
      Quantity(Quantity<T> obj) : value(DimensionT{obj.template GetVal<typename DimensionT::NumTuple, typename DimensionT::DenTuple>()}) {}

      template<typename NumTuple, typename DenTuple>
      PrecisionType GetVal() const
      {
         PrecisionType result = value.template GetVal<typename DimensionT::NumTuple, typename DimensionT::DenTuple>();

         ConvertDimension<0, false, NumTuple, typename DimensionT::NumTuple, false>(result);
         ConvertDimension<0, true, DenTuple, typename DimensionT::DenTuple, false>(result);

         return result;
      }

      template<typename NumTuple, typename DenTuple>
      void SetVal(PrecisionType val)
      {
         ConvertDimension<0, false, typename DimensionT::NumTuple, NumTuple, false>(val);
         ConvertDimension<0, true, typename DimensionT::DenTuple, DenTuple, false>(val);

         value.template SetVal<typename DimensionT::NumTuple, typename DimensionT::DenTuple>(val);
      }


   private:
      DimensionT value;
   };

   /// @brief simple always-false trait used in static_asserts to disallow functions
   template<typename T>
   struct disallowed_operator : std::false_type {};


   // Quantity operators
   
   /// @brief Addition operator for two Dimensions
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object of type matching the inputs.
   ///    The value is the values of obj1 and obj2 added, after converting
   ///    obj2 to the same units as obj1
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator+(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      static_assert(disallowed_operator<NumTuple1>::value, "Cannot add quantities!");
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
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator+(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      return Quantity<BaseDimension<NumTuple1, DenTuple1>>{ obj1.template GetVal<NumTuple1, DenTuple1>() + obj2.template GetVal<NumTuple1, DenTuple1>() };
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
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator+(const BaseDimension<NumTuple1, DenTuple1>& obj1, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& obj2)
   {
      return Quantity<BaseDimension<NumTuple1, DenTuple1>>{ obj1.template GetVal<NumTuple1, DenTuple1>() + obj2.template GetVal<NumTuple1, DenTuple1>() };
   }

   template<typename Dim1, typename Dim2>
   requires MatchingDimensions<typename Dim1::NumTuple, typename Dim2::NumTuple> && MatchingDimensions<typename Dim1::DenTuple, typename Dim2::DenTuple>
   BaseDimension<typename Dim1::NumTuple, typename Dim1::DenTuple> operator-(const Quantity<Dim1>& obj1, const Quantity<Dim2>& obj2)
   {
      return BaseDimension<typename Dim1::NumTuple, typename Dim1::DenTuple>{ obj1.template GetVal<typename Dim1::NumTuple, typename Dim1::DenTuple>() - obj2.template GetVal<typename Dim1::NumTuple, typename Dim1::DenTuple>() };
   }

   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator-(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      return Quantity<BaseDimension<NumTuple1, DenTuple1>>{ obj1.template GetVal<NumTuple1, DenTuple1>() - obj2.template GetVal<NumTuple1, DenTuple1>() };
   }

   /// @brief Subtracting a Qauntity from a delta is intentionally not implemented
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator-(const BaseDimension<NumTuple1, DenTuple1>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      static_assert(disallowed_operator<NumTuple1>::value, "Cannot subtract a quantity from a delta!");
   }

   /// @brief Multiplying quantities is intentionally not implemented
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator*(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      static_assert(disallowed_operator<NumTuple1>::value, "Cannot multiply quantities!");
   }

   /// @brief Multiplying a quantity and delta is intentionally not implemented
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   void operator*(const BaseDimension<NumTuple1, DenTuple1>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      static_assert(disallowed_operator<NumTuple1>::value, "Cannot multiply quantity by delta!");
   }

   /// @brief Multiplying a delta and a quantity is intentionally not implemented
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   void operator*(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const BaseDimension<NumTuple2, DenTuple2>& /*obj2*/)
   {
      static_assert(disallowed_operator<NumTuple1>::value, "Cannot multiply quantity by delta!");
   }

   /// @brief Dividing quantities is intentionally not implemented
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator/(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
         static_assert(disallowed_operator<NumTuple1>::value, "Cannot divide quantities!");
   }

   /// @brief Dividing two quantities is intentionally not implemented by default
   /// @details Defining the preprocessor value DIMENSIONAL_ENABLE_QUANTITY_DIVISION will
   ///   allow this division, but only if the units are already identical. This is generally not
   ///   advised but some real-world cases require this usage.
   template<typename NumTuple, typename DenTuple>
   PrecisionType operator/(const Quantity<BaseDimension<NumTuple, DenTuple>>& obj1 [[maybe_unused]], 
                           const Quantity<BaseDimension<NumTuple, DenTuple>>& obj2 [[maybe_unused]])
   {
      #ifdef DIMENSIONAL_ENABLE_QUANTITY_DIVISION
         return obj1.template GetVal<NumTuple, DenTuple>() / obj2.template GetVal<NumTuple, DenTuple>();
      #else
         static_assert(disallowed_operator<NumTuple>::value, "Cannot divide quantities!");
         return 0.0;
      #endif
   }

   /// @brief Dividing a delta by a quantity is intentionally not implemented
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   void operator/(const BaseDimension<NumTuple1, DenTuple1>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      static_assert(disallowed_operator<NumTuple1>::value, "Cannot divide quantity by delta!");
   }

   /// @brief Dividing a quantity by a delta is intentionally not implemented
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   void operator/(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const BaseDimension<NumTuple2, DenTuple2>& /*obj2*/)
   {
      static_assert(disallowed_operator<NumTuple1>::value, "Cannot divide quantity by delta!");
   }

   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   bool operator>(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      return true;
   }

   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   bool operator<(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      return false;
   }

}

#endif // DIMENSION_QUANTITY_H