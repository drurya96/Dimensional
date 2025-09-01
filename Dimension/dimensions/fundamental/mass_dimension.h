#ifndef STATIC_DIMENSION_MASS_H
#define STATIC_DIMENSION_MASS_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/mass_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{
  struct grams{};

  template<> struct unit_traits<grams>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Grams";
     static constexpr std::string_view abbr   = "g";
     static constexpr std::string_view dimName= "mass";
     static constexpr int id = 0;
  };
  struct pound_mass{};

  template<> struct unit_traits<pound_mass>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Pound mass";
     static constexpr std::string_view abbr   = "lb";
     static constexpr std::string_view dimName= "mass";
     static constexpr int id = 0;
  };
  struct ounces{};

  template<> struct unit_traits<ounces>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Ounces";
     static constexpr std::string_view abbr   = "oz";
     static constexpr std::string_view dimName= "mass";
     static constexpr int id = 0;
  };
  struct slugs{};

  template<> struct unit_traits<slugs>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Slugs";
     static constexpr std::string_view abbr   = "slug";
     static constexpr std::string_view dimName= "mass";
     static constexpr int id = 0;
  };
  struct grains{};

  template<> struct unit_traits<grains>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Grains";
     static constexpr std::string_view abbr   = "gr";
     static constexpr std::string_view dimName= "mass";
     static constexpr int id = 0;
  };
  struct stone{};

  template<> struct unit_traits<stone>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Stone";
     static constexpr std::string_view abbr   = "st";
     static constexpr std::string_view dimName= "mass";
     static constexpr int id = 0;
  };
  struct short_ton{};

  template<> struct unit_traits<short_ton>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Short Ton";
     static constexpr std::string_view abbr   = "ton";
     static constexpr std::string_view dimName= "mass";
     static constexpr int id = 0;
  };
  struct long_ton{};

  template<> struct unit_traits<long_ton>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Long Ton";
     static constexpr std::string_view abbr   = "LT";
     static constexpr std::string_view dimName= "mass";
     static constexpr int id = 0;
  };
  struct tonne{};

  template<> struct unit_traits<tonne>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Tonne";
     static constexpr std::string_view abbr   = "t";
     static constexpr std::string_view dimName= "mass";
     static constexpr int id = 0;
  };

  template<> struct Conversion<grams, pound_mass>
  {
     using scale = factor_t<
        std::ratio<100000, 45359237>>;
  };
  template<> struct Conversion<grams, ounces>
  {
     using scale = factor_t<
        std::ratio<1600000, 45359237>>;
  };
  template<> struct Conversion<grams, slugs>
  {
     using scale = factor_t<
        std::ratio<609600000, 8896443230521>>;
  };
  template<> struct Conversion<grams, grains>
  {
     using scale = factor_t<
        std::ratio<100000000, 6479891>>;
  };
  template<> struct Conversion<grams, stone>
  {
     using scale = factor_t<
        std::ratio<50000, 317514659>>;
  };
  template<> struct Conversion<grams, short_ton>
  {
     using scale = factor_t<
        std::ratio<50, 45359237>>;
  };
  template<> struct Conversion<grams, long_ton>
  {
     using scale = factor_t<
        std::ratio<625, 635029318>>;
  };
  template<> struct Conversion<grams, tonne>
  {
     using scale = factor_t<
        std::ratio<1, 1>, pow10<-6>>;
  };

    struct pico_grams{};

    template<> struct unit_traits<pico_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "picoGrams";
       static constexpr std::string_view abbr   = "pg";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> pico_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, pico_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<12>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<pico_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-12>>;
    };
    struct nano_grams{};

    template<> struct unit_traits<nano_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "nanoGrams";
       static constexpr std::string_view abbr   = "ng";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> nano_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, nano_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<9>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<nano_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-9>>;
    };
    struct micro_grams{};

    template<> struct unit_traits<micro_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "microGrams";
       static constexpr std::string_view abbr   = "ug";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> micro_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, micro_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<6>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<micro_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-6>>;
    };
    struct milli_grams{};

    template<> struct unit_traits<milli_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "milliGrams";
       static constexpr std::string_view abbr   = "mg";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> milli_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, milli_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<3>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<milli_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-3>>;
    };
    struct centi_grams{};

    template<> struct unit_traits<centi_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "centiGrams";
       static constexpr std::string_view abbr   = "cg";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> centi_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, centi_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<2>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<centi_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-2>>;
    };
    struct deci_grams{};

    template<> struct unit_traits<deci_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "deciGrams";
       static constexpr std::string_view abbr   = "dg";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> deci_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, deci_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<1>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<deci_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-1>>;
    };
    struct deca_grams{};

    template<> struct unit_traits<deca_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "decaGrams";
       static constexpr std::string_view abbr   = "dag";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> deca_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, deca_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-1>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<deca_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<1>>;
    };
    struct hecto_grams{};

    template<> struct unit_traits<hecto_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "hectoGrams";
       static constexpr std::string_view abbr   = "hg";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> hecto_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, hecto_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-2>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<hecto_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<2>>;
    };
    struct kilo_grams{};

    template<> struct unit_traits<kilo_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "kiloGrams";
       static constexpr std::string_view abbr   = "kg";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> kilo_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, kilo_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-3>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<kilo_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<3>>;
    };
    struct mega_grams{};

    template<> struct unit_traits<mega_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "megaGrams";
       static constexpr std::string_view abbr   = "Mg";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> mega_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, mega_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-6>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<mega_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<6>>;
    };
    struct giga_grams{};

    template<> struct unit_traits<giga_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "gigaGrams";
       static constexpr std::string_view abbr   = "Gg";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> giga_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, giga_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-9>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<giga_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<9>>;
    };
    struct tera_grams{};

    template<> struct unit_traits<tera_grams>
    {
       using dimension = massType;
       static constexpr std::string_view name   = "teraGrams";
       static constexpr std::string_view abbr   = "Tg";
       static constexpr std::string_view dimName= "mass";
       static constexpr int id = 0;
    };

    // grams <-> tera_grams
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<grams, tera_grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<-12>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<tera_grams, grams>
    {
        using scale = factor_t<std::ratio<1>, pow10<12>>;
    };
}


#endif // STATIC_DIMENSION_MASS_H