#ifndef STATIC_DIMENSION_CONSTANTS_H
#define STATIC_DIMENSION_CONSTANTS_H

#include "EnergyDimension.h"
#include "TemperatureDimension.h"
#include "AmountDimension.h"
#include "LengthDimension.h"
#include "ForceDimension.h"

namespace Dimension
{

    // Reminder: The units chosen for these constants are based on typical usage.
    // However, the same constant will work in other systems of measurement without adjustement.
    // Unit conversions are implicit, so even if these units don't match the use-case needed, they will still behave as expected.

    static constexpr BaseDimension<std::tuple<Joules>, std::tuple<Quantity<Kelvin>, Moles>> ideal_gas_constant(8.31446261815324);
}

#endif // STATIC_DIMENSION_CONSTANTS_H
