#include "Spring.h"

#include "Mass.h"

#include <Windows.h>
#include <gl/GL.h>

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

void Spring::draw()
{
    glBegin(GL_LINE_STRIP);

    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(mMass1->position().x(), mMass1->position().y());
    glVertex2f(mMass2->position().x(), mMass2->position().y());

    glEnd();
}

} // namespace phys