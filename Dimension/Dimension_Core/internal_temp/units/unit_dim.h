#ifndef DIMENSIONAL_UNIT_DIM_H
#define DIMENSIONAL_UNIT_DIM_H

namespace dimension
{

   // TODO: These are placeholders so that when I have different logic to determine the dimension
   //   and primary units, I can just slot those in here without modifying the rest of the library.

   template<typename T>
   struct unit_dim
   {
      using type = T::Dim;
   };

   template<typename T>
   using unit_dim_t = typename unit_dim<T>::type;

   template<typename T>
   struct unit_primary
   {
      using type = T::Primary;
   };

   template<typename T>
   using unit_primary_t = typename unit_primary<T>::type;

} // end Dimension

#endif // DIMENSIONAL_UNIT_DIM_H