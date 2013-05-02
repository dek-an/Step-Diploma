#ifndef PHYS_FISHNET_H_
#define PHYS_FISHNET_H_

#include "Vector2D.h"

namespace phys
{

class Mass;
class Spring;

// class Fisnet - an object to represent a fisnet;
// fishnet consist of an array of springs
class Fishnet
{
public:

    Fishnet(
                int numOfMassesWidth,
                int numOfMassesHeight,
                float mass,
                const Vector2DFloat& leftConnectionPosition,
                float springsDeltaStrain,
                const Vector2DFloat& gravitation,
                float springsConstant,
                float springsLength,
                float springsAirFrictionConstant
            );
    ~Fishnet();

public:
    // the complete procedure of simulation
    void operate(float dt, Mass* pTouchedMass);
    // draws fishnet with all springs and masses
    void draw();
    // check wether if the point P(xCoord, yCoord) in the drawn mass;
    // returns the mass in which the point
    Mass* isPointInMass(float xCoord, float yCoord) const;

private:
    // reset state of the fishnet
    void reset();
    // apply forces for all springs
    void solve();
    // call simulate(dt) for every mass
    void simulate(float dt, Mass* pTouchedMass);

private:
    Fishnet(const Fishnet&);
    Fishnet& operator=(const Fishnet&);

private:
    typedef Mass* MassPtr;
    typedef Spring* SpringPtr;

    const int           mNumOfMassesWidth;  // width of fishnet
    const int           mNumOfMassesHeight; // height of fishnet
    const int           mNumOfSprings;      // num of springs
    const Vector2DFloat mLeftConnectionPosition;    // left top connection
    const Vector2DFloat mRightConnectionPosition;   // right top connection
    MassPtr**           mMasses;    // double array of pointers to masses
    SpringPtr*          mSprings;   // array of pointers to springs
};

} // namespace phys

#endif // PHYS_FISHNET_H_