#ifndef STATIC_DIMENSION_CHARGE_H
#define STATIC_DIMENSION_CHARGE_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/charge_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{
  struct coulombs{};

  template<> struct unit_traits<coulombs>
  {
     using dimension = chargeType;
     static constexpr std::string_view name   = "Coulombs";
     static constexpr std::string_view abbr   = "C";
     static constexpr std::string_view dimName= "charge";
     static constexpr int id = 0;
  };
  struct elementary_charges{};

  template<> struct unit_traits<elementary_charges>
  {
     using dimension = chargeType;
     static constexpr std::string_view name   = "Elementary charges";
     static constexpr std::string_view abbr   = "e";
     static constexpr std::string_view dimName= "charge";
     static constexpr int id = 0;
  };

template<> struct Conversion<coulombs, elementary_charges>
{
using scale = factor_t<
   std::ratio<1, 1>,
   std::tuple<ratio_exponent<std::ratio<2, 1>, 27, 1>, ratio_exponent<std::ratio<3, 1>, -2, 1>, ratio_exponent<std::ratio<5, 1>, 28, 1>, ratio_exponent<std::ratio<19, 1>, -1, 1>, ratio_exponent<std::ratio<389, 1>, -1, 1>, ratio_exponent<std::ratio<12043, 1>, -1, 1>>>;
};

struct pico_coulombs{};

template<> struct unit_traits<pico_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "picoCoulombs";
  static constexpr std::string_view abbr    = "pC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> pico_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, pico_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<12>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<pico_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-12>>
  >;
};
struct nano_coulombs{};

template<> struct unit_traits<nano_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "nanoCoulombs";
  static constexpr std::string_view abbr    = "nC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> nano_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, nano_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<9>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<nano_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-9>>
  >;
};
struct micro_coulombs{};

template<> struct unit_traits<micro_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "microCoulombs";
  static constexpr std::string_view abbr    = "uC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> micro_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, micro_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<6>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<micro_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-6>>
  >;
};
struct milli_coulombs{};

template<> struct unit_traits<milli_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "milliCoulombs";
  static constexpr std::string_view abbr    = "mC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> milli_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, milli_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<3>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<milli_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-3>>
  >;
};
struct centi_coulombs{};

template<> struct unit_traits<centi_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "centiCoulombs";
  static constexpr std::string_view abbr    = "cC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> centi_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, centi_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<2>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<centi_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-2>>
  >;
};
struct deci_coulombs{};

template<> struct unit_traits<deci_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "deciCoulombs";
  static constexpr std::string_view abbr    = "dC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> deci_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, deci_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<1>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<deci_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-1>>
  >;
};
struct deca_coulombs{};

template<> struct unit_traits<deca_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "decaCoulombs";
  static constexpr std::string_view abbr    = "daC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> deca_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, deca_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-1>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<deca_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<1>>
  >;
};
struct hecto_coulombs{};

template<> struct unit_traits<hecto_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "hectoCoulombs";
  static constexpr std::string_view abbr    = "hC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> hecto_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, hecto_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-2>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<hecto_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<2>>
  >;
};
struct kilo_coulombs{};

template<> struct unit_traits<kilo_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "kiloCoulombs";
  static constexpr std::string_view abbr    = "kC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> kilo_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, kilo_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-3>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<kilo_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<3>>
  >;
};
struct mega_coulombs{};

template<> struct unit_traits<mega_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "megaCoulombs";
  static constexpr std::string_view abbr    = "MC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> mega_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, mega_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-6>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<mega_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<6>>
  >;
};
struct giga_coulombs{};

template<> struct unit_traits<giga_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "gigaCoulombs";
  static constexpr std::string_view abbr    = "GC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> giga_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, giga_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-9>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<giga_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<9>>
  >;
};
struct tera_coulombs{};

template<> struct unit_traits<tera_coulombs>
{
  using dimension = chargeType;
  static constexpr std::string_view name    = "teraCoulombs";
  static constexpr std::string_view abbr    = "TC";
  static constexpr std::string_view dimName = "charge";
  static constexpr int id = 0;
};

// coulombs <-> tera_coulombs
// base->prefixed multiplies by 10^{-exp}
template<> struct Conversion<coulombs, tera_coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<-12>>
  >;
};

// prefixed->base multiplies by 10^{+exp}
template<> struct Conversion<tera_coulombs, coulombs>
{
  using scale = factor_t<
    std::ratio<1>,
    std::tuple<>,
    std::tuple<pow10<12>>
  >;
};


}


#endif // STATIC_DIMENSION_CHARGE_H