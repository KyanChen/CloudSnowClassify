#include "readimg.h"

ReadIMG::ReadIMG()
{

}

bool ReadIMG::toMat(string &filePath, Mat &dst)
{   

    GDALAllRegister();//注册文件格式
    printf("\n打开的文件为:\n%s\n\n", (char*)filePath.c_str());

    //GDAL数据集
    GDALDataset *poDataset = (GDALDataset*) GDALOpen((char*)filePath.c_str(), GA_ReadOnly);
    if( poDataset == NULL ){
            cout<<"文件打开失败!!!";
            return false;
    }

    //输出图像格式信息
    cout<<"输出图像格式信息:\t"<<endl;
    printf("Driver:\t%s/%s\n",
           poDataset->GetDriver()->GetDescription(),
           poDataset->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));

    //输出图像的大小和波段数
    printf("Size:%dx\t%dx\t%d\n",
           poDataset->GetRasterXSize(),
           poDataset->GetRasterYSize(),
           poDataset->GetRasterCount());

    //输出图像的投影信息

    if(strcmp("",poDataset->GetProjectionRef()) != 0){
        printf("Projrction:%s\n", poDataset->GetProjectionRef());
    }
    //输出图像的坐标和分辨率信息
    /*
     0:左上角X坐标
     1:图像横向（东西）分辨率
     2:旋转角度，若北朝上则为0；
     3:左上角Y坐标
     4:旋转角度，若北朝上则为0；
     5:图像纵向（南北）分辨率
     */
    double adfGeoTransform[6];
    if(poDataset->GetGeoTransform(adfGeoTransform) == CE_None){
        printf("Origin = (%.6f,%.6f)\n",
               adfGeoTransform[0], adfGeoTransform[3]);
        printf("PixelSize = (%.6f,%.6f)\n",
               adfGeoTransform[1], adfGeoTransform[5]);
    }



    int nImgSizeX = poDataset->GetRasterXSize();
    int nImgSizeY = poDataset->GetRasterYSize();
    int nBandCount = poDataset->GetRasterCount();

    GDALRasterBand *poBandTemp;   //遥感的一个波段
    GDALDataType dataType = poDataset->GetRasterBand(1)->GetRasterDataType();

    // vector<vector<ushort>> pData(nBandCount,vector<ushort>(nImgSizeX*nImgSizeY));
    vector<ushort> pData(nImgSizeX*nImgSizeY);
    vector<Mat> outputVector;

    for(int i = 0; i<nBandCount; i++){
        poBandTemp = poDataset->GetRasterBand(i+1);
        poBandTemp->RasterIO(GF_Read, 0, 0, nImgSizeX, nImgSizeY, &pData.at(0), nImgSizeX, nImgSizeY, dataType, 0, 0);
        Mat temp = Mat(pData, true);
        /*
        for(int row=0; row<temp.rows; row++){
            ushort * dataImg = temp.ptr<ushort>(row);
            for(int col=0; col<temp.cols; col++){
                printf("%d\n",dataImg[col]);
            }
        }
        */
        Mat dest = temp.reshape(0, nImgSizeY);
        outputVector.push_back(dest);
    }
    merge(outputVector, dst);

    vector<ushort>().swap(pData);
    vector<Mat>().swap(outputVector);
}

void ReadIMG::showRSImg(Mat &src)
{
    vector<Mat> channels;
    split(src, channels);
    vector<Mat> channelsUsed;
    channelsUsed.push_back(channels.at(2));
    channelsUsed.push_back(channels.at(1));
    channelsUsed.push_back(channels.at(0));
    Mat dst;
    merge(channelsUsed, dst);
    normalize(dst, dst, 0,255, cv::NORM_MINMAX);
    dst.convertTo(dst, CV_8UC3);
    // printf("%d\n",dst.at<uchar>(2,2));
    namedWindow("RS Img",0);
    resizeWindow("RS Img", 640, 480);
    imshow("RS Img", dst);

    if(waitKey(0) == 27){
        destroyAllWindows();
    }
    vector<Mat>().swap(channels);
    vector<Mat>().swap(channelsUsed);
}
