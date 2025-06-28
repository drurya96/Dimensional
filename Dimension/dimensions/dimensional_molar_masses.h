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
    static constexpr molar_mass<grams,moles> hydrogen(1.00784);
    static constexpr molar_mass<grams,moles> helium(4.002602);
    static constexpr molar_mass<grams,moles> lithium(6.941);
    static constexpr molar_mass<grams,moles> beryllium(9.0121831);
    static constexpr molar_mass<grams,moles> boron(10.81);
    static constexpr molar_mass<grams,moles> carbon(12.011);
    static constexpr molar_mass<grams,moles> nitrogen(14.007);
    static constexpr molar_mass<grams,moles> oxygen(15.999);
    static constexpr molar_mass<grams,moles> fluorine(18.998403163);
    static constexpr molar_mass<grams,moles> neon(20.1797);
    static constexpr molar_mass<grams,moles> sodium(22.98976928);
    static constexpr molar_mass<grams,moles> magnesium(24.305);
    static constexpr molar_mass<grams,moles> aluminum(26.9815385);
    static constexpr molar_mass<grams,moles> silicon(28.085);
    static constexpr molar_mass<grams,moles> phosphorus(30.973761998);
    static constexpr molar_mass<grams,moles> sulfur(32.06);
    static constexpr molar_mass<grams,moles> chlorine(35.45);
    static constexpr molar_mass<grams,moles> argon(39.948);
    static constexpr molar_mass<grams,moles> potassium(39.0983);
    static constexpr molar_mass<grams,moles> calcium(40.078);
    static constexpr molar_mass<grams,moles> scandium(44.955908);
    static constexpr molar_mass<grams,moles> titanium(47.867);
    static constexpr molar_mass<grams,moles> vanadium(50.9415);
    static constexpr molar_mass<grams,moles> chromium(51.9961);
    static constexpr molar_mass<grams,moles> manganese(54.938044);
    static constexpr molar_mass<grams,moles> iron(55.845);
    static constexpr molar_mass<grams,moles> cobalt(58.933194);
    static constexpr molar_mass<grams,moles> nickel(58.6934);
    static constexpr molar_mass<grams,moles> copper(63.546);
    static constexpr molar_mass<grams,moles> zinc(65.38);
    static constexpr molar_mass<grams,moles> gallium(69.723);
    static constexpr molar_mass<grams,moles> germanium(72.63);
    static constexpr molar_mass<grams,moles> arsenic(74.921595);
    static constexpr molar_mass<grams,moles> selenium(78.971);
    static constexpr molar_mass<grams,moles> bromine(79.904);
    static constexpr molar_mass<grams,moles> krypton(83.798);
    static constexpr molar_mass<grams,moles> rubidium(85.4678);
    static constexpr molar_mass<grams,moles> strontium(87.62);
    static constexpr molar_mass<grams,moles> yttrium(88.90584);
    static constexpr molar_mass<grams,moles> zirconium(91.224);
    static constexpr molar_mass<grams,moles> niobium(92.90637);
    static constexpr molar_mass<grams,moles> molybdenum(95.95);
    static constexpr molar_mass<grams,moles> technetium(98);
    static constexpr molar_mass<grams,moles> ruthenium(101.07);
    static constexpr molar_mass<grams,moles> rhodium(102.90550);
    static constexpr molar_mass<grams,moles> palladium(106.42);
    static constexpr molar_mass<grams,moles> silver(107.8682);
    static constexpr molar_mass<grams,moles> cadmium(112.414);
    static constexpr molar_mass<grams,moles> indium(114.818);
    static constexpr molar_mass<grams,moles> tin(118.710);
    static constexpr molar_mass<grams,moles> antimony(121.760);
    static constexpr molar_mass<grams,moles> tellurium(127.60);
    static constexpr molar_mass<grams,moles> iodine(126.90447);
    static constexpr molar_mass<grams,moles> xenon(131.293);
    static constexpr molar_mass<grams,moles> cesium(132.90545196);
    static constexpr molar_mass<grams,moles> barium(137.327);
    static constexpr molar_mass<grams,moles> lanthanum(138.90547);
    static constexpr molar_mass<grams,moles> cerium(140.116);
    static constexpr molar_mass<grams,moles> praseodymium(140.90766);
    static constexpr molar_mass<grams,moles> neodymium(144.242);
    static constexpr molar_mass<grams,moles> promethium(145);
    static constexpr molar_mass<grams,moles> samarium(150.36);
    static constexpr molar_mass<grams,moles> europium(151.964);
    static constexpr molar_mass<grams,moles> gadolinium(157.25);
    static constexpr molar_mass<grams,moles> terbium(158.92535);
    static constexpr molar_mass<grams,moles> dysprosium(162.500);
    static constexpr molar_mass<grams,moles> holmium(164.93033);
    static constexpr molar_mass<grams,moles> erbium(167.259);
    static constexpr molar_mass<grams,moles> thulium(168.93422);
    static constexpr molar_mass<grams,moles> ytterbium(173.045);
    static constexpr molar_mass<grams,moles> lutetium(174.9668);
    static constexpr molar_mass<grams,moles> hafnium(178.49);
    static constexpr molar_mass<grams,moles> tantalum(180.94788);
    static constexpr molar_mass<grams,moles> tungsten(183.84);
    static constexpr molar_mass<grams,moles> rhenium(186.207);
    static constexpr molar_mass<grams,moles> osmium(190.23);
    static constexpr molar_mass<grams,moles> iridium(192.217);
    static constexpr molar_mass<grams,moles> platinum(195.084);
    static constexpr molar_mass<grams,moles> gold(196.966569);
    static constexpr molar_mass<grams,moles> mercury(200.592);
    static constexpr molar_mass<grams,moles> thallium(204.38);
    static constexpr molar_mass<grams,moles> lead(207.2);
    static constexpr molar_mass<grams,moles> bismuth(208.98040);
    static constexpr molar_mass<grams,moles> polonium(209);
    static constexpr molar_mass<grams,moles> astatine(210);
    static constexpr molar_mass<grams,moles> radon(222);
    static constexpr molar_mass<grams,moles> francium(223);
    static constexpr molar_mass<grams,moles> radium(226);
    static constexpr molar_mass<grams,moles> actinium(227);
    static constexpr molar_mass<grams,moles> thorium(232.0377);
    static constexpr molar_mass<grams,moles> protactinium(231.03588);
    static constexpr molar_mass<grams,moles> uranium(238.02891);
    static constexpr molar_mass<grams,moles> neptunium(237);
    static constexpr molar_mass<grams,moles> plutonium(244);
    static constexpr molar_mass<grams,moles> americium(243);
    static constexpr molar_mass<grams,moles> curium(247);
    static constexpr molar_mass<grams,moles> berkelium(247);
    static constexpr molar_mass<grams,moles> californium(251);
    static constexpr molar_mass<grams,moles> einsteinium(252);
    static constexpr molar_mass<grams,moles> fermium(257);
    static constexpr molar_mass<grams,moles> mendelevium(258);
    static constexpr molar_mass<grams,moles> nobelium(259);
    static constexpr molar_mass<grams,moles> lawrencium(262);
    static constexpr molar_mass<grams,moles> rutherfordium(267);
    static constexpr molar_mass<grams,moles> dubnium(270);
    static constexpr molar_mass<grams,moles> seaborgium(271);
    static constexpr molar_mass<grams,moles> bohrium(270);
    static constexpr molar_mass<grams,moles> hassium(277);
    static constexpr molar_mass<grams,moles> meitnerium(278);
    static constexpr molar_mass<grams,moles> darmstadtium(281);
    static constexpr molar_mass<grams,moles> roentgenium(282);
    static constexpr molar_mass<grams,moles> copernicium(285);
    static constexpr molar_mass<grams,moles> nihonium(286);
    static constexpr molar_mass<grams,moles> flerovium(289);
    static constexpr molar_mass<grams,moles> moscovium(290);
    static constexpr molar_mass<grams,moles> livermorium(293);
    static constexpr molar_mass<grams,moles> tennessine(294);
    static constexpr molar_mass<grams,moles> oganesson(294);

    // Compounds
    // This cannot be exhaustive as there are limitless compounds.
    // Instead, these represent some "commonly used" compounds.
    // Add to this list as necessary/convenient.
    // Eventually, this may become a more robust solution, mirroring the typing
    //   of the dimensional library itself...
    static constexpr molar_mass<grams,moles> water(18.01528);
    static constexpr molar_mass<grams,moles> carbon_dioxide(44.009);
    static constexpr molar_mass<grams,moles> hydrogen_gas(2.01588);
    static constexpr molar_mass<grams,moles> oxygen_gas(31.9988);
    static constexpr molar_mass<grams,moles> nitrogen_gas(28.0134);
    static constexpr molar_mass<grams,moles> methane(16.04);
    static constexpr molar_mass<grams,moles> ammonia(17.0305);
    static constexpr molar_mass<grams,moles> sulfur_dioxide(64.066);
    static constexpr molar_mass<grams,moles> sodium_chloride(58.44);
    static constexpr molar_mass<grams,moles> glucose(180.156);
    static constexpr molar_mass<grams,moles> ethanol(46.068);
    static constexpr molar_mass<grams,moles> calcium_carbonate(100.0869);
    static constexpr molar_mass<grams,moles> methanol(32.04);
    static constexpr molar_mass<grams,moles> acetic_acid(60.052);
    static constexpr molar_mass<grams,moles> hydrochloric_acid(36.46);
    static constexpr molar_mass<grams,moles> nitric_acid(63.01);
    static constexpr molar_mass<grams,moles> potassium_nitrate(101.1032);
    static constexpr molar_mass<grams,moles> sodium_bicarbonate(84.0066);
    static constexpr molar_mass<grams,moles> magnesium_sulfate(120.366);
    static constexpr molar_mass<grams,moles> sodium_hydroxide(40.00);
    static constexpr molar_mass<grams,moles> calcium_hydroxide(74.0927);
    static constexpr molar_mass<grams,moles> benzene(78.1118);
    static constexpr molar_mass<grams,moles> propane(44.0956);
    static constexpr molar_mass<grams,moles> ethylene(28.054);
    static constexpr molar_mass<grams,moles> acetylene(26.0373);
    static constexpr molar_mass<grams,moles> formaldehyde(30.026);
    static constexpr molar_mass<grams,moles> acetone(58.08);
    static constexpr molar_mass<grams,moles> phosphoric_acid(97.994);
    static constexpr molar_mass<grams,moles> sulfuric_acid(98.079);
    static constexpr molar_mass<grams,moles> hydrogen_peroxide(34.0147);
    static constexpr molar_mass<grams,moles> ethylene_glycol(62.068);
    static constexpr molar_mass<grams,moles> sodium_sulfate(142.04);

}

#endif // STATIC_DIMENSION_MOLAR_MASSES_H
