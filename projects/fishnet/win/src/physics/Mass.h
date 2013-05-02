#ifndef PHYS_MASS_H_
#define PHYS_MASS_H_

#include "Vector2D.h"

namespace phys
{

// class Mass - an object to represent a mass
// any mass characterizied by values: 1) mass
//                                    2) position (Vector2D)
//                                    3) velocity
//                                    4) applied force
class Mass
{
public:
    Mass(float m, const Vector2DFloat& position);

public:
    // Reset the state of mass
    void reset();
    // add external force to the mass
    void applyForce(const Vector2DFloat& force);
    // calculates the new velocity and new position of the mass according to change in time (dt)
    void simulate(float dt);
    // mass drawing
    void draw();

    void setPosition(const Vector2DFloat& position);

    float mass() const;
    const Vector2DFloat& position() const;
    const Vector2DFloat& velocity() const;

private:
    Mass(const Mass&);
    Mass& operator=(const Mass&);

private:
    float mMass;    // the mass value
    Vector2DFloat mPosition;    // Position in a plane
    Vector2DFloat mVelocity;    // Velocity
    Vector2DFloat mForce;       // Force applied on this Mass
};

} // namespace phys

#endif // PHYS_MASS_H_