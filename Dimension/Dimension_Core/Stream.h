#ifndef DIMENSION_STREAM_H
#define DIMENSION_STREAM_H

#include "TupleHandling.h"

#include <iostream>
#include <string>
#include <sstream>

namespace Dimension
{

   // Forward declare dimension and unit
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, StringLiteral DimName, int UnitID>
   struct BaseUnit;

   template<are_unit_exponents... Units>
   class BaseDimension;

   /// @brief Type of string to print
   enum class UnitNameTypes{Name, Abbr, DimName};

   /// @brief Update stream using a string associated with each type in a tuple
   /// @tparam StreamType Type of stream
   /// @tparam TupleType Tuple to stream
   /// @tparam Member Type of string to stream
   /// @tparam ...Indices Indecies in the tuple
   /// @param [out] stream Stream to update
   /// @param [in] delim Delimiter to use between strings
   template <typename StreamType, typename TupleType, UnitNameTypes Member, std::size_t... Indices>
   void streamMember(StreamType& stream, std::index_sequence<Indices...>, const std::string& delim) {
      ((stream << (Indices == 0 ? "" : delim), 
         [&]() {
            if constexpr (Member == UnitNameTypes::Name) {
               stream << type_from_quantity_or_delta<std::tuple_element_t<Indices, TupleType>>::type::name;
            } else if constexpr (Member == UnitNameTypes::Abbr) {
               stream << type_from_quantity_or_delta<std::tuple_element_t<Indices, TupleType>>::type::abbr;
            } else if constexpr (Member == UnitNameTypes::DimName) {
               stream << type_from_quantity_or_delta<std::tuple_element_t<Indices, TupleType>>::type::dimName;
            }
         }()), ...);
   }

   /// @brief Write dimension object to stream
   /// @tparam NumTuple Numerator unit tuple
   /// @tparam DenTuple Denominator unit tuple
   /// @param os stream to write to
   /// @param obj object to write
   /// @return reference to stream written
   template<typename NumTupleT, typename DenTupleT>
   std::ostream& to_stream(std::ostream& os, const BaseDimension<NumTupleT, DenTupleT>& obj)
   {
      using NumTuple = BaseDimension<NumTupleT, DenTupleT>::NumTuple;
      using DenTuple = BaseDimension<NumTupleT, DenTupleT>::DenTuple;

      os << obj.template GetVal<NumTuple, DenTuple>() << " [";

      if constexpr(std::tuple_size_v<NumTuple> == 1)
      {
         os << type_from_quantity_or_delta<std::tuple_element_t<0, NumTuple>>::type::abbr;
      }
      else if constexpr(std::tuple_size_v<NumTuple> > 1)
      {
         os << "(";
         streamMember<std::ostream, NumTuple, UnitNameTypes::Abbr>(os, std::make_index_sequence<std::tuple_size_v<NumTuple>>{}, "*");
         os << ")";
      }

      if constexpr(std::tuple_size_v<DenTuple> == 1)
      {
         os << " / " << type_from_quantity_or_delta<std::tuple_element_t<0, DenTuple>>::type::abbr;
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

   /// @brief Write dimension object to std::string
   /// @tparam NumTuple Numerator unit tuple
   /// @tparam DenTuple Denominator unit tuple
   /// @param obj Dimension object to write
   /// @return string representation of object
   template<typename NumTuple, typename DenTuple>
   std::string to_string(const BaseDimension<NumTuple, DenTuple>& obj)
   {
      std::ostringstream os;
      to_stream(os, obj);
      return os.str();
   }

   /// @brief Stream operator from BaseDimension to ostream
   /// @tparam NumTuple Numerator tuple
   /// @tparam DenTuple Denominator tuple
   /// @param os stream object
   /// @param obj dimension object
   /// @return reference to stream object
   template<typename NumTuple, typename DenTuple>
   std::ostream& operator<<(std::ostream& os, const BaseDimension<NumTuple, DenTuple>& obj)
   {
      return to_stream(os, obj);
   }

} // end Dimension

#endif // DIMENSION_STREAM_H
