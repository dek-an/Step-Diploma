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
    virtual void detect() = 0;

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
    ~PaperScissorsStoneDetector();

public:
    virtual void detect(/*some contour info here*/);
};

} // namespace core

#endif // GESTURES_GESTUREDETECTOR_H_