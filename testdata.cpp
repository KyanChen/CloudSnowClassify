#include "testdata.h"

TestData::TestData()
{

}

TestData::TestData(string fileSample, string fileLabel, string modelFileNameFB, string modelFileNameCS)
{
    this->fileSample = fileSample;
    this->fileLabel = fileLabel;
    modelForFB = svm_load_model(modelFileNameFB.c_str());
    modelForCS = svm_load_model(modelFileNameCS.c_str());
}

void TestData::test(Mat &mask)
{
    ReadData reader;
    vector<double> test_y;
    vector<vector<double> > test_x;
    vector<uchar> output;

    int totalBackground = 0, predTotalBackground = 0;
    int totalCloud = 0, predTotalCloud = 0;
    int totalSnow = 0, predTotalSnow = 0;


    reader.getDataForTest(test_y, test_x);


    /*test_y.push_back(-1);
    test_y.push_back(1);
    test_y.push_back(1);
    test_y.push_back(1);
    test_y.push_back(-1);
    test_y.push_back(-1);
    test_y.push_back(-1);
    test_y.push_back(-1);
    test_y.push_back(1);
    test_y.push_back(1);


    double a1[4]={0.2414472,0.2355823,0.1358754,0.339198};
    vector<double> x(a1,a1+4);
    test_x.push_back(x);
    x.clear();

    double a2[4]={0.8846532,0.8807433,0.8768334,0.878788};
    vector<double> x1(a1,a1+4);
    test_x.push_back(x1);
    x1.clear();

    double a3[4]={0.8866082, 0.8924733,0.8963834,0.888563};
    vector<double> x3(a3,a3+4);
    test_x.push_back(x3);
    x3.clear();

    double a4[4]={0.4037152,0.4740963,0.41744,0.479961};
    vector<double> x4(a4,a4+4);
    test_x.push_back(x4);
    x4.clear();

    double a5[4]={-0.0263932,-0.004887593,-0.1045944,0.0518084};
    vector<double> x5(a5,a5+4);
    test_x.push_back(x5);
    x5.clear();

    double a6[4]={0.1026392,0.1202353,0.006842624,0.219941};
    vector<double> x6(a6,a6+4);
    test_x.push_back(x6);
    x6.clear();

    double a7[4]={0.118282,0.1769313,0.08113394,0.296188};
    vector<double> x7(a7,a7+4);
    test_x.push_back(x7);
    x7.clear();
    */


    int testSize = test_x.size();
    int featureSize = test_x.front().size();

    svm_node* node= new svm_node[1 + featureSize];
    for(size_t i = 0; i<testSize;i++){
        for(int j = 0; j<featureSize; j++){
            node[j].index = j+1;
            node[j].value = test_x[i][j];
        }
        node[featureSize].index = -1;

        double predFB = svm_predict(modelForFB, node);
        cout<<predFB<<endl;

        //calculate the total pixel number of background, cloud, snow
        switch (int(test_y.at(i))) {
        case 0:
            totalBackground++;
            break;
        case 128:
            totalCloud++;
            break;
        default:
            totalSnow++;
        }

        if(predFB == -1){
            output.push_back(0);
            if(test_y.at(i)==0){
                predTotalBackground++;
            }
        }
        else{
            double predCS = svm_predict(modelForCS, node);
            if(predCS == -1){
                output.push_back(128);
                if(test_y.at(i)==255){
                    predTotalCloud++;
                }
            }
            else{
                output.push_back(255);
                if(test_y.at(i)==255){
                    predTotalCloud++;
                }
            }
        }
    }
    Mat temp = Mat(output, true);
    mask = temp.reshape(0, 1);
    cout<<"FBSVM accuracy is:"<<predTotalBackground/totalBackground<<endl;
    cout<<"CSSVM accuracy is:"<<predTotalCloud/totalCloud<<endl;
}
