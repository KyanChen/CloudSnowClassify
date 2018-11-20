#include "getfeatures.h"


GetFeatures::GetFeatures()
{

}

void GetFeatures::loadData(Mat inputImg)
{
    this->inputImg =inputImg.clone();
}

vector<Mat> GetFeatures::getFeaturesForFB() //综合得到前景背景的特征
{
    vector<Mat> features; //总特征
    vector<Mat> feature1 = getBandsValue();
    for(auto it = feature1.begin(); it != feature1.end(); ++it){
        features.push_back(*it);
    }



    return features;

}

vector<Mat> GetFeatures::getFeaturesForCS()
{
    vector<Mat> features; //总特征
    vector<Mat> feature1 = getBandsValue();
    for(auto it = feature1.begin(); it != feature1.end(); ++it){
        features.push_back(*it);
    }

    return features;
}

vector<Mat> GetFeatures::getBandsValue() // 每个波段的数值
{
    vector<Mat> channels;
    split(inputImg, channels);
    return channels;
}
