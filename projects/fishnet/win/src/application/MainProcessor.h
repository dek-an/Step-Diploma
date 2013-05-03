#ifndef APPLICATION_MAINPROCESSOR_H_
#define APPLICATION_MAINPROCESSOR_H_


namespace phys
{
class Fishnet;
class Mass;
}

namespace application
{

class GLDrawer;

// class MainProcessor - an engine of programm;
// manages the functionality of the program
class MainProcessor
{
public:

    MainProcessor();
    ~MainProcessor();

    // call operate() method for mpFisnet
    void update();
    // draws all fishnet
    void draw() const;
    // sets drawer support
    void setDrawer(GLDrawer* drawer);

    void onLeftButtonDown(float xCoord, float yCoord);
    void onLeftButtonUp(float xCoord, float yCoord);
    void onMouseMove(float xCoord, float yCoord);

private:
    MainProcessor(const MainProcessor&);
    MainProcessor& operator=(const MainProcessor&);

private:
    phys::Fishnet*  m_pFishnet; // an object, that represent the fishnet
    phys::Mass*     m_pTouchedMass; // mass in wich point P(xCoord, yCoord) in;
                                    // not NULL, when the left mouse button is down and when
                                    // the coordinates of mouse is in this mass
};

} // namespace application

#endif // APPLICATION_MAINPROCESSOR_H_