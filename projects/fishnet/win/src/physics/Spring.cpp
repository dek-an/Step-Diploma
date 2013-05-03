#include "Spring.h"

#include "Mass.h"

#include <common/CommonException.h>

namespace phys
{

Spring::Spring(
                Mass* mass1,
                Mass* mass2,
                float springConstant,
                float springLength,
                float airFrictionConstant,
                const Vector2DFloat& gravitation
               )
    : mMass1(mass1)
    , mMass2(mass2)
    , mSpringConstant(springConstant)
    , mSpringLength(springLength)
    , mAirFrictionConstant(airFrictionConstant)
    , mGravitation(gravitation)
{
    if (!mass1 || !mass2)
        throw common::CommonException("Spring.cpp;\nSpring::Spring();\nillegal parameters");
}


void Spring::solve()
{
    const Vector2DFloat springVector = mMass1->position() - mMass2->position(); // vector between two masses

    const float springLengthDiff = springVector.magnitude() - mSpringLength; // current length of spring

    // Hooke's law: F = -k * x; 
    // k - is the spring constant, x - is the displacement of the spring's end from its equilibrium position
    Vector2DFloat force;
    if (springLengthDiff > 0.f)
        force += springVector.normalized() * (-mSpringConstant) * springLengthDiff;

    // and apply force to masses with air friction and gravitation
    mMass1->applyForce(force - mMass1->velocity() * mAirFrictionConstant + mGravitation * mMass1->mass());
    mMass2->applyForce(-force - mMass2->velocity() * mAirFrictionConstant + mGravitation * mMass2->mass());
}

void Spring::draw() const
{
    if (!mDrawer._Empty())
        mDrawer(mMass1, mMass2);
}


void Spring::setDrawer(std::function<void (const Mass*, const Mass*)> drawer)
{
    mDrawer = drawer;
}

} // namespace phys