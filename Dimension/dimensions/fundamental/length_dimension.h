#ifndef STATIC_DIMENSION_LENGTH_H
#define STATIC_DIMENSION_LENGTH_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/length_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{
  struct meters{};

  template<> struct unit_traits<meters>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "meters";
     static constexpr std::string_view abbr   = "m";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };
  struct feet{};

  template<> struct unit_traits<feet>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Feet";
     static constexpr std::string_view abbr   = "ft";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };
  struct inches{};

  template<> struct unit_traits<inches>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Inches";
     static constexpr std::string_view abbr   = "in";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };
  struct astronomical_units{};

  template<> struct unit_traits<astronomical_units>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Astronomical Units";
     static constexpr std::string_view abbr   = "au";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };
  struct data_miles{};

  template<> struct unit_traits<data_miles>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Data Miles";
     static constexpr std::string_view abbr   = "DataMiles";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };
  struct nautical_miles{};

  template<> struct unit_traits<nautical_miles>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Nautical Miles";
     static constexpr std::string_view abbr   = "nmi";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };
  struct miles{};

  template<> struct unit_traits<miles>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Miles";
     static constexpr std::string_view abbr   = "mi";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };
  struct fathoms{};

  template<> struct unit_traits<fathoms>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Fathoms";
     static constexpr std::string_view abbr   = "ftm";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };
  struct furlong{};

  template<> struct unit_traits<furlong>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Furlong";
     static constexpr std::string_view abbr   = "fur";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };
  struct yards{};

  template<> struct unit_traits<yards>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Yards";
     static constexpr std::string_view abbr   = "yd";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };
  struct us_survey_feet{};

  template<> struct unit_traits<us_survey_feet>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "US Survey Feet";
     static constexpr std::string_view abbr   = "ftUS";
     static constexpr std::string_view dimName= "length";
     static constexpr int id = 0;
  };

  template<> struct Conversion<meters, feet>
  {
     using scale = factor_t<
        std::ratio<1250, 381>>;
  };
  template<> struct Conversion<meters, inches>
  {
     using scale = factor_t<
        std::ratio<15000, 381>>;
  };
  template<> struct Conversion<meters, astronomical_units>
  {
     using scale = factor_t<
        std::ratio<1, 149597870700>>;
  };
  template<> struct Conversion<meters, data_miles>
  {
     using scale = factor_t<
        std::ratio<5, 9144>>;
  };
  template<> struct Conversion<meters, nautical_miles>
  {
     using scale = factor_t<
        std::ratio<1, 1852>>;
  };
  template<> struct Conversion<meters, miles>
  {
     using scale = factor_t<
        std::ratio<125, 201168>>;
  };
  template<> struct Conversion<meters, fathoms>
  {
     using scale = factor_t<
        std::ratio<3937, 7200>>;
  };
  template<> struct Conversion<meters, furlong>
  {
     using scale = factor_t<
        std::ratio<3937, 792000>>;
  };
  template<> struct Conversion<meters, yards>
  {
     using scale = factor_t<
        std::ratio<1250, 1143>>;
  };
  template<> struct Conversion<meters, us_survey_feet>
  {
     using scale = factor_t<
        std::ratio<3937, 1200>>;
  };

    struct pico_meters{};

    template<> struct unit_traits<pico_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "picometers";
       static constexpr std::string_view abbr   = "pm";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> pico_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, pico_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<12>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<pico_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-12>>;
    };
    struct nano_meters{};

    template<> struct unit_traits<nano_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "nanometers";
       static constexpr std::string_view abbr   = "nm";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> nano_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, nano_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<9>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<nano_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-9>>;
    };
    struct micro_meters{};

    template<> struct unit_traits<micro_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "micrometers";
       static constexpr std::string_view abbr   = "um";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> micro_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, micro_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<6>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<micro_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-6>>;
    };
    struct milli_meters{};

    template<> struct unit_traits<milli_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "millimeters";
       static constexpr std::string_view abbr   = "mm";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> milli_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, milli_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<3>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<milli_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-3>>;
    };
    struct centi_meters{};

    template<> struct unit_traits<centi_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "centimeters";
       static constexpr std::string_view abbr   = "cm";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> centi_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, centi_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<2>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<centi_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-2>>;
    };
    struct deci_meters{};

    template<> struct unit_traits<deci_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "decimeters";
       static constexpr std::string_view abbr   = "dm";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> deci_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, deci_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<1>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<deci_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-1>>;
    };
    struct deca_meters{};

    template<> struct unit_traits<deca_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "decameters";
       static constexpr std::string_view abbr   = "dam";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> deca_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, deca_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-1>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<deca_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<1>>;
    };
    struct hecto_meters{};

    template<> struct unit_traits<hecto_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "hectometers";
       static constexpr std::string_view abbr   = "hm";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> hecto_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, hecto_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-2>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<hecto_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<2>>;
    };
    struct kilo_meters{};

    template<> struct unit_traits<kilo_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "kilometers";
       static constexpr std::string_view abbr   = "km";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> kilo_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, kilo_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-3>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<kilo_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<3>>;
    };
    struct mega_meters{};

    template<> struct unit_traits<mega_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "megameters";
       static constexpr std::string_view abbr   = "Mm";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> mega_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, mega_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-6>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<mega_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<6>>;
    };
    struct giga_meters{};

    template<> struct unit_traits<giga_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "gigameters";
       static constexpr std::string_view abbr   = "Gm";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> giga_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, giga_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-9>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<giga_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<9>>;
    };
    struct tera_meters{};

    template<> struct unit_traits<tera_meters>
    {
       using dimension = lengthType;
       static constexpr std::string_view name   = "terameters";
       static constexpr std::string_view abbr   = "Tm";
       static constexpr std::string_view dimName= "length";
       static constexpr int id = 0;
    };

    // meters <-> tera_meters
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<meters, tera_meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<-12>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<tera_meters, meters>
    {
        using scale = factor_t<std::ratio<1>, pow10<12>>;
    };
}


#endif // STATIC_DIMENSION_LENGTH_H