#include "TrigonometricTable.h"

#include "Constants.h"

#include <cmath>

namespace phys
{

float TrigonometricTable::SIN_TABLE[TABLE_SIZE];
float TrigonometricTable::COS_TABLE[TABLE_SIZE];

void TrigonometricTable::init()
{
    static const float DEG_TO_RAD = PI_VALUE / 180.f;

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        SIN_TABLE[i] = sin(DEG_TO_RAD * i);
        COS_TABLE[i] = cos(DEG_TO_RAD * i);
    }
}

} // namespace phys