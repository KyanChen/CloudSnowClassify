#include"config.h"
#include"readimg.h"

int main(int argc, char *argv[])
{

    string path = "/Users/keyanchen/FileBox/Coding/Dataset/CSDataset/train/mss/GF1_WFV1_E3.6_N6.1_20141021_L1A0000402374.tif";
    Mat dstImg;
    ReadIMG reader;
    reader.toMat(path, dstImg);
    reader.showRSImg(dstImg);
    return 0;
}
