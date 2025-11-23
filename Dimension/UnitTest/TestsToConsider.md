This is from ChatGPT, ideas on tests to add.

----------------

Here’s a gap analysis with concrete additions. Each item lists a new test file to add (with its scope) and, where relevant, high-level cases to append to existing files.

New files to add

1) TestSIPrefixes.cpp — SI prefix generation & correctness

Auto-generated prefixed unit traits exist and round-trip correctly (e.g., milli/meters ↔ meters), including chained conversions (milli → kilo, etc.).

Multiplication/division of prefixed units simplifies to expected base units (e.g., km / s vs m / s).

Mixed-prefix algebra preserves exactness for simple rational values.

Prefixed symbols interact correctly with symbols and ratios (no double counting in factors).

2) TestDerivedDimensionAlgebra.cpp — dimensional identities & type algebra

Verify canonical identities: force = mass * acceleration, energy = force * length, power = energy / time, pressure = force / area, electric_field = potential / length, etc., via both numeric equality and type identity (static type checks).

Associativity/commutativity sanity (e.g., (a*b)*c vs a*(b*c) produce the same type and value).

Distributive patterns where supported (scalar distribution over quantities).

3) TestFactorSimplification.cpp — prime-factor exponents & cancellation

Products with inverse ratio_exponents cancel to pure scalars (dimensionless == 1.0; symbol lists empty).

Negative exponents and rational exponents reduce correctly (e.g., (m^3) / (m^(5/2)) = m^(1/2)).

Mixed ratio + sym factors: verify simplification removes zero-power terms; stable ordering of surviving terms.

4) TestConversionGraphConsistency.cpp — conversion round-trips & path independence

A → B → A yields exact identity for exact conversions; bounded tolerance where expected.

Direct vs composed path equivalence for a triangle of units (e.g., meters ↔ feet ↔ inches): value equality and same resulting type.

Verify scale(A→B) * scale(B→A) == 1 (dimensionless) for several unit pairs.

5) TestSerializationSymbols.cpp — streaming with symbols/ratios

Serialize/deserialize quantities carrying symbols::pi and nontrivial ratio_exponents; after round-trip: numeric equality, symbol tuple preserved, and order/duplication not introduced.

Cross-policy (ExactTagPolicy vs RawValuePolicy) parity for symbol-bearing values.

6) TestAnglesAndTrigUnits.cpp — angles & trig coverage beyond radians

Degrees path: sin(30°) == 0.5, etc., by converting degree inputs properly (ensure APIs accept angle<degrees> via conversion).

Inverse trig on boundary/near-boundary values with degree outputs validated via conversion (e.g., acos(±1)).

Large-magnitude angles normalization behavior if applicable (no catastrophic cancellation).

7) TestChronoInterop.cpp — std::chrono conversions

Construct timespan from common std::chrono durations (s, ms, min, h) and back; verify exact numeric mapping.

Arithmetic mixing timespan with std::chrono objects (where supported) or explicit conversions.

8) TestOrderingHashing.cpp — comparisons, total ordering, and hashing

Cross-unit comparisons at zero and nonzero (already covered for speed/zero; extend to length, mass).

std::hash/unordered containers behavior (if provided) for quantities with different units but equal physical value—keys collide as expected or remain distinct per design.

Strict-weak-ordering properties with heterogeneous units (all relational operators consistent).

9) TestRoundingEdgeCases.cpp — rounding semantics for negatives

floor/ceil/round for negative quantities (e.g., −1.3, −1.5, −1.8) vs mathematical expectations.

fmod with negative dividends/divisors; confirm sign conventions align with std::fmod.

10) TestPointExtended.cpp — additional point scenarios

Points with other affine frames beyond temperature: treat base-unit frames (meters, feet) as zero-offset frames and add another custom zero-offset frame; confirm conversions are pure scale changes.

Very-small delta temperatures (sub-Ulp) around offset edges (near 0 K / −273.15 °C) to ensure no bias at the offset boundary.

Scalar scaling not allowed for points (compile-time negative test via separate compile test if applicable) and runtime surfaces only permit point ± quantity, point − point.

11) TestGeneratedUnitsSmoke.cpp — generation smoke tests

Include a representative subset of auto-generated headers from your Jinja pipeline and instantiate a small selection of units/quantities to ensure the output compiles and links in a minimal TU.

Validate trait values (name, abbr, dimName) for a few generated units.

12) TestHelperUnitsAndAliases.cpp — helper units & alias integrity

Use HelperUnits (e.g., US volume chain: tsp → tbsp → fl oz → cup → pint → quart → gallon) across multiple steps; verify exact integer relationships.

Aliases or alternative names map to the same conversion factor and dimension.

13) TestNaNInfPropagation.cpp — special floating values

Quantities constructed with NaN, +Inf, -Inf maintain propagation through arithmetic and conversions in line with IEEE expectations (comparisons, trig where defined).

Serialization/deserialization of NaN/Inf (policy-dependent behavior) if supported.

14) TestDimensionlessMath.cpp — dimensionless functions

Where provided: exp/log/sqrt/cbrt (or Pow<1/2> via your API) on dimensionless values; ensure type remains dimensionless and numbers are correct.

sqrt(area) -> length and cbrt(volume) -> length if these helpers exist via Pow<1/2> / Pow<1/3>.

Add to existing files

TestFundamentalConversions.cpp

Angles: Add degree-based conversions in both directions beyond 180° (e.g., 90°, 360°) and non-trivial values (e.g., 123°).

Timespan: Add sub-second conversions (milliseconds ↔ seconds), and very large hour/day equivalents to probe scaling.

Mass: Add exactness checks for SI prefixes (kg ↔ g ↔ mg) to complement imperial tests.

Temperature (delta): Add negative delta temperatures and non-integer rational deltas (e.g., 12.5 K ↔ 22.5 R).

TestOperators.cpp

Mixed-unit comparison stability for additional dimensions (length, mass) mirroring the speed checks.

Associativity checks for chained products/divisions (values and types).

Root/Power: Add Pow<-1> (reciprocal), Pow<2> vs direct multiplication, and Pow<1> identity.

TestSerialization.cpp

Add cases with symbols present and with negative exponents in the unit list.

Round-trip extremely small and extremely large magnitudes to ensure no precision regression across policies.

TestSymbols.cpp

Multiply different symbol terms and verify symbol exponent aggregation (e.g., π * π^1/2 = π^3/2).

Division that fully cancels symbols yields empty symbol tuple; mixed with ratios still cancels correctly.

TestTrig.cpp

Add degree inputs (converted internally to radians) to each trig function suite for parity.

Boundary value tests near discontinuities (tan near π/2) using angle arguments that avoid undefined computations but approach limits.

TestCompoundConversions.cpp

Add a second compound dimension (e.g., acceleration, density) for multi-hop conversion checks mixing both numerator and denominator units (prefix + non-SI).

TestFunctions.cpp

Extend the NonBaseUnit section with a reverse conversion path and verify Conversion<meters, NonBaseUnit> presence yields identity over round-trips (where defined).

Add overload resolution sanity with additional function templates that accept different dimensions but convertible units, verifying no ambiguity.

PhysicsProblemsExamples/Example1.cpp

Add scenarios with cancelation to dimensionless (e.g., coefficient of drag computations yielding pure scalars).

Add a rotational energy/torque example: energy = torque * angle, power = torque * angular_speed to cover angular families in realistic settings.

TestSimplification.cpp

Add compile-time (constexpr) assertions for products generating higher powers and their square-root/reciprocal relationships via Pow<N> calls.

Confirm tuple order invariants for internal representation if exposed (e.g., canonical ordering unaffected by multiply order).

TestStd.cpp

Add std::accumulate and std::reduce on vectors of homogeneous quantities (if operators and iterators are supported).

Extend modf/fmod with negative values (cross-check against scalar semantics).

TestTemp.cpp

Populate with std::chrono interop and any temperature-point edge checks that are chronologically relevant (e.g., converting time-based rates multiplied by temperature deltas).

If any of the above APIs (hashing, chrono interop, sqrt/cbrt, reduce) aren’t implemented, keep the corresponding file as a TODO scaffold with GTEST_SKIP() so you remember to wire them once available.








Aim for ~10–15 targeted tests that run quickly and raise confidence without boiling the ocean:

Path-independence sanity

Round-trip A→B→A == identity and direct vs composed (e.g., m→ft→in vs m→in) for length, time, mass.

Prime-factor simplification smoke

Multiply a few ratio_exponent terms then divide to cancel back to scalar 1.0; ensure symbol tuple empties.

SI prefix generation spot-checks

milli/kilo round-trip on length and mass; km/s vs m/s equivalence.

Points beyond temperature (zero-offset frame)

point<meters,lengthType> arithmetic: point − point -> length, point ± length -> point, and no scaling of points.

Offset edge cases

Temperature near absolute zero: celsius(-273.15) ↔ kelvin(0) conversions and tiny deltas around the boundary.

Serialization with symbols/ratios

Round-trip a value carrying symbols::pi and a nontrivial ratio_exponent; equality and symbol list preserved.

Operators associativity/type identity

(a*b)*c vs a*(b*c) for types and values on a 3-way derived identity (e.g., energy = mass*length^2/time^2).

Rounding semantics for negatives

floor/ceil/round/fmod on negative quantities to match std behavior.

Extreme magnitude stability

Very large/small values across a conversion chain (e.g., AU↔m, ms↔h) to catch precision pathologies.




Important TODO: Make a "unitless" alias (one might already exist?)