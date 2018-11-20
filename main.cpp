#include"config.h"
#include"readData.h"
#include"traindata.h"
#include"testdata.h"

int main(int argc, char *argv[])
{
    string pathSamples = "/Users/keyanchen/FileBox/Coding/Dataset/train/mss";
    string pathLabels = "/Users/keyanchen/FileBox/Coding/Dataset/train/label";
                        //GF1_WFV4_E111.2_N42.3_20160120_L1A0001354901_roiMap.tif";
    string modelFileNameFB = "modelFB.model";
    string modelFileNameCS = "modelCS.model";

    TrainData trainer(pathSamples, pathLabels, modelFileNameFB, modelFileNameCS);
    trainer.trainforCS();

    //Mat mask;
    //TestData tester(pathSamples, pathLabels, modelFileNameFB, modelFileNameCS);
    //tester.test(mask);




    /*Mat dstImg;
    ReadData reader(pathSamples, pathLabels);
    vector<double> train_y;
    //train_y.reserve()
    vector<vector<double>> train_x;

    reader.getData(train_y, train_x);*/







    /*Mat pData1=Mat::ones(3, 3, CV_32F)*2.;
    Mat pData2=Mat::ones(3, 3, CV_32F)*3.;
    Mat pData3=Mat::ones(3, 3, CV_32F)*4.;
    vector<Mat> ou;
    ou.push_back(pData1);
    ou.push_back(pData2);
    ou.push_back(pData3);
    Mat dst;
    merge(ou, dst);

    for( size_t nrow = 0; nrow < dst.rows; nrow++)
        {
           float *data = dst.ptr<float>(nrow);
           for(size_t ncol = 0; ncol < dst.cols ; ncol++)
           {

               for(size_t ncha = 0; ncha < dst.channels(); ncha++)
               {
                    cout << int( data[ncol*dst.cols+ncha] ) ;
               }
               cout << endl;

           }


        }
    //cout<<dst.at(1,1);*/

    return 0;
}
