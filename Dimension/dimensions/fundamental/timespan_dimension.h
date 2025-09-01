#ifndef STATIC_DIMENSION_TIMESPAN_H
#define STATIC_DIMENSION_TIMESPAN_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/timespan_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{
  struct seconds{};

  template<> struct unit_traits<seconds>
  {
     using dimension = timespanType;
     static constexpr std::string_view name   = "seconds";
     static constexpr std::string_view abbr   = "s";
     static constexpr std::string_view dimName= "timespan";
     static constexpr int id = 0;
  };
  struct minutes{};

  template<> struct unit_traits<minutes>
  {
     using dimension = timespanType;
     static constexpr std::string_view name   = "minutes";
     static constexpr std::string_view abbr   = "min";
     static constexpr std::string_view dimName= "timespan";
     static constexpr int id = 0;
  };
  struct hours{};

  template<> struct unit_traits<hours>
  {
     using dimension = timespanType;
     static constexpr std::string_view name   = "Hours";
     static constexpr std::string_view abbr   = "h";
     static constexpr std::string_view dimName= "timespan";
     static constexpr int id = 0;
  };

  template<> struct Conversion<seconds, minutes>
  {
     using scale = factor_t<
        std::ratio<1, 60>>;
  };
  template<> struct Conversion<seconds, hours>
  {
     using scale = factor_t<
        std::ratio<1, 3600>>;
  };

    struct pico_seconds{};

    template<> struct unit_traits<pico_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "picoseconds";
       static constexpr std::string_view abbr   = "ps";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> pico_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, pico_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<12>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<pico_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-12>>;
    };
    struct nano_seconds{};

    template<> struct unit_traits<nano_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "nanoseconds";
       static constexpr std::string_view abbr   = "ns";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> nano_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, nano_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<9>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<nano_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-9>>;
    };
    struct micro_seconds{};

    template<> struct unit_traits<micro_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "microseconds";
       static constexpr std::string_view abbr   = "us";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> micro_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, micro_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<6>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<micro_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-6>>;
    };
    struct milli_seconds{};

    template<> struct unit_traits<milli_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "milliseconds";
       static constexpr std::string_view abbr   = "ms";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> milli_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, milli_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<3>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<milli_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-3>>;
    };
    struct centi_seconds{};

    template<> struct unit_traits<centi_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "centiseconds";
       static constexpr std::string_view abbr   = "cs";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> centi_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, centi_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<2>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<centi_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-2>>;
    };
    struct deci_seconds{};

    template<> struct unit_traits<deci_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "deciseconds";
       static constexpr std::string_view abbr   = "ds";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> deci_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, deci_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<1>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<deci_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-1>>;
    };
    struct deca_seconds{};

    template<> struct unit_traits<deca_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "decaseconds";
       static constexpr std::string_view abbr   = "das";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> deca_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, deca_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-1>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<deca_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<1>>;
    };
    struct hecto_seconds{};

    template<> struct unit_traits<hecto_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "hectoseconds";
       static constexpr std::string_view abbr   = "hs";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> hecto_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, hecto_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-2>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<hecto_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<2>>;
    };
    struct kilo_seconds{};

    template<> struct unit_traits<kilo_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "kiloseconds";
       static constexpr std::string_view abbr   = "ks";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> kilo_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, kilo_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-3>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<kilo_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<3>>;
    };
    struct mega_seconds{};

    template<> struct unit_traits<mega_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "megaseconds";
       static constexpr std::string_view abbr   = "Ms";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> mega_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, mega_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-6>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<mega_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<6>>;
    };
    struct giga_seconds{};

    template<> struct unit_traits<giga_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "gigaseconds";
       static constexpr std::string_view abbr   = "Gs";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> giga_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, giga_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-9>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<giga_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<9>>;
    };
    struct tera_seconds{};

    template<> struct unit_traits<tera_seconds>
    {
       using dimension = timespanType;
       static constexpr std::string_view name   = "teraseconds";
       static constexpr std::string_view abbr   = "Ts";
       static constexpr std::string_view dimName= "timespan";
       static constexpr int id = 0;
    };

    // seconds <-> tera_seconds
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<seconds, tera_seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<-12>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<tera_seconds, seconds>
    {
        using scale = factor_t<std::ratio<1>, pow10<12>>;
    };
}


#endif // STATIC_DIMENSION_TIMESPAN_H