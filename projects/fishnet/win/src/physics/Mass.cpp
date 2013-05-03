#include "Mass.h"

#include "Constants.h"

#include <common/CommonException.h>

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

void Mass::draw() const
{
    if (!mDrawer._Empty())
        mDrawer(this);
}


void Mass::setPosition(const Vector2DFloat& position)
{
    mPosition = position;
}

void Mass::setDrawer(std::function<void (const Mass*)> drawer)
{
    mDrawer = drawer;
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