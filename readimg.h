#ifndef READIMG_H
#define READIMG_H


#include"config.h"

#include"gdal_priv.h"



using namespace std;
using namespace cv;

class ReadIMG
{
public:
    ReadIMG();
    bool toMat(string &filePath, Mat &dst);
    void showRSImg(Mat &src);
};

#endif // READIMG_H
