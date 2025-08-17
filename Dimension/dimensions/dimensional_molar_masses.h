#ifndef STATIC_DIMENSION_MOLAR_MASSES_H
#define STATIC_DIMENSION_MOLAR_MASSES_H

#include "dimensions.h"

namespace dimension::molar_mass_constants
{

    // Reminder: The units chosen for these constants are based on typical usage.
    // However, the same constant will work in other systems of measurement without adjustment.
    // Unit conversions are implicit, so even if these units don't match the use-case needed, they will still behave as expected.

    // Constant values have **NOT YET BEEN THOROUGHLY VERIFIED!**
    // If a constant is found to be incorrect, please open either an issue or an MR fixing the value

    // Elements
    // Note: these are molar masses of elements, not common forms such as O2, H2, He2, etc.
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> hydrogen{1.00784};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> helium{4.002602};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> lithium{6.941};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> beryllium{9.0121831};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> boron{10.81};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> carbon{12.011};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> nitrogen{14.007};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> oxygen{15.999};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> fluorine{18.998403163};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> neon{20.1797};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> sodium{22.98976928};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> magnesium{24.305};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> aluminum{26.9815385};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> silicon{28.085};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> phosphorus{30.973761998};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> sulfur{32.06};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> chlorine{35.45};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> argon{39.948};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> potassium{39.0983};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> calcium{40.078};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> scandium{44.955908};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> titanium{47.867};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> vanadium{50.9415};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> chromium{51.9961};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> manganese{54.938044};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> iron{55.845};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> cobalt{58.933194};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> nickel{58.6934};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> copper{63.546};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> zinc{65.38};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> gallium{69.723};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> germanium{72.63};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> arsenic{74.921595};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> selenium{78.971};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> bromine{79.904};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> krypton{83.798};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> rubidium{85.4678};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> strontium{87.62};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> yttrium{88.90584};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> zirconium{91.224};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> niobium{92.90637};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> molybdenum{95.95};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> technetium{98};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> ruthenium{101.07};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> rhodium{102.90550};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> palladium{106.42};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> silver{107.8682};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> cadmium{112.414};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> indium{114.818};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> tin{118.710};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> antimony{121.760};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> tellurium{127.60};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> iodine{126.90447};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> xenon{131.293};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> cesium{132.90545196};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> barium{137.327};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> lanthanum{138.90547};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> cerium{140.116};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> praseodymium{140.90766};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> neodymium{144.242};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> promethium{145};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> samarium{150.36};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> europium{151.964};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> gadolinium{157.25};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> terbium{158.92535};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> dysprosium{162.500};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> holmium{164.93033};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> erbium{167.259};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> thulium{168.93422};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> ytterbium{173.045};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> lutetium{174.9668};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> hafnium{178.49};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> tantalum{180.94788};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> tungsten{183.84};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> rhenium{186.207};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> osmium{190.23};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> iridium{192.217};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> platinum{195.084};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> gold{196.966569};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> mercury{200.592};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> thallium{204.38};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> lead{207.2};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> bismuth{208.98040};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> polonium{209};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> astatine{210};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> radon{222};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> francium{223};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> radium{226};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> actinium{227};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> thorium{232.0377};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> protactinium{231.03588};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> uranium{238.02891};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> neptunium{237};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> plutonium{244};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> americium{243};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> curium{247};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> berkelium{247};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> californium{251};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> einsteinium{252};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> fermium{257};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> mendelevium{258};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> nobelium{259};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> lawrencium{262};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> rutherfordium{267};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> dubnium{270};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> seaborgium{271};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> bohrium{270};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> hassium{277};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> meitnerium{278};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> darmstadtium{281};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> roentgenium{282};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> copernicium{285};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> nihonium{286};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> flerovium{289};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> moscovium{290};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> livermorium{293};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> tennessine{294};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> oganesson{294};

    // Compounds
    // This cannot be exhaustive as there are limitless compounds.
    // Instead, these represent some "commonly used" compounds.
    // Add to this list as necessary/convenient.
    // Eventually, this may become a more robust solution, mirroring the typing
    //   of the dimensional library itself...
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> water{18.01528};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> carbon_dioxide{44.009};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> hydrogen_gas{2.01588};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> oxygen_gas{31.9988};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> nitrogen_gas{28.0134};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> methane{16.04};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> ammonia{17.0305};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> sulfur_dioxide{64.066};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> sodium_chloride{58.44};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> glucose{180.156};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> ethanol{46.068};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> calcium_carbonate{100.0869};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> methanol{32.04};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> acetic_acid{60.052};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> hydrochloric_acid{36.46};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> nitric_acid{63.01};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> potassium_nitrate{101.1032};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> sodium_bicarbonate{84.0066};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> magnesium_sulfate{120.366};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> sodium_hydroxide{40.00};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> calcium_hydroxide{74.0927};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> benzene{78.1118};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> propane{44.0956};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> ethylene{28.054};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> acetylene{26.0373};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> formaldehyde{30.026};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> acetone{58.08};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> phosphoric_acid{97.994};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> sulfuric_acid{98.079};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> hydrogen_peroxide{34.0147};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> ethylene_glycol{62.068};
    [[maybe_unused]] inline constexpr molar_mass<grams,moles> sodium_sulfate{142.04};

}

#endif // STATIC_DIMENSION_MOLAR_MASSES_H
