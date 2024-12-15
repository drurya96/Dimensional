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





   // Quantity operators

   // Addition operators
   // Between Quantities
   //   Only valid for extensive quantities, delete for now
   
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
      static_assert(false, "Cannot add quantities!");
   }


   // Delta and quantity
   //   Yields quantity

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






   // Subtraction operators
   // Between Quantities
   //   Yields delta
   /*
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   BaseDimension<NumTuple1, DenTuple1> operator-(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& obj1, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& obj2)
   {
      return BaseDimension<NumTuple1, DenTuple1>{ obj1.template GetVal<NumTuple1, DenTuple1>() - obj2.template GetVal<NumTuple1, DenTuple1>() };
   }
   */


   // Delta from quantity
   //   Yields quantity
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator-(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      return Quantity<BaseDimension<NumTuple1, DenTuple1>>{ obj1.template GetVal<NumTuple1, DenTuple1>() - obj2.template GetVal<NumTuple1, DenTuple1>() };
   }

   // Quantity from Delta
   //   Invalid
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator-(const BaseDimension<NumTuple1, DenTuple1>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      static_assert(false, "Cannot subtract a quantity from a delta!");
   }





   template<typename Dim1, typename Dim2>
   //requires true
   BaseDimension<typename Dim1::NumTuple, typename Dim1::DenTuple> operator-(const Quantity<Dim1>& obj1, const Quantity<Dim2>& obj2)
   {
      return BaseDimension<typename Dim1::NumTuple, typename Dim1::DenTuple>{ obj1.template GetVal<typename Dim1::NumTuple, typename Dim1::DenTuple>() - obj2.template GetVal<typename Dim1::NumTuple, typename Dim1::DenTuple>() };
   }






   // Multiplication operators
   // Between Quantities
   //   Only valid for extensive quantities, delete for now
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator*(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      static_assert(false, "Cannot multiply quantities!");
   }


   // Delta and quantity
   //   invalid
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   void operator*(const BaseDimension<NumTuple1, DenTuple1>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      static_assert(false, "Cannot multiply quantity by delta!");
   }

   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   void operator*(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const BaseDimension<NumTuple2, DenTuple2>& /*obj2*/)
   {
      static_assert(false, "Cannot multiply quantity by delta!");
   }



   // Division operators
   // Between Quantities
   //   Only valid for extensive quantities, delete for now

   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   requires MatchingDimensions<NumTuple1, NumTuple2> && MatchingDimensions<DenTuple1, DenTuple2>
   Quantity<BaseDimension<NumTuple1, DenTuple1>> operator/(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      static_assert(false, "Cannot divide quantities!");
   }


   // Delta and quantity
   //   invalid
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   void operator/(const BaseDimension<NumTuple1, DenTuple1>& /*obj1*/, const Quantity<BaseDimension<NumTuple2, DenTuple2>>& /*obj2*/)
   {
      static_assert(false, "Cannot divide quantity by delta!");
   }

   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   void operator/(const Quantity<BaseDimension<NumTuple1, DenTuple1>>& /*obj1*/, const BaseDimension<NumTuple2, DenTuple2>& /*obj2*/)
   {
      static_assert(false, "Cannot divide quantity by delta!");
   }


   // Comparison operators
   //    Only valid between like-quantities


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