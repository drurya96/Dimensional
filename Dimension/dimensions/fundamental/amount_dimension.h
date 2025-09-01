#ifndef STATIC_DIMENSION_AMOUNT_H
#define STATIC_DIMENSION_AMOUNT_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/amount_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{
  struct moles{};

  template<> struct unit_traits<moles>
  {
     using dimension = amountType;
     static constexpr std::string_view name   = "Moles";
     static constexpr std::string_view abbr   = "mol";
     static constexpr std::string_view dimName= "amount";
     static constexpr int id = 0;
  };
  struct pound_moles{};

  template<> struct unit_traits<pound_moles>
  {
     using dimension = amountType;
     static constexpr std::string_view name   = "Pound Moles";
     static constexpr std::string_view abbr   = "lbmol";
     static constexpr std::string_view dimName= "amount";
     static constexpr int id = 0;
  };

  template<> struct Conversion<moles, pound_moles>
  {
     using scale = factor_t<
        std::ratio<100000, 45359237>>;
  };

    struct pico_moles{};

    template<> struct unit_traits<pico_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "picoMoles";
       static constexpr std::string_view abbr   = "pmol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> pico_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, pico_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<12>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<pico_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-12>>;
    };
    struct nano_moles{};

    template<> struct unit_traits<nano_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "nanoMoles";
       static constexpr std::string_view abbr   = "nmol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> nano_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, nano_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<9>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<nano_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-9>>;
    };
    struct micro_moles{};

    template<> struct unit_traits<micro_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "microMoles";
       static constexpr std::string_view abbr   = "umol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> micro_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, micro_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<6>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<micro_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-6>>;
    };
    struct milli_moles{};

    template<> struct unit_traits<milli_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "milliMoles";
       static constexpr std::string_view abbr   = "mmol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> milli_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, milli_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<3>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<milli_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-3>>;
    };
    struct centi_moles{};

    template<> struct unit_traits<centi_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "centiMoles";
       static constexpr std::string_view abbr   = "cmol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> centi_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, centi_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<2>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<centi_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-2>>;
    };
    struct deci_moles{};

    template<> struct unit_traits<deci_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "deciMoles";
       static constexpr std::string_view abbr   = "dmol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> deci_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, deci_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<1>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<deci_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-1>>;
    };
    struct deca_moles{};

    template<> struct unit_traits<deca_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "decaMoles";
       static constexpr std::string_view abbr   = "damol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> deca_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, deca_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-1>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<deca_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<1>>;
    };
    struct hecto_moles{};

    template<> struct unit_traits<hecto_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "hectoMoles";
       static constexpr std::string_view abbr   = "hmol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> hecto_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, hecto_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-2>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<hecto_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<2>>;
    };
    struct kilo_moles{};

    template<> struct unit_traits<kilo_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "kiloMoles";
       static constexpr std::string_view abbr   = "kmol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> kilo_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, kilo_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-3>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<kilo_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<3>>;
    };
    struct mega_moles{};

    template<> struct unit_traits<mega_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "megaMoles";
       static constexpr std::string_view abbr   = "Mmol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> mega_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, mega_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-6>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<mega_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<6>>;
    };
    struct giga_moles{};

    template<> struct unit_traits<giga_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "gigaMoles";
       static constexpr std::string_view abbr   = "Gmol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> giga_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, giga_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-9>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<giga_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<9>>;
    };
    struct tera_moles{};

    template<> struct unit_traits<tera_moles>
    {
       using dimension = amountType;
       static constexpr std::string_view name   = "teraMoles";
       static constexpr std::string_view abbr   = "Tmol";
       static constexpr std::string_view dimName= "amount";
       static constexpr int id = 0;
    };

    // moles <-> tera_moles
    // base->prefixed multiplies by 10^{-exp}
    template<> struct Conversion<moles, tera_moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<-12>>;
    };

    // prefixed->base multiplies by 10^{+exp}
    template<> struct Conversion<tera_moles, moles>
    {
        using scale = factor_t<std::ratio<1>, pow10<12>>;
    };
}


#endif // STATIC_DIMENSION_AMOUNT_H