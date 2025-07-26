#ifndef DIMENSION_PRECISION_TYPE_H
#define DIMENSION_PRECISION_TYPE_H

namespace dimension
{
   #if defined(USE_DOUBLE) + defined(USE_FLOAT) + defined(USE_LONG_DOUBLE) > 1
   #error "More than one precision type defined. Please define only one of USE_DOUBLE, USE_FLOAT, USE_LONG_DOUBLE, or USE_LONG_LONG_DOUBLE."
   #endif
   
   /// @brief The data storage type used throughout this library
   #ifdef USE_DOUBLE
   using PrecisionType = double;
   #elif defined(USE_FLOAT)
   using PrecisionType = float;
   #elif defined(USE_LONG_DOUBLE)
   using PrecisionType = long double;
   #else
   #error "PrecisionType is not defined. Please define one of USE_DOUBLE, USE_FLOAT, USE_LONG_DOUBLE."
   #endif
} // end Dimension

#endif // DIMENSION_PRECISION_TYPE_H