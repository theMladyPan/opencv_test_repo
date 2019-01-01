#include <string>
#include <chrono>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>


using namespace cv;
using namespace std;

#define TEST_IMAGE "./test.png"

void showMat(Mat &mat){
    imshow( "progress", mat );                   // Show our image inside it.
    waitKey(0);
}

int main(int argc, char **argv)
{
    string image_name;
    if(argc>1){
        image_name = argv[1];
      }
    if(image_name.empty()){
        image_name = TEST_IMAGE;
      }

    namedWindow( "progress", WINDOW_NORMAL|WINDOW_KEEPRATIO|WINDOW_GUI_EXPANDED);// Create a window for display.
    Mat src, srcColor;
    src = imread(image_name, CV_8UC1);
    srcColor = imread(image_name, CV_8UC4);
    showMat(srcColor);
    if(src.empty()){
        cerr << "image is empty!" << endl;
        return -1;
      }

    Mat thr;
    threshold(src, thr, 250, 255, THRESH_BINARY_INV);
    Mat dilated;
    Mat kernel = Mat::ones(3,3,CV_8U);
    dilate(thr, dilated, kernel);
    Mat distance;
    distanceTransform(dilated, distance, DIST_L2, 3);
    normalize(distance, distance, 0, 1, NORM_MINMAX);
    threshold(distance, thr, 0.5, 1, THRESH_BINARY);
    Mat dist8U;
    thr.convertTo(dist8U, CV_8UC1);
    vector<vector<Point>> contours;
    findContours(dist8U, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat markers = Mat::zeros(distance.size(), CV_32SC1);

    for (size_t i = 0; i < contours.size(); i++)
        {
            drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i)+1), -1);
        }

    circle(markers, Point(5,5), 3, Scalar(255), -1);

    cvtColor(src, src, COLOR_GRAY2BGR);
    watershed(src, markers);

    Mat markers8U;
    markers.convertTo(markers8U, CV_8U);
    bitwise_not(markers8U, markers8U);
    showMat(markers8U);

    // Generate random colors
    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 256);
        int g = theRNG().uniform(0, 256);
        int r = theRNG().uniform(0, 256);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // Create the result image
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
            {
                dst.at<Vec3b>(i,j) = colors[index-1];
            }
        }
    }
    Mat colored;
    bitwise_and(src, dst, src);
    addWeighted(src, 0.5, dst, 0.5, 0, dst);
    showMat(dst);

    return 0;
}
