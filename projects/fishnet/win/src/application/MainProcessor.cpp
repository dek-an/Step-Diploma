#include "MainProcessor.h"

#include "GLDrawer.h"

#include <physics/Constants.h>
#include <physics/Fishnet.h>
#include <physics/Mass.h>

namespace application
{

using phys::Vector2DFloat;

MainProcessor::MainProcessor()
    : m_pFishnet(new phys::Fishnet(
        phys::NUM_OF_MASSES_WIDTH,
        phys::NUM_OF_MASSES_HEIGHT,
        phys::MASS,
        phys::LEFT_CONNECTION,
        phys::SPRING_DELTA_STRAIN,
        phys::GRAVITATION,
        phys::SPRING_CONSTANT,
        phys::SPRING_LENGTH,
        phys::AIR_FRICTION_CONSTANT))
    , m_pTouchedMass(0)
{}

MainProcessor::~MainProcessor()
{
    delete m_pFishnet;
}

void MainProcessor::update()
{
    static float step = phys::DT / phys::NUM_OF_INTEGRATE_STEPS;   // calculate integrate step
    for (float i = step; i <= phys::DT; i += step)
        m_pFishnet->operate(i, m_pTouchedMass);
}

void MainProcessor::draw() const
{
    m_pFishnet->draw(); // draw the fishnet
}


void MainProcessor::setDrawer(GLDrawer* drawer)
{
    if (!drawer)
        return;

    m_pFishnet->setSpringDrawer(*drawer);
    m_pFishnet->setMassDrawer(*drawer);
}


void MainProcessor::onLeftButtonDown(float xCoord, float yCoord)
{
    m_pTouchedMass = m_pFishnet->isPointInMass(xCoord, yCoord); // found a mass, in which point P(xCoord, yCoord) in
}

void MainProcessor::onLeftButtonUp(float xCoord, float yCoord)
{
    if (m_pTouchedMass)
        m_pTouchedMass->setPosition(Vector2DFloat(xCoord, yCoord));
    m_pTouchedMass = 0;
}

void MainProcessor::onMouseMove(float xCoord, float yCoord)
{
    if (m_pTouchedMass)
        m_pTouchedMass->setPosition(Vector2DFloat(xCoord, yCoord));
}

} // namespace application