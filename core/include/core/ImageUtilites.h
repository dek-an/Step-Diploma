#ifndef CORE_IMAGEUTILITES_H_
#define CORE_IMAGEUTILITES_H_

namespace core
{

class ImageUtilites
{
public:
    static void Yuv2Yuv420sp(const unsigned char* const yuvSrc, unsigned char* yuv420spDst,
        int width, int height);
};

} // namespace core

#endif // CORE_IMAGEUTILITES_H_