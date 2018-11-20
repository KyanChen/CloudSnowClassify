#ifndef TESTDATA_H
#define TESTDATA_H

#include"config.h"
#include"readData.h"

#include<string>
#include"svm.h"

using namespace std;

class TestData
{
public:
    TestData();
    TestData(string fileSample, string fileLabel, string modelFileNameFB, string modelFileNameCS);
    void test(Mat &mask);


private:
    string fileSample;
    string fileLabel;
    svm_model *modelForFB;
    svm_model *modelForCS;


};

#endif // TESTDATA_H
