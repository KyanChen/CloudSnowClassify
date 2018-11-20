#ifndef TRAINDATA_H
#define TRAINDATA_H
#include"config.h"
#include"readData.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "svm.h"
#include<iostream>



class TrainData
{
public:
    TrainData();
    TrainData(string pathSamples, string pathLabels, string modelFileNameFB, string modelFileNameCS);

    svm_model* trainforFB();
    svm_model* trainforCS();





private:
#define max(x,y) (((x)>(y))?(x):(y))
#define min(x,y) (((x)<(y))?(x):(y))
    void init_svm_parameter();   // 初始化训练参数
    void init_svm_problem(vector<double> train_y, vector<vector<double>> train_x);     // 打包训练样本
    double *vec2arr(vector<double> data);

    void trainXScale(double lower, double upper, vector<vector<double>> &train_x);
    double valueScale(double lower, double upper, double input);



    struct svm_parameter param;
    struct svm_problem prob;
    /*
    int l;      // 记录样本的总数
    double *y;  // 样本所属的标签(+1, -1)
    struct svm_node **x;    // 指向样本数据的二维数组(即一个矩阵，行数是样本数，列数是特征向量维度)
    */

    int cross_validation;//数据随机分成n个部分，并计算它们的交叉验证精度/均方误差
    int nr_fold;//n-fold cross validation: n must >= 2

    int trainSize;
    int featureSize;
    double featureMin;
    double featureMax;

    string pathSamples;
    string pathLabels;
    string modelFileNameFB;
    string modelFileNameCS;
};

#endif // TRAINDATA_H
