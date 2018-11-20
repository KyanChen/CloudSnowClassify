#ifndef READDATA_H
#define READDATA_H

#include"config.h"
#include"gdal_priv.h"
#include<iostream>

#include"getfeatures.h"


#ifdef Windows
#include<io.h>
#else
#include<dirent.h>
#endif


using namespace std;
using namespace cv;

class ReadData
{
public:

    ReadData();
    ReadData(string samplesDirectory, string labelsDirectory);

    bool readRSImgtoMat(string filePath, Mat &dst);
    bool readLabels(string fileName, Mat &dst);

    void getDataFB(vector<double> &train_y, vector<vector<double>> &train_x);
    void getDataCS(vector<double> &train_y, vector<vector<double>> &train_x);
    void getDataForTest(vector<double> &test_y, vector<vector<double>> &test_x);

    void showRSImg(Mat &src);

private:
    string samplesDirectory;
    string labelsDirectory;

#ifdef Windows
    bool getFilesPath(string &path, vector<string>& files);
#else
    bool getFilesPath(string &path, vector<string>& files);
#endif



};


#endif // READDATA_H
