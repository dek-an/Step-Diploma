#ifndef GESTURES_GESTUREDETECTOR_H_
#define GESTURES_GESTUREDETECTOR_H_

namespace core
{

class GestureDetector
{
protected:
    GestureDetector(long detectortype);

public:
    long type() const;
    bool isType(long detectorType) const;

public:
    void detect();


private:
    long    m_detectorType;
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

class PaperScissorsStoneDetector
    : public GestureDetector
{
public:
    static const long DetectorType;

public:
    PaperScissorsStoneDetector();

};

} // namespace core

#endif // GESTURES_GESTUREDETECTOR_H_