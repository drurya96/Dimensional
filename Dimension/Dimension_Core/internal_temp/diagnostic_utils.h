#ifndef DIMENSIONAL_DIAGNOSTIC_UTILS_H
#define DIMENSIONAL_DIAGNOSTIC_UTILS_H

namespace dimension::details
{
   template <bool B>
   struct implicit_cast_to_build_warning
   {
      static void call() {}
   };

   template <>
   struct implicit_cast_to_build_warning<true>
   {
      [[deprecated("Attempting to create new dimension ...")]]
      static void call() {}
   };
}

#endif // DIMENSIONAL_DIAGNOSTIC_UTILS_H