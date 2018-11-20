#ifndef GETFEATURES_H
#define GETFEATURES_H
#include"config.h"

class GetFeatures
{
public:
    GetFeatures();
    void loadData(Mat inputImg);
    vector<Mat> getFeaturesForFB();// classify foreground and background
    vector<Mat> getFeaturesForCS();


private:
    Mat inputImg;
    vector<Mat> getBandsValue();
};

#endif // GETFEATURES_H
