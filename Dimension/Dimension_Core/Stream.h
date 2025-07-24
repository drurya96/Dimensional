#ifndef DIMENSION_STREAM_H
#define DIMENSION_STREAM_H

#include <concepts>
#include <iostream>
#include <string>
#include <sstream>

#include "TupleHandling.h"
#include "base_dimension_signature.h"

namespace dimension
{
   // pretty-print one std::ratio
   template<typename Stream, typename R>
   constexpr void stream_ratio(Stream& os)
   {
      if constexpr (R::den == 1)
         os << R::num;                         //  n
      else
         os << '(' << R::num << '/' << R::den << ')';   // (n/d)
   }

   // write one unit_exponent<U,Num,Den>
   template<typename Stream, typename UE>
   constexpr void stream_one_unit(Stream& os)
   {
      using Unit     = typename UE::unit;
      using Exponent = typename UE::exponent;   // std::ratio<Num,Den>

      os << Unit::abbr;                         // "m", "s", …

      // omit "^1"
      if constexpr (!(Exponent::num == 1 && Exponent::den == 1))
      {
         os << '^';
         stream_ratio<Stream, Exponent>(os);
      }
   }

   //--------------------------------------------------------------------------
   //  main entry – variadic pack
   //--------------------------------------------------------------------------

   template<typename Stream, typename... Units>
   constexpr void stream_units(Stream& os)
   {
      bool first = true;

      ((
         // This condition is NOT always known, its updated inside the fold expression.
         // cppcheck can't detect that.
         // cppcheck-suppress knownConditionTrueFalse
         first ? static_cast<void>(first = false)          // first element
               : static_cast<void>(os << " * "),           // later elements
         stream_one_unit<Stream, Units>(os)                // print the unit
      ), ...);                                              // fold over pack
   }


   template<typename Stream, typename Tuple>
   struct _stream_units_tuple_impl;                          // primary

   template<typename Stream, typename... Units>
   struct _stream_units_tuple_impl<Stream, std::tuple<Units...>>
   {
      static constexpr void apply(Stream& os)
      {
         stream_units<Stream, Units...>(os);        // reuse the pack fn
      }
   };

   // convenience helper -----------------------------------------------
   template<typename Stream, typename Tuple>
   constexpr void stream_units_tuple(Stream& os)
   {
      _stream_units_tuple_impl<Stream, Tuple>::apply(os);
   }



   /// @brief Write dimension object to stream
   /// @tparam NumTuple Numerator unit tuple
   /// @tparam DenTuple Denominator unit tuple
   /// @param os stream to write to
   /// @param obj object to write
   /// @return reference to stream written
   template<is_base_dimension Dim>
   std::ostream& to_stream(std::ostream& os, const Dim& obj)
   {
      //using NumTuple = base_dimension<NumTupleT, DenTupleT>::NumTuple;
      //using DenTuple = base_dimension<NumTupleT, DenTupleT>::DenTuple;
      using units = typename Dim::units;

      os << get_dimension_tuple<units>(obj) << " [";

      stream_units_tuple<std::ostream, units>(os);

      os << "]";

      return os;
   }

   /// @brief Write dimension object to std::string
   /// @tparam NumTuple Numerator unit tuple
   /// @tparam DenTuple Denominator unit tuple
   /// @param obj Dimension object to write
   /// @return string representation of object
   template<is_base_dimension Dim>
   std::string to_string(const Dim& obj)
   {
      std::ostringstream os;
      to_stream(os, obj);
      return os.str();
   }

   /// @brief Stream operator from base_dimension to ostream
   /// @tparam NumTuple Numerator tuple
   /// @tparam DenTuple Denominator tuple
   /// @param os stream object
   /// @param obj dimension object
   /// @return reference to stream object
   template<is_base_dimension Dim>
   std::ostream& operator<<(std::ostream& os, const Dim& obj)
   {
      return to_stream(os, obj);
   }

} // end Dimension

#endif // DIMENSION_STREAM_H
