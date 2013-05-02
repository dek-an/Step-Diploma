#ifndef PHYS_TRIGONOMETRICTEBLE_H_
#define PHYS_TRIGONOMETRICTEBLE_H_

namespace phys
{

// sin and cos table of all angles (0 ~ 360);
// uses for nodes drawing
class TrigonometricTable
{
public:
    static void init();

    static const int TABLE_SIZE = 360;

    static float SIN_TABLE[TABLE_SIZE];
    static float COS_TABLE[TABLE_SIZE];
};

} // namespace phys

#endif // PHYS_TRIGONOMETRICTEBLE_H_