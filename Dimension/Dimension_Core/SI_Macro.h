#ifndef DIMENSION_SI_MACRO_H
#define DIMENSION_SI_MACRO_H

namespace Dimension
{
    // Macro for SI prefixes
    struct Pico {};
    struct Nano {};
    struct Micro {};
    struct Milli {};
    struct Centi {};
    struct Deci {};
    struct Deca {};
    struct Hecto {};
    struct Kilo {};
    struct Mega {};
    struct Giga {};
    struct Tera {};

    // SI prefix factors
    template <typename Prefix>
    struct SIFactor;

    #define DEFINE_SI_FACTOR(Prefix, Factor) \
    template <> struct SIFactor<Prefix> { \
        static constexpr double value = Factor; \
    };

    // Define the conversion factors for SI prefixes
    DEFINE_SI_FACTOR(Pico, 1e-12)
    DEFINE_SI_FACTOR(Nano, 1e-9)
    DEFINE_SI_FACTOR(Micro, 1e-6)
    DEFINE_SI_FACTOR(Milli, 1e-3)
    DEFINE_SI_FACTOR(Centi, 1e-2)
    DEFINE_SI_FACTOR(Deci, 1e-1)
    DEFINE_SI_FACTOR(Deca, 1e1)
    DEFINE_SI_FACTOR(Hecto, 1e2)
    DEFINE_SI_FACTOR(Kilo, 1e3)
    DEFINE_SI_FACTOR(Mega, 1e6)
    DEFINE_SI_FACTOR(Giga, 1e9)
    DEFINE_SI_FACTOR(Tera, 1e12)

    #define STRINGIFY(x) #x
    #define CONCAT_AND_STRINGIFY(x, y) STRINGIFY(x##y)

    // Macro definition to create a prefixed factory function
    #define SI_PREFIX(baseName, baseAbbr, UnitType, Prefix, Abbr) \
    struct Prefix##baseName : public UnitType<Prefix##baseName, CONCAT_AND_STRINGIFY(Prefix, baseName), Abbr baseAbbr> { public: using UnitType::UnitType; }; \
    template<> struct Conversion<baseName, Prefix##baseName> { static constexpr PrecisionType slope = 1.0 / SIFactor<Prefix>::value; }; \
    template<> struct Conversion<Prefix##baseName, baseName> { static constexpr PrecisionType slope = SIFactor<Prefix>::value; };

    #define ALL_SI_PREFIXES(baseName, baseAbbr, UnitType) \
    SI_PREFIX(baseName, baseAbbr, UnitType, Pico, "p"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Nano, "n"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Micro, "u"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Milli, "m"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Centi, "c"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Deci, "d"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Deca, "da"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Hecto, "h"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Kilo, "k"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Mega, "M"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Giga, "G"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, Tera, "T");

} // end Dimension

#endif // DIMENSION_SI_MACRO_H