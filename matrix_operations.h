#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <utility>

using namespace cv;
using namespace std;

typedef pair<int, int> pixelPair;
typedef vector<pixelPair> pixelVector;
typedef vector<Point> pointsVector;

void average(pixelVector &src, float &avgX, float &avgY);
void average(pointsVector &src, float &avgX, float &avgY);
void average(Mat &src, float &avgX, float &avgY);
pixelVector extractPoints(Mat &src);


template<typename T>
T sum(vector<T> array)
{
    T n;
    for(auto &elem:array){
         n += elem;
    }
    return n;
}


#endif // MATRIX_OPERATIONS_H

