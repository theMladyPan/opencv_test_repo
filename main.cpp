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
#include "segmentation.h"

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

  int at_block_size, at_C;
  if(argc>3){
      at_block_size = atoi(argv[2]);
      at_C = atoi(argv[3]);
    }else {
      at_block_size = 5;
      at_C = 0;
    }

  auto iMat = new Mat();
  auto destMat = new Mat();
  *iMat = imread(image_name, CV_8UC1);
  *destMat = Mat::zeros(iMat->rows, iMat->cols, CV_8UC1);
  if(iMat->empty()){
      cerr << "image is empty!" << endl;
      return -1;
    }

  namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
  // imshow( "Display window", *iMat );                   // Show our image inside it.

  imshow("Display window", *iMat);
  waitKey(0);
  adaptiveThreshold(*iMat, *destMat,255, ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,at_block_size, at_C);
  if(destMat->empty()){
      return -2;
    }else{
      resize(*destMat, *destMat, Size(),0.3, 0.3, INTER_CUBIC);
      imshow("Display window", *destMat);
      waitKey(0);
    }

  Segmentation *segm = new Segmentation(*iMat);
  segm->calculate();


  delete iMat;
  delete destMat;
  return 0;
}
