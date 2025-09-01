#ifndef STATIC_DIMENSION_PRESSURE_H
#define STATIC_DIMENSION_PRESSURE_H

#include "Dimension_Impl/DerivedDimensions/pressure_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  // Helper fundamental units used only to compose derived units
  struct atmosphere_mass{};

  template<> struct unit_traits<atmosphere_mass>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Atmospheremass";
     static constexpr std::string_view abbr   = "Atmospheremass";
     static constexpr std::string_view dimName= "pressure";
     static constexpr int id = 0;
  };

  template<> struct Conversion<atmosphere_mass, grams>
  {
     using scale = factor_t<
        std::ratio<101325, 1>, pow10<3>>;
  };

  struct bar_mass{};

  template<> struct unit_traits<bar_mass>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Barmass";
     static constexpr std::string_view abbr   = "Barmass";
     static constexpr std::string_view dimName= "pressure";
     static constexpr int id = 0;
  };

  template<> struct Conversion<bar_mass, grams>
  {
     using scale = factor_t<
        std::ratio<1, 1>, pow10<8>>;
  };

  struct torr_mass{};

  template<> struct unit_traits<torr_mass>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Torrmass";
     static constexpr std::string_view abbr   = "Torrmass";
     static constexpr std::string_view dimName= "pressure";
     static constexpr int id = 0;
  };

  template<> struct Conversion<torr_mass, grams>
  {
     using scale = factor_t<
        std::ratio<13332231202220616, 1>, pow10<-11>>;
  };

  struct in_hg_mass{};

  template<> struct unit_traits<in_hg_mass>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "inHgmass";
     static constexpr std::string_view abbr   = "inHgmass";
     static constexpr std::string_view dimName= "pressure";
     static constexpr int id = 0;
  };

  template<> struct Conversion<in_hg_mass, grams>
  {
     using scale = factor_t<
        std::ratio<3386386725364, 1>, pow10<-6>>;
  };

  struct pascals
  {
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<pascals>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };
  struct atmospheres
  {
     using units = std::tuple<unit_exponent<atmosphere_mass, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<atmospheres>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<atmosphere_mass, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };
  struct bar
  {
     using units = std::tuple<unit_exponent<bar_mass, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<bar>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<bar_mass, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };
  struct torr
  {
     using units = std::tuple<unit_exponent<torr_mass, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<torr>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<torr_mass, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };
  struct in_hg
  {
     using units = std::tuple<unit_exponent<in_hg_mass, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<in_hg>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<in_hg_mass, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };
  struct mm_hg
  {
     using units = std::tuple<unit_exponent<torr_mass, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<mm_hg>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<torr_mass, 1, 1>, unit_exponent<meters, -1, 1>, unit_exponent<seconds, -2, 1>>;
  };
}


#endif // STATIC_DIMENSION_PRESSURE_H