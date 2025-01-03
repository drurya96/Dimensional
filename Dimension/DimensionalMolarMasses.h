#ifndef STATIC_DIMENSION_MOLAR_MASSES_H
#define STATIC_DIMENSION_MOLAR_MASSES_H

#include "MolarMassDimension.h"

namespace Dimension::MolarMassConstants
{

    // Reminder: The units chosen for these constants are based on typical usage.
    // However, the same constant will work in other systems of measurement without adjustment.
    // Unit conversions are implicit, so even if these units don't match the use-case needed, they will still behave as expected.

    // Constant values have **NOT YET BEEN THOROUGHLY VERIFIED!**
    // If a constant is found to be incorrect, please open either an issue or an MR fixing the value

    // Elements
    // Note: these are molar masses of elements, not common forms such as O2, H2, He2, etc.
    static constexpr MolarMass<Grams,Moles> hydrogen(1.00784);
    static constexpr MolarMass<Grams,Moles> helium(4.002602);
    static constexpr MolarMass<Grams,Moles> lithium(6.941);
    static constexpr MolarMass<Grams,Moles> beryllium(9.0121831);
    static constexpr MolarMass<Grams,Moles> boron(10.81);
    static constexpr MolarMass<Grams,Moles> carbon(12.011);
    static constexpr MolarMass<Grams,Moles> nitrogen(14.007);
    static constexpr MolarMass<Grams,Moles> oxygen(15.999);
    static constexpr MolarMass<Grams,Moles> fluorine(18.998403163);
    static constexpr MolarMass<Grams,Moles> neon(20.1797);
    static constexpr MolarMass<Grams,Moles> sodium(22.98976928);
    static constexpr MolarMass<Grams,Moles> magnesium(24.305);
    static constexpr MolarMass<Grams,Moles> aluminum(26.9815385);
    static constexpr MolarMass<Grams,Moles> silicon(28.085);
    static constexpr MolarMass<Grams,Moles> phosphorus(30.973761998);
    static constexpr MolarMass<Grams,Moles> sulfur(32.06);
    static constexpr MolarMass<Grams,Moles> chlorine(35.45);
    static constexpr MolarMass<Grams,Moles> argon(39.948);
    static constexpr MolarMass<Grams,Moles> potassium(39.0983);
    static constexpr MolarMass<Grams,Moles> calcium(40.078);
    static constexpr MolarMass<Grams,Moles> scandium(44.955908);
    static constexpr MolarMass<Grams,Moles> titanium(47.867);
    static constexpr MolarMass<Grams,Moles> vanadium(50.9415);
    static constexpr MolarMass<Grams,Moles> chromium(51.9961);
    static constexpr MolarMass<Grams,Moles> manganese(54.938044);
    static constexpr MolarMass<Grams,Moles> iron(55.845);
    static constexpr MolarMass<Grams,Moles> cobalt(58.933194);
    static constexpr MolarMass<Grams,Moles> nickel(58.6934);
    static constexpr MolarMass<Grams,Moles> copper(63.546);
    static constexpr MolarMass<Grams,Moles> zinc(65.38);
    static constexpr MolarMass<Grams,Moles> gallium(69.723);
    static constexpr MolarMass<Grams,Moles> germanium(72.63);
    static constexpr MolarMass<Grams,Moles> arsenic(74.921595);
    static constexpr MolarMass<Grams,Moles> selenium(78.971);
    static constexpr MolarMass<Grams,Moles> bromine(79.904);
    static constexpr MolarMass<Grams,Moles> krypton(83.798);
    static constexpr MolarMass<Grams,Moles> rubidium(85.4678);
    static constexpr MolarMass<Grams,Moles> strontium(87.62);
    static constexpr MolarMass<Grams,Moles> yttrium(88.90584);
    static constexpr MolarMass<Grams,Moles> zirconium(91.224);
    static constexpr MolarMass<Grams,Moles> niobium(92.90637);
    static constexpr MolarMass<Grams,Moles> molybdenum(95.95);
    static constexpr MolarMass<Grams,Moles> technetium(98);
    static constexpr MolarMass<Grams,Moles> ruthenium(101.07);
    static constexpr MolarMass<Grams,Moles> rhodium(102.90550);
    static constexpr MolarMass<Grams,Moles> palladium(106.42);
    static constexpr MolarMass<Grams,Moles> silver(107.8682);
    static constexpr MolarMass<Grams,Moles> cadmium(112.414);
    static constexpr MolarMass<Grams,Moles> indium(114.818);
    static constexpr MolarMass<Grams,Moles> tin(118.710);
    static constexpr MolarMass<Grams,Moles> antimony(121.760);
    static constexpr MolarMass<Grams,Moles> tellurium(127.60);
    static constexpr MolarMass<Grams,Moles> iodine(126.90447);
    static constexpr MolarMass<Grams,Moles> xenon(131.293);
    static constexpr MolarMass<Grams,Moles> cesium(132.90545196);
    static constexpr MolarMass<Grams,Moles> barium(137.327);
    static constexpr MolarMass<Grams,Moles> lanthanum(138.90547);
    static constexpr MolarMass<Grams,Moles> cerium(140.116);
    static constexpr MolarMass<Grams,Moles> praseodymium(140.90766);
    static constexpr MolarMass<Grams,Moles> neodymium(144.242);
    static constexpr MolarMass<Grams,Moles> promethium(145);
    static constexpr MolarMass<Grams,Moles> samarium(150.36);
    static constexpr MolarMass<Grams,Moles> europium(151.964);
    static constexpr MolarMass<Grams,Moles> gadolinium(157.25);
    static constexpr MolarMass<Grams,Moles> terbium(158.92535);
    static constexpr MolarMass<Grams,Moles> dysprosium(162.500);
    static constexpr MolarMass<Grams,Moles> holmium(164.93033);
    static constexpr MolarMass<Grams,Moles> erbium(167.259);
    static constexpr MolarMass<Grams,Moles> thulium(168.93422);
    static constexpr MolarMass<Grams,Moles> ytterbium(173.045);
    static constexpr MolarMass<Grams,Moles> lutetium(174.9668);
    static constexpr MolarMass<Grams,Moles> hafnium(178.49);
    static constexpr MolarMass<Grams,Moles> tantalum(180.94788);
    static constexpr MolarMass<Grams,Moles> tungsten(183.84);
    static constexpr MolarMass<Grams,Moles> rhenium(186.207);
    static constexpr MolarMass<Grams,Moles> osmium(190.23);
    static constexpr MolarMass<Grams,Moles> iridium(192.217);
    static constexpr MolarMass<Grams,Moles> platinum(195.084);
    static constexpr MolarMass<Grams,Moles> gold(196.966569);
    static constexpr MolarMass<Grams,Moles> mercury(200.592);
    static constexpr MolarMass<Grams,Moles> thallium(204.38);
    static constexpr MolarMass<Grams,Moles> lead(207.2);
    static constexpr MolarMass<Grams,Moles> bismuth(208.98040);
    static constexpr MolarMass<Grams,Moles> polonium(209);
    static constexpr MolarMass<Grams,Moles> astatine(210);
    static constexpr MolarMass<Grams,Moles> radon(222);
    static constexpr MolarMass<Grams,Moles> francium(223);
    static constexpr MolarMass<Grams,Moles> radium(226);
    static constexpr MolarMass<Grams,Moles> actinium(227);
    static constexpr MolarMass<Grams,Moles> thorium(232.0377);
    static constexpr MolarMass<Grams,Moles> protactinium(231.03588);
    static constexpr MolarMass<Grams,Moles> uranium(238.02891);
    static constexpr MolarMass<Grams,Moles> neptunium(237);
    static constexpr MolarMass<Grams,Moles> plutonium(244);
    static constexpr MolarMass<Grams,Moles> americium(243);
    static constexpr MolarMass<Grams,Moles> curium(247);
    static constexpr MolarMass<Grams,Moles> berkelium(247);
    static constexpr MolarMass<Grams,Moles> californium(251);
    static constexpr MolarMass<Grams,Moles> einsteinium(252);
    static constexpr MolarMass<Grams,Moles> fermium(257);
    static constexpr MolarMass<Grams,Moles> mendelevium(258);
    static constexpr MolarMass<Grams,Moles> nobelium(259);
    static constexpr MolarMass<Grams,Moles> lawrencium(262);
    static constexpr MolarMass<Grams,Moles> rutherfordium(267);
    static constexpr MolarMass<Grams,Moles> dubnium(270);
    static constexpr MolarMass<Grams,Moles> seaborgium(271);
    static constexpr MolarMass<Grams,Moles> bohrium(270);
    static constexpr MolarMass<Grams,Moles> hassium(277);
    static constexpr MolarMass<Grams,Moles> meitnerium(278);
    static constexpr MolarMass<Grams,Moles> darmstadtium(281);
    static constexpr MolarMass<Grams,Moles> roentgenium(282);
    static constexpr MolarMass<Grams,Moles> copernicium(285);
    static constexpr MolarMass<Grams,Moles> nihonium(286);
    static constexpr MolarMass<Grams,Moles> flerovium(289);
    static constexpr MolarMass<Grams,Moles> moscovium(290);
    static constexpr MolarMass<Grams,Moles> livermorium(293);
    static constexpr MolarMass<Grams,Moles> tennessine(294);
    static constexpr MolarMass<Grams,Moles> oganesson(294);

    // Compounds
    // This cannot be exhaustive as there are limitless compounds.
    // Instead, these represent some "commonly used" compounds.
    // Add to this list as necessary/convenient.
    // Eventually, this may become a more robust solution, mirroring the typing
    //   of the dimensional library itself...
    static constexpr MolarMass<Grams,Moles> water(18.01528);
    static constexpr MolarMass<Grams,Moles> carbon_dioxide(44.009);
    static constexpr MolarMass<Grams,Moles> hydrogen_gas(2.01588);
    static constexpr MolarMass<Grams,Moles> oxygen_gas(31.9988);
    static constexpr MolarMass<Grams,Moles> nitrogen_gas(28.0134);
    static constexpr MolarMass<Grams,Moles> methane(16.04);
    static constexpr MolarMass<Grams,Moles> ammonia(17.0305);
    static constexpr MolarMass<Grams,Moles> sulfur_dioxide(64.066);
    static constexpr MolarMass<Grams,Moles> sodium_chloride(58.44);
    static constexpr MolarMass<Grams,Moles> glucose(180.156);
    static constexpr MolarMass<Grams,Moles> ethanol(46.068);
    static constexpr MolarMass<Grams,Moles> calcium_carbonate(100.0869);
    static constexpr MolarMass<Grams,Moles> methanol(32.04);
    static constexpr MolarMass<Grams,Moles> acetic_acid(60.052);
    static constexpr MolarMass<Grams,Moles> hydrochloric_acid(36.46);
    static constexpr MolarMass<Grams,Moles> nitric_acid(63.01);
    static constexpr MolarMass<Grams,Moles> potassium_nitrate(101.1032);
    static constexpr MolarMass<Grams,Moles> sodium_bicarbonate(84.0066);
    static constexpr MolarMass<Grams,Moles> magnesium_sulfate(120.366);
    static constexpr MolarMass<Grams,Moles> sodium_hydroxide(40.00);
    static constexpr MolarMass<Grams,Moles> calcium_hydroxide(74.0927);
    static constexpr MolarMass<Grams,Moles> benzene(78.1118);
    static constexpr MolarMass<Grams,Moles> propane(44.0956);
    static constexpr MolarMass<Grams,Moles> ethylene(28.054);
    static constexpr MolarMass<Grams,Moles> acetylene(26.0373);
    static constexpr MolarMass<Grams,Moles> formaldehyde(30.026);
    static constexpr MolarMass<Grams,Moles> acetone(58.08);
    static constexpr MolarMass<Grams,Moles> phosphoric_acid(97.994);
    static constexpr MolarMass<Grams,Moles> sulfuric_acid(98.079);
    static constexpr MolarMass<Grams,Moles> hydrogen_peroxide(34.0147);
    static constexpr MolarMass<Grams,Moles> ethylene_glycol(62.068);
    static constexpr MolarMass<Grams,Moles> sodium_sulfate(142.04);

}

#endif // STATIC_DIMENSION_MOLAR_MASSES_H
