#include "matrix_operations.h"

void average(cv::Mat &src, float &avgX, float &avgY)
{
    avgX=0;
    avgY=0;
    pixelVector data = extractPoints(src);
    average(data, avgX, avgY);
}



pixelVector extractPoints(Mat &src)
{
    assert(src.type()==0);
    pixelVector data;
    auto newMat = Mat(src);
    for(int y(0);y<src.cols;y++){
        for(int x(0);x<src.rows;x++){
            if(src.at<int>(x,y)>0){
                data.push_back(pixelPair(x,y));
            }
        }
    }
    return data;
}

void average(pixelVector &src, float &avgX, float &avgY)
{
    avgX=0;
    avgY=0;
    for(auto pixel:src){
        avgX+=pixel.first;
        avgY+=pixel.second;
    }
}

void average(pointsVector &src, float &avgX, float &avgY)
{
    avgX=0;
    avgY=0;
    for(auto pixel:src){
        avgX+=pixel.x;
        avgY+=pixel.y;
    }
}
