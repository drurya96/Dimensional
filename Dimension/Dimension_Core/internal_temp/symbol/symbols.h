#ifndef DIMENSIONAL_SYMBOLS_H
#define DIMENSIONAL_SYMBOLS_H

#include <numbers>

#include "symbol_utils.h"

namespace dimension::symbols
{

   // Constants from std::numbers

   // e
   struct e : public symbol{
      static constexpr double value = std::numbers::e;
   };

   struct log2e : public symbol{
      static constexpr double value = std::numbers::log2e;
   };

   struct log10e : public symbol{
      static constexpr double value = std::numbers::log10e;
   };

   // pi
   struct pi : public symbol{
      static constexpr double value = std::numbers::pi;
   };

   struct inv_pi : public symbol{
      static constexpr double value = std::numbers::inv_pi;
   };

   struct inv_sqrtpi : public symbol{
      static constexpr double value = std::numbers::inv_sqrtpi;
   };

   struct sqrt_pi : public symbol{
      static constexpr double value = 1.0 / std::numbers::inv_sqrtpi;
   };

   // natural logs
   struct ln2 : public symbol{
      static constexpr double value = std::numbers::ln2;
   };

   struct ln10 : public symbol{
      static constexpr double value = std::numbers::ln10;
   };

   // Common roots
   struct sqrt2 : public symbol{
      static constexpr double value = std::numbers::sqrt2;
   };

   struct sqrt3 : public symbol{
      static constexpr double value = std::numbers::sqrt3;
   };

   struct inv_sqrt3 : public symbol{
      static constexpr double value = std::numbers::inv_sqrt3;
   };

   // Euler's Constant (egamma)
   struct egamma : public symbol{
      static constexpr double value = std::numbers::egamma;
   };

   // golden ratio (phi)
   struct phi : public symbol{
      static constexpr double value = std::numbers::phi;
   };


   // Symbols from other constants, not in std::numbers

   struct catalan : public symbol{
      static constexpr double value = 0.915965594177219015054603514932384110774;
   };

   struct apery : public symbol{
      static constexpr double value = 1.202056903159594285399738161511449990764986292;
   };

   struct feigenbaum_delta : public symbol{
      static constexpr double value = 4.669201609102990671853203820466201617258185577475768632745651;
   };

   struct feigenbaum_alpha : public symbol{
      static constexpr double value = 2.502907875095892822283902873218215786381271376727149977336192;
   };

   struct plastic_rho : public symbol{
      static constexpr double value = 1.324717957244746;
   };

   struct khinchin : public symbol{
      static constexpr double value = 2.685452001065306445309714835481795693820382293994462953051152;
   }; 

}

#endif //DIMENSIONAL_SYMBOLS_H
