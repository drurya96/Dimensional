#ifndef DIMENSION_SI_MACRO_H
#define DIMENSION_SI_MACRO_H

namespace dimension
{
    // Macro for SI prefixes
    struct pico {};
    struct nano {};
    struct micro {};
    struct milli {};
    struct centi {};
    struct deci {};
    struct deca {};
    struct hecto {};
    struct kilo {};
    struct mega {};
    struct giga {};
    struct tera {};

    // SI prefix factors
    template <typename Prefix>
    struct SIFactor;

    #define DEFINE_SI_FACTOR(Prefix, Factor) \
    template <> struct SIFactor<Prefix> { \
        static constexpr double value = Factor; \
    };

    // Define the conversion factors for SI prefixes
    DEFINE_SI_FACTOR(pico, 1e-12)
    DEFINE_SI_FACTOR(nano, 1e-9)
    DEFINE_SI_FACTOR(micro, 1e-6)
    DEFINE_SI_FACTOR(milli, 1e-3)
    DEFINE_SI_FACTOR(centi, 1e-2)
    DEFINE_SI_FACTOR(deci, 1e-1)
    DEFINE_SI_FACTOR(deca, 1e1)
    DEFINE_SI_FACTOR(hecto, 1e2)
    DEFINE_SI_FACTOR(kilo, 1e3)
    DEFINE_SI_FACTOR(mega, 1e6)
    DEFINE_SI_FACTOR(giga, 1e9)
    DEFINE_SI_FACTOR(tera, 1e12)
/*
    #define STRINGIFY(x) #x
    #define CONCAT_AND_STRINGIFY(x, y) STRINGIFY(x##y)

    // Macro definition to create a prefixed factory function
    #define SI_PREFIX(baseName, baseAbbr, UnitType, Prefix, Abbr) \
    struct Prefix##baseName : public UnitType<Prefix##baseName, CONCAT_AND_STRINGIFY(Prefix, baseName), Abbr baseAbbr> { public: using UnitType::UnitType; }; \
    template<> struct Conversion<baseName, Prefix##baseName> { static constexpr PrecisionType slope = 1.0 / SIFactor<Prefix>::value; }; \
    template<> struct Conversion<Prefix##baseName, baseName> { static constexpr PrecisionType slope = SIFactor<Prefix>::value; };
*/

    #define STRINGIFY(x) #x
    #define CONCAT3_INNER(a, b, c) a##b##c
    #define CONCAT3(a, b, c) CONCAT3_INNER(a, b, c)
    #define CONCAT_AND_STRINGIFY(x, y) STRINGIFY(CONCAT3(x, _, y))

    #define SI_PREFIX(baseName, baseAbbr, UnitType, Prefix, Abbr) \
    struct CONCAT3(Prefix, _, baseName) : public UnitType<CONCAT3(Prefix, _, baseName), CONCAT_AND_STRINGIFY(Prefix, baseName), Abbr baseAbbr> { public: using UnitType::UnitType; }; \
    template<> struct Conversion<baseName, CONCAT3(Prefix, _, baseName)> { static constexpr PrecisionType slope = 1.0 / SIFactor<Prefix>::value; }; \
    template<> struct Conversion<CONCAT3(Prefix, _, baseName), baseName> { static constexpr PrecisionType slope = SIFactor<Prefix>::value; };


    #define ALL_SI_PREFIXES(baseName, baseAbbr, UnitType) \
    SI_PREFIX(baseName, baseAbbr, UnitType, pico, "p"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, nano, "n"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, micro, "u"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, milli, "m"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, centi, "c"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, deci, "d"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, deca, "da"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, hecto, "h"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, kilo, "k"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, mega, "M"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, giga, "G"); \
    SI_PREFIX(baseName, baseAbbr, UnitType, tera, "T");

} // end Dimension

#endif // DIMENSION_SI_MACRO_H