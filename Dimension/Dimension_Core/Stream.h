#ifndef DIMENSION_STREAM_H
#define DIMENSION_STREAM_H

#include "TupleHandling.h"

#include <iostream>
#include <string>
#include <sstream>

namespace Dimension
{

   // Forward declare dimension and unit
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, StringLiteral DimName>
   struct BaseUnit;

   template<typename NumTupleT, typename DenTupleT>
   requires IsUnitTuplePair<NumTupleT, DenTupleT>
   class BaseDimension;

   enum class UnitNameTypes{Name, Abbr, DimName};

   // Modified streamMember function with if-else structure
   template <typename StreamType, typename TupleType, UnitNameTypes Member, std::size_t... Indices>
   void streamMember(StreamType& stream, std::index_sequence<Indices...>, const std::string& delim) {
      ((stream << (Indices == 0 ? "" : delim), 
         [&]() {
            if constexpr (Member == UnitNameTypes::Name) {
               stream << std::tuple_element_t<Indices, TupleType>::name;
            } else if constexpr (Member == UnitNameTypes::Abbr) {
               stream << std::tuple_element_t<Indices, TupleType>::abbr;
            } else if constexpr (Member == UnitNameTypes::DimName) {
               stream << std::tuple_element_t<Indices, TupleType>::dimName;
            }
         }()), ...);
   }

   template<typename NumTuple, typename DenTuple>
   std::ostream& to_stream(std::ostream& os, const BaseDimension<NumTuple, DenTuple>& obj)
   {

      os << obj.template GetVal<NumTuple, DenTuple>() << " [";

      if constexpr(std::tuple_size_v<NumTuple> == 1)
      {
         os << std::tuple_element_t<0, NumTuple>::abbr;
      }
      else if constexpr(std::tuple_size_v<NumTuple> > 1)
      {
         os << "(";
         streamMember<std::ostream, NumTuple, UnitNameTypes::Abbr>(os, std::make_index_sequence<std::tuple_size_v<NumTuple>>{}, "*");
         os << ")";
      }

      if constexpr(std::tuple_size_v<DenTuple> == 1)
      {
         os << " / " << std::tuple_element_t<0, DenTuple>::abbr;
      }
      else if constexpr(std::tuple_size_v<DenTuple> > 1)
      {
         os << " / (";
         streamMember<std::ostream, DenTuple, UnitNameTypes::Abbr>(os, std::make_index_sequence<std::tuple_size_v<DenTuple>>{}, "*");
         os << ")";
      }

      os << "]";

      return os;
   }

   template<typename NumTuple, typename DenTuple>
   std::string to_string(const BaseDimension<NumTuple, DenTuple>& obj)
   {
      std::ostringstream os;
      to_stream(os, obj);
      return os.str();
   }

   template<typename NumTuple, typename DenTuple>
   std::ostream& operator<<(std::ostream& os, const BaseDimension<NumTuple, DenTuple>& obj)
   {
      return to_stream(os, obj);
   }

} // end Dimension

#endif // DIMENSION_STREAM_H