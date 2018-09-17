#ifndef CONVERTIMAGEFORMAT_H
#define CONVERTIMAGEFORMAT_H


class ConvertImageFormat
{
public:
    ConvertImageFormat();

    static void YU12toYUV(char *outYuv, char *inYv12, int width, int height,int widthStep);
};

#endif // CONVERTIMAGEFORMAT_H
