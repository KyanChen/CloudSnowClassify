#include "readData.h"

ReadData::ReadData()
{

}

ReadData::ReadData(string samplesDirectory, string labelsDirectory)
{
    this->samplesDirectory = samplesDirectory;
    this->labelsDirectory = labelsDirectory;

}

bool ReadData::readLabels(string fileName, Mat &dst)
{
    dst = cv::imread(fileName, CV_LOAD_IMAGE_UNCHANGED);

    if (dst.empty()){
        cout << "load fail" << endl;
        return 0;
    }
    //cout<<"::"<<double(dst.at<uchar>(100,1000))<<endl;
}

bool ReadData::readRSImgtoMat(string filePath, Mat &dst)
{

    GDALAllRegister();//注册文件格式

    //printf("\n打开的文件为:\n%s\n\n", (char*)filePath.c_str());

    //GDAL数据集
    GDALDataset *poDataset = (GDALDataset*) GDALOpen((char*)filePath.c_str(), GA_ReadOnly);
    if( poDataset == NULL ){
            cout<<"文件打开失败!!!";
            return false;
    }

#if 0
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
     ***/
    double adfGeoTransform[6];
    if(poDataset->GetGeoTransform(adfGeoTransform) == CE_None){
        printf("Origin = (%.6f,%.6f)\n",
               adfGeoTransform[0], adfGeoTransform[3]);
        printf("PixelSize = (%.6f,%.6f)\n",
               adfGeoTransform[1], adfGeoTransform[5]);
    }

#endif



    int nImgSizeX = poDataset->GetRasterXSize();
    int nImgSizeY = poDataset->GetRasterYSize();
    int nBandCount = poDataset->GetRasterCount();

    GDALRasterBand *poBandTemp;   //遥感的一个波段
    GDALDataType dataType = poDataset->GetRasterBand(1)->GetRasterDataType();
    // cout<<dataType<<";;;;;;"<<endl;
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

    //销毁内存
    vector<ushort>().swap(pData);
    vector<Mat>().swap(outputVector);
    GDALClose(poDataset);
    GDALDestroyDriverManager();
}

void ReadData::showRSImg(Mat &src)
{
    vector<Mat> channels;
    split(src, channels);
    vector<Mat> channelsUsed;
    channelsUsed.push_back(channels.at(0));
    channelsUsed.push_back(channels.at(1));
    channelsUsed.push_back(channels.at(2));
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

#ifdef Windows
bool ReadData::getFilesPath(string &path, vector<string>& files){

    struct _finddata_t fileInfo;
    path = this->fileDirectory;
    string p;
    long hFile;
    //string strfind = path + "*";
    //按FileName命名规则匹配当前目录第一个文件
    //long Handle = _findfirst(strfind.c_str(), &fileInfo);
    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileInfo)) != -1)
    {
        do
        {
            if ((fileInfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
                    getFilesPath(p.assign(path).append("\\").append(fileInfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileInfo.name));
            }
        } while (_findnext(hFile, &fileInfo) == 0);

        _findclose(hFile);
    }
}
#else
bool ReadData::getFilesPath(string &path, vector<string> &files)
{
    DIR *dir;
/*
struct dirent
{
   long d_ino; // inode number 索引节点号
   off_t d_off; // offset to this dirent 在目录文件中的偏移
   unsigned short d_reclen; /* length of this d_name 文件名长
   unsigned char d_type; /* the type of d_name 文件类型
   char d_name [NAME_MAX+1]; /* file name (null-terminated) 文件名，最长255字符
}
*/
    struct dirent *ptr;
    string p;
    if ((dir=opendir(path.c_str())) == NULL)
    {
        cout<<"Open dir error...";
        return 0;
    }

    //其中d_type表明该文件的类型：文件(8)、目录(4)、链接文件(10)
    while((ptr=readdir(dir)) != NULL){
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
            continue;
        else if(ptr->d_type == 8){
            string sFile = ptr->d_name;
            int iPos = sFile.find_last_of(".");
            if ((iPos == string::npos) || (strcmp(".tif", sFile.substr(iPos).c_str()))){
                continue;
            }
            files.push_back(p.assign(path).append("/").append(ptr->d_name));
            // cout<<files.back()<<endl;
            // printf("d_name:%s/%s\n",files.back());
        }
        else if(ptr->d_type == 4){
            // strcpy(base,basePath);    strcat(base,"/");   strcat(base,ptr->d_name);
            // getFilesPath(p.assign(path).append("/").append(ptr->d_name), files);
        }
    }
    closedir(dir);
    return 1;
}
#endif


void ReadData::getDataFB(vector<double> &train_y, vector<vector<double>> &train_x){

    vector<string> samplesFiles;
    vector<string> labelsFiles;


    Mat singleSample;
    Mat singleLabel;
    vector<Mat> features;

    getFilesPath(samplesDirectory, samplesFiles);
    getFilesPath(labelsDirectory, labelsFiles);
    // cout<<samplesFiles.size()<<"   "<<labelsFiles.size()<<endl;
    if(samplesFiles.size() != labelsFiles.size()){
        cout<< "样本与标签数量不匹配" <<endl;
    }

    GetFeatures getF;

    for(int i=0; i<samplesFiles.size(); i++){
        cout<<samplesFiles.at(i)<<endl<<labelsFiles.at(i)<<endl<<endl;
        // cout<<"1:"<<i<<endl;
        readRSImgtoMat(samplesFiles.at(i), singleSample);
        readLabels(labelsFiles.at(i), singleLabel);

        getF.loadData(singleSample);
        features = getF.getFeaturesForFB();

        // 改成确定值加快速度

        vector<double> singleSampleF;
        double threshold;
        // cout<<"4:"<<features.size()<<endl;
        for(int n_rows = 0; n_rows < features.front().rows; n_rows++){
            uchar *labelValue = singleLabel.ptr<uchar>(n_rows);

            for(size_t n_cols = 0; n_cols < features.front().cols ; n_cols++){

                for(size_t n_dims = 0; n_dims < features.size(); n_dims++){
                    Mat temp = features.at(n_dims);
                    ushort *data = temp.ptr<ushort>(n_rows);
                    // cout<<"5:"<<double(data[n_cols])<<endl;
                    singleSampleF.push_back(double(data[n_cols]));

               }
               train_x.push_back(singleSampleF);
               // cout<<"2:"<<train_x.front().size()<<endl;
               vector<double>().swap(singleSampleF);

               // cout<<singleSampleF.size()<<":::"<<endl;
               threshold = double(labelValue[n_cols]);

               if(threshold<128){
                   // cout<<"3:"<<"-1"<<endl;
                   train_y.push_back(0);
               }
               else{
                   // cout<<"3:"<<"1"<<endl;
                   train_y.push_back(1024);
               }
           }
        }
   }

}

void ReadData::getDataCS(vector<double> &train_y, vector<vector<double> > &train_x)
{
    vector<string> samplesFiles;
    vector<string> labelsFiles;


    Mat singleSample;
    Mat singleLabel;
    vector<Mat> features;

    getFilesPath(samplesDirectory, samplesFiles);
    getFilesPath(labelsDirectory, labelsFiles);
    // cout<<samplesFiles.size()<<"   "<<labelsFiles.size()<<endl;
    if(samplesFiles.size() != labelsFiles.size()){
        cout<< "样本与标签数量不匹配" <<endl;
    }

    GetFeatures getF;

    for(int i=0; i<samplesFiles.size(); i++){
        cout<<samplesFiles.at(i)<<endl<<labelsFiles.at(i)<<endl<<endl;
        // cout<<"1:"<<i<<endl;
        readRSImgtoMat(samplesFiles.at(i), singleSample);
        readLabels(labelsFiles.at(i), singleLabel);

        getF.loadData(singleSample);
        features = getF.getFeaturesForCS();

        // 改成确定值加快速度

        vector<double> singleSampleF;
        double threshold;
        // cout<<"4:"<<features.size()<<endl;
        for(int n_rows = 0; n_rows < features.front().rows; n_rows++){
            uchar *labelValue = singleLabel.ptr<uchar>(n_rows);

            for(size_t n_cols = 0; n_cols < features.front().cols ; n_cols++){
                threshold = double(labelValue[n_cols]);
                if(threshold<128){
                    continue;
                }
                else if (threshold==128) {
                    train_y.push_back(0);
                }
                else{
                    train_y.push_back(1024);
                }
                for(size_t n_dims = 0; n_dims < features.size(); n_dims++){
                    Mat temp = features.at(n_dims);
                    ushort *data = temp.ptr<ushort>(n_rows);
                    // cout<<"5:"<<double(data[n_cols])<<endl;
                    singleSampleF.push_back(double(data[n_cols]));

               }
               train_x.push_back(singleSampleF);
               // cout<<"2:"<<train_x.front().size()<<endl;
               vector<double>().swap(singleSampleF);
           }
        }
   }

}

void ReadData::getDataForTest(vector<double> &test_y, vector<vector<double> > &test_x)
{

    Mat singleSample;
    Mat singleLabel;

    vector<Mat> featuresFB;
    vector<Mat> featuresCS;
    readRSImgtoMat(samplesDirectory, singleSample);
    readLabels(labelsDirectory, singleLabel);

    GetFeatures getF;
    getF.loadData(singleSample);
    featuresFB  = getF.getFeaturesForFB();
    featuresCS = getF.getFeaturesForCS();
    vector<double> singleSampleF;

    for(int n_rows = 0; n_rows < featuresFB.front().rows; n_rows++){
        uchar *labelValue = singleLabel.ptr<uchar>(n_rows);

        for(size_t n_cols = 0; n_cols < featuresFB.front().cols ; n_cols++){

            for(size_t n_dims = 0; n_dims < featuresFB.size(); n_dims++){
                Mat temp = featuresFB.at(n_dims);
                ushort *data = temp.ptr<ushort>(n_rows);

                // cout<<"5:"<<double(data[n_cols])<<endl;

                singleSampleF.push_back(double(data[n_cols]));

           }
           test_x.push_back(singleSampleF);

           // cout<<"2:"<<train_x.front().size()<<endl;
           vector<double>().swap(singleSampleF);

           test_y.push_back(double(labelValue[n_cols]));
       }
    }
}

