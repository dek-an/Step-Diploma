#ifndef PHYS_CONSTANTS_H_
#define PHYS_CONSTANTS_H_

#include "Vector2D.h"

namespace phys
{

const float PI_VALUE = 3.141592f;           // pi

const Vector2DFloat GRAVITATION(0, 7.81f);     // gravity force

const float SPRING_CONSTANT = 270;          // spring stiffness
const float SPRING_LENGTH = 25;             // the length that the spring does not exert any force
const float SPRING_DELTA_STRAIN = 0;        // do not use...

const float AIR_FRICTION_CONSTANT = 0.6f;   // air friction coefficient

const int NUM_OF_MASSES_WIDTH = 15;         // number of fishnet points in width
const int NUM_OF_MASSES_HEIGHT = 10;        // number of fishnet points in height

const Vector2DFloat LEFT_CONNECTION(313, 70);  // left top fishnet connection point 

const float MASS = 4.5;                     // mass of any fishnet node

const float DT = 0.04f;                     // time step

const float POINT_RADIUS = 2.5;             // radius of the fishnet node

const float NUM_OF_INTEGRATE_STEPS = 4;     // number of intervals of integration

} // namespace phys

#endif // PHYS_CONSTANTS_H_