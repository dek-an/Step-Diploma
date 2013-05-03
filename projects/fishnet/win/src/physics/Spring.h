#ifndef PHYS_SPRING_H_
#define PHYS_SPRING_H_

#include "Vector2D.h"

#include <functional>

namespace phys
{

class Mass;

// class Spring - an object to represent a spring with inner friction binding two masses
class Spring
{
public:
    Spring(
            Mass* mass1,
            Mass* mass2,
            float springConstant,
            float springLength,
            float airFrictionConstant,
            const Vector2DFloat& gravitation
            );

public:
    // method for applying forces
    void solve();
    // method for drawing spring (without masses)
    void draw() const;

    void setDrawer(std::function<void (const Mass*, const Mass*)> drawer);

private:
    Spring(const Spring&);
    Spring& operator=(const Spring&);

private:
    Mass* mMass1;   // mass at the tips of the spring
    Mass* mMass2;   // mass at the tips of the spring
    const float mSpringConstant;    // a constant to represent the stiffness of the spring
    const float mSpringLength;      // the length that the spring does not exert any force
    const float mAirFrictionConstant;   // a constant of air friction applied to masses
    const Vector2DFloat mGravitation;   // gravitation constant applied to masses

    std::function<void (const Mass*, const Mass*)> mDrawer;
};

} // namespace phys

#endif // PHYS_SPRING_H_