#include <string>
#include <chrono>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>

#include <math.h>
#include <cstdlib>
#include "noise_filter.h"

using namespace cv;
using namespace std;

#define TEST_IMAGE "/home/stanke/test.jpg";

int main(int argc, char *argv[])
{
  string image_name;
  if(argc>1){
      image_name = argv[1];
    }
  if(image_name.empty()){
      image_name = TEST_IMAGE;
    }

  auto iMat = new Mat();
  auto destMat = new Mat();
  *iMat = imread(image_name, CV_8UC1);
  *destMat = Mat::zeros(iMat->rows, iMat->cols, CV_8UC1);
  resize(*iMat, *iMat, Size(),0.3, 0.3, INTER_CUBIC);
  adaptiveThreshold(*iMat,*destMat, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 15, 5);

  if(destMat->empty()){
      cerr << "image is empty!" << endl;
      return -1;
    }

  namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
  imshow( "Display window", *destMat );                   // Show our image inside it.
  waitKey(0);

  Segmentation seg(*destMat);
  seg.calculate();


  delete iMat;
  delete destMat;
  return 0;
}
