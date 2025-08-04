#ifndef DIMENSIONAL_NULL_UNIT_H
#define DIMENSIONAL_NULL_UNIT_H

namespace dimension::details
{
   /// @brief A type-trait with void Dim and Primary, only used to satisfy a metaprogramming condition
   struct null_unit
   {
      using Dim = void;
      using Primary = void;
      static constexpr int ID = 0;
   };
}

#endif // DIMENSIONAL_NULL_UNIT_H