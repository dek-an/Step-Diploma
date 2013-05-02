#include "Fishnet.h"

#include "Mass.h"
#include "Spring.h"
#include "Constants.h"

#include <common/CommonException.h>

namespace phys
{

Fishnet::Fishnet(
                    int numOfMassesWidth,
                    int numOfMassesHeight,
                    float mass,
                    const Vector2DFloat& leftConnectionPosition,
                    float springsDeltaStrain,
                    const Vector2DFloat& gravitation,
                    float springsConstant,
                    float springsLength,
                    float springsAirFrictionConstant
                )
    : mNumOfMassesWidth(numOfMassesWidth)
    , mNumOfMassesHeight(numOfMassesHeight)
    , mNumOfSprings(numOfMassesHeight * (numOfMassesWidth - 1) + numOfMassesWidth * (numOfMassesHeight - 1))
    , mLeftConnectionPosition(leftConnectionPosition)
    , mRightConnectionPosition()
    , mMasses(0)
    , mSprings(0)
{
    // can't create fishnet with width or height <= 0
    if (numOfMassesWidth <= 0 || numOfMassesHeight <= 0)
        throw common::CommonException("Fishnet.cpp;\nFishnet::CFishnet();\nsize of array can't be <= 0");

    // mMasses double array initialization
    mMasses = new MassPtr*[numOfMassesHeight];
    for (int i = 0; i < numOfMassesHeight; i++)
    {
        mMasses[i] = new MassPtr[numOfMassesWidth];
        for (int j = 0; j < numOfMassesWidth; j++)
        {
            const Vector2DFloat massPosition(leftConnectionPosition +
                Vector2DFloat(j * (springsLength + springsDeltaStrain), i * springsLength));
            mMasses[i][j] = new Mass(mass, massPosition);
        }
    }

    // determinate the points of connection
    const_cast<Vector2DFloat&>(mRightConnectionPosition).set(mMasses[0][numOfMassesWidth - 1]->position());

    // mSprings array initialization
    mSprings = new SpringPtr[mNumOfSprings];
    int numOfInitializedSprings = 0;
    for (int i = 0; i < mNumOfMassesHeight; i++)
        for(int j = 0; j < mNumOfMassesWidth - 1; j++)
            mSprings[numOfInitializedSprings++] = new Spring(
                                                                mMasses[i][j],
                                                                mMasses[i][j + 1],
                                                                springsConstant,
                                                                springsLength,
                                                                springsAirFrictionConstant,
                                                                gravitation
                                                            );

    for(int j = 0; j < mNumOfMassesWidth; j++)
        for (int i = 0; i < mNumOfMassesHeight - 1; i++)
            mSprings[numOfInitializedSprings++] = new Spring(
                                                                mMasses[i][j],
                                                                mMasses[i + 1][j],
                                                                springsConstant,
                                                                springsLength,
                                                                springsAirFrictionConstant,
                                                                gravitation
                                                            );
}

Fishnet::~Fishnet()
{
    for (int i = 0; i < mNumOfMassesHeight; i++)
    {
        for (int j = 0; j < mNumOfMassesWidth; j++)
            delete mMasses[i][j];
        delete[] mMasses[i];
    }
    delete[] mMasses;

    for (int i = 0; i < mNumOfSprings; i++)
        delete mSprings[i];
    delete[] mSprings;
}

void Fishnet::reset()
{
    // for all masses in mMass call Init() method
    for (int i = 0; i < mNumOfMassesHeight; i++)
        for (int j = 0; j < mNumOfMassesWidth; j++)
            mMasses[i][j]->reset();
}

void Fishnet::solve()
{
    // apply forces for all masses
    for (int i = 0; i < mNumOfSprings; i++)
        mSprings[i]->solve();
}

void Fishnet::simulate(float dt, Mass *pTouchedMass)
{
    Vector2DFloat touchedPos; // position of the mass, which moving with mouse
    if (pTouchedMass)
        touchedPos = pTouchedMass->position();

    // calculate new positions and velocities for all masses
    for (int i = 0; i < mNumOfMassesHeight; i++)
        for (int j = 0; j < mNumOfMassesWidth; j++)
            mMasses[i][j]->simulate(dt);

    // left top an right top masses positions don't changing
    mMasses[0][0]->setPosition(mLeftConnectionPosition);
    mMasses[0][mNumOfMassesWidth - 1]->setPosition(mRightConnectionPosition);

    // touched mass too
    if (pTouchedMass)
        pTouchedMass->setPosition(touchedPos);
}

void Fishnet::operate(float dt, Mass *pTouchedMass)
{
    // complete procedure of simulation
    reset();
    solve();
    simulate(dt, pTouchedMass);
}

void Fishnet::draw()
{
    // draw all springs
    for (int i = 0; i < mNumOfSprings; i++)
        mSprings[i]->draw();

    // draw all masses
    for (int i = 0; i < mNumOfMassesHeight; i++)
        for (int j = 0; j < mNumOfMassesWidth; j++)
            mMasses[i][j]->draw();
}

Mass* Fishnet::isPointInMass(float xCoord, float yCoord) const
{
    // check all masses
    for (int i = 0; i < mNumOfMassesHeight; i++)
        for (int j = 0; j < mNumOfMassesWidth; j++)
            if (!((i == 0) && ((j == 0) || (j == mNumOfMassesWidth - 1))))
            {
                // determine the wether point is in any mass
                const bool isIn = (((xCoord - mMasses[i][j]->position().x()) * (xCoord - mMasses[i][j]->position().x()) + 
                                (yCoord - mMasses[i][j]->position().y()) * (yCoord - mMasses[i][j]->position().y())) <=
                                    (POINT_RADIUS + 1) * (POINT_RADIUS + 1));
                // if such mass is exist => return this mass
                if (isIn)
                    return mMasses[i][j];
            }

    return 0;
}

} // namespace phys