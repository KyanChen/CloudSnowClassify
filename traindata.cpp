#include "traindata.h"


TrainData::TrainData()
{


}

TrainData::TrainData(string pathSamples, string pathLabels, string modelFileNameFB, string modelFileNameCS)
{
    this->pathSamples = pathSamples;
    this->pathLabels = pathLabels;
    this->modelFileNameFB = modelFileNameFB;
    this->modelFileNameCS = modelFileNameCS;
}

svm_model *TrainData::trainforFB()
{
    init_svm_parameter();

    Mat dstImg;
    ReadData reader(pathSamples, pathLabels);

    vector<double> train_y;
    vector<vector<double>> train_x;
    reader.getDataFB(train_y, train_x);
/*
    train_y.push_back(-1);
    train_y.push_back(1);
    train_y.push_back(1);
    train_y.push_back(1);
    train_y.push_back(-1);
    train_y.push_back(-1);
    train_y.push_back(-1);
    train_y.push_back(-1);
    train_y.push_back(1);
    train_y.push_back(1);


    double a1[4]={0.2414472,0.2355823,0.1358754,0.339198};
    vector<double> x(a1,a1+4);
    train_x.push_back(x);
    x.clear();

    double a2[4]={0.8846532,0.8807433,0.8768334,0.878788};
    vector<double> x1(a1,a1+4);
    train_x.push_back(x1);
    x1.clear();

    double a3[4]={0.8866082, 0.8924733,0.8963834,0.888563};
    vector<double> x3(a3,a3+4);
    train_x.push_back(x3);
    x3.clear();

    double a4[4]={0.4037152,0.4740963,0.41744,0.479961};
    vector<double> x4(a4,a4+4);
    train_x.push_back(x4);
    x4.clear();

    double a5[4]={-0.0263932,-0.004887593,-0.1045944,0.0518084};
    vector<double> x5(a5,a5+4);
    train_x.push_back(x5);
    x5.clear();

    double a6[4]={0.1026392,0.1202353,0.006842624,0.219941};
    vector<double> x6(a6,a6+4);
    train_x.push_back(x6);
    x6.clear();

    double a7[4]={0.118282,0.1769313,0.08113394,0.296188};
    vector<double> x7(a7,a7+4);
    train_x.push_back(x7);
    x7.clear();
*/


    this->trainSize = train_x.size();
    this->featureSize = train_x.front().size();

    //trainXScale(-1, 1, train_x);

    /*

    for(int i=0; i<trainSize; i++){
        cout<<"label:"<<train_y[i]<<"   ";
        for(int j=0; j<featureSize; j++){
            cout<<j+1<<":"<<train_x[i][j];
        }
        cout<<endl;
    }
    cout<<"sucessful"<<endl;
    */

    // cout<<train_x[100][2]<<":"<<train_x[200][1]<<endl;
    cout<<1111<<endl;
    init_svm_problem(train_y, train_x);
    svm_model* model = svm_train(&prob, &param);
    if(svm_save_model(modelFileNameFB.c_str(), model) == -1){
        cout<<"can't save model to file "<< modelFileNameFB<<endl;
        exit(1);
    }
    cout<<"save model to file:"<< modelFileNameFB<<"   sucessful"<<endl;
    svm_free_and_destroy_model(&model);
    svm_destroy_param(&param);
    free(prob.y);
    free(prob.x);
    vector<double>().swap(train_y);
    vector<vector<double>>().swap(train_x);
    return model;
}

svm_model *TrainData::trainforCS()
{
    init_svm_parameter();

    Mat dstImg;
    ReadData reader(pathSamples, pathLabels);

    vector<double> train_y;
    vector<vector<double>> train_x;
    reader.getDataCS(train_y, train_x);
/*
    train_y.push_back(-1);
    train_y.push_back(1);
    train_y.push_back(1);
    train_y.push_back(1);
    train_y.push_back(-1);
    train_y.push_back(-1);
    train_y.push_back(-1);
    train_y.push_back(-1);
    train_y.push_back(1);
    train_y.push_back(1);


    double a1[4]={0.2414472,0.2355823,0.1358754,0.339198};
    vector<double> x(a1,a1+4);
    train_x.push_back(x);
    x.clear();

    double a2[4]={0.8846532,0.8807433,0.8768334,0.878788};
    vector<double> x1(a1,a1+4);
    train_x.push_back(x1);
    x1.clear();

    double a3[4]={0.8866082, 0.8924733,0.8963834,0.888563};
    vector<double> x3(a3,a3+4);
    train_x.push_back(x3);
    x3.clear();

    double a4[4]={0.4037152,0.4740963,0.41744,0.479961};
    vector<double> x4(a4,a4+4);
    train_x.push_back(x4);
    x4.clear();

    double a5[4]={-0.0263932,-0.004887593,-0.1045944,0.0518084};
    vector<double> x5(a5,a5+4);
    train_x.push_back(x5);
    x5.clear();

    double a6[4]={0.1026392,0.1202353,0.006842624,0.219941};
    vector<double> x6(a6,a6+4);
    train_x.push_back(x6);
    x6.clear();

    double a7[4]={0.118282,0.1769313,0.08113394,0.296188};
    vector<double> x7(a7,a7+4);
    train_x.push_back(x7);
    x7.clear();
   */

    this->trainSize = train_x.size();
    this->featureSize = train_x.front().size();

    for(int i=0; i<trainSize; i++){
        cout<<"label:"<<train_y[i]<<"   ";
        for(int j=0; j<featureSize; j++){
            cout<<j+1<<":"<<train_x[i][j];
        }
        cout<<endl;
    }
    cout<<"sucessful"<<endl;




    // trainXScale(-1, 1, train_x);

    // cout<<train_x[100][2]<<":"<<train_x[200][1]<<endl;
    cout<<1111<<endl;
    init_svm_problem(train_y, train_x);
    svm_model* model = svm_train(&prob, &param);
    if(svm_save_model(modelFileNameCS.c_str(), model) == -1){
        cout<<"can't save model to file "<< modelFileNameCS<<endl;
        exit(1);
    }
    cout<<"save model to file:"<< modelFileNameCS<<"  sucessful"<<endl;
    svm_free_and_destroy_model(&model);
    svm_destroy_param(&param);
    free(prob.y);
    free(prob.x);
    vector<double>().swap(train_y);
    vector<vector<double>>().swap(train_x);
    return model;
}

void TrainData::trainXScale(double lower, double upper, vector<vector<double> > &train_x)
{
    this->featureMin = DBL_MAX;
    this->featureMax = -DBL_MAX;
    // find featureMin, featureMax
    for(size_t i=0; i< trainSize; i++){
        for(size_t j=0; j< featureSize; j++){
            this->featureMin = min(this->featureMin, train_x[i][j]);
            this->featureMax = max(this->featureMax, train_x[i][j]);
        }
    }
    // cout<<this->featureMin<<":"<<this->featureMax<<endl;
    // scale the data
    for(size_t i=0; i< trainSize; i++){
        for(size_t j=0; j< featureSize; j++){
            train_x[i][j] = valueScale(lower, upper, train_x[i][j]);
        }
    }
}

void TrainData::init_svm_parameter()
{
    // default values
    param.svm_type = C_SVC; // SVM的类型
    param.kernel_type = RBF; // 核函数
    param.degree = 3; // 多项式参数
    param.gamma = 0;	// 1/num_features // 核函数为poly/rbf/sigmoid的参数
    param.coef0 = 0; // 核函数为poly/sigmoid的参数
    param.nu = 0.5;
    //下面是训练所需的参数
    param.cache_size = 1000; // 训练所需的内存MB为单位
    param.C = 1; // 惩罚因子，越大训练时间越长
    param.eps = 1e-5; // 训练停止的标准(误差小于eps停止)
    param.p = 0.1;
    param.shrinking = 1; // 训练过程是否使用压缩
    param.probability = 0; // 是否做概率估计
    param.nr_weight = 0; // 权重的数目，目前只有两个值，默认为0
    param.weight_label = NULL; // 权重，元素个数由nr_weight决定
    param.weight = NULL; // C_SVC权重


    cross_validation = 0;
}

void TrainData::init_svm_problem(vector<double> train_y, vector<vector<double>> train_x)
{

    prob.l = trainSize;        // 训练样本数
    prob.y = new double[trainSize];
    prob.x = new svm_node*[trainSize];
    svm_node* node = new svm_node[trainSize*(1 + featureSize)];
    prob.y = vec2arr(train_y);
    // 按照格式打包
    for (int i = 0; i < trainSize; i++)
    {
        for (int j = 0; j < featureSize; j++)
        {
            // 类比成二维数组的操作
            node[(featureSize + 1) * i + j].index = j + 1;
            node[(featureSize + 1) * i + j].value = train_x[i][j];
        }
        node[(featureSize + 1) * i + featureSize].index = -1;
        prob.x[i] = &node[(featureSize + 1) * i];
    }

}

double *TrainData::vec2arr(vector<double> data)
{
    double *arr = new double[data.size()];
    for(int i=0; i<data.size(); i++){
        *(arr+i) = data.at(i);
    }
    return arr;

}

double TrainData::valueScale(double lower, double upper, double input)
{
    double value;
    /* skip single-valued attribute */
    if(featureMax == featureMin){
        return input;
    }
    return value = lower + (upper-lower) *(input-featureMin)/(featureMax-featureMin);
}
