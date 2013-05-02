#include "Mass.h"

#include "TrigonometricTable.h"
#include "Constants.h"

#include <common/CommonException.h>

#include <Windows.h>
#include <gl/GL.h>

namespace phys
{

Mass::Mass(float m, const Vector2DFloat& position)
    : mMass(m)
    , mPosition(position)
    , mVelocity()
    , mForce()
{
    // can't exist negative mass
    if(mMass < 0.f)
        throw common::CommonException("Mass.cpp;\nMass::Mass();\nmass can't be negative");
}


void Mass::applyForce(const Vector2DFloat& force)
{
    // add force
    mForce += force;
}

void Mass::reset()
{
    mForce.set(0.f, 0.f);
}

void Mass::simulate(float dt)
{
    // using Newton's second law: F = a*m
    // a - acseleration, m - mass

    // vel = a * dt;
    // change in velocity is added to the velocity
    mVelocity += (mForce / mMass) * dt;

    // pos = vel * dt + a * dt * dt / 2;
    // change in position is added to the position
    mPosition += mVelocity * dt + (mForce / mMass) * dt * dt / 2;
}

void Mass::draw()
{
    glBegin(GL_TRIANGLE_FAN);

    for (int i = 0; i < 360; i += 10)
        glVertex2f( mPosition.x() + POINT_RADIUS * TrigonometricTable::COS_TABLE[i],
                    mPosition.y() + POINT_RADIUS * TrigonometricTable::SIN_TABLE[i]);

    glEnd();
}


void Mass::setPosition(const Vector2DFloat& position)
{
    mPosition = position;
}


float Mass::mass() const
{
    return mMass;
}

const Vector2DFloat& Mass::position() const
{
    return mPosition;
}

const Vector2DFloat& Mass::velocity() const
{
    return mVelocity;
}

} // namespace phys