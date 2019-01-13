#ifndef LEASTSQUARES_H
#define LEASTSQUARES_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include "matrix_operations.h"

using namespace cv;
typedef vector<Point> Contour;

class LeastSquares
{
private:
    Mat dataset;
public:
    double k;
    double q;
    double R2;

    LeastSquares();
    LeastSquares(Mat &input);
    LeastSquares(Contour contour);

    double getX(double y);
    double getY(double x);
};

#endif // LEASTSQUARES_H
