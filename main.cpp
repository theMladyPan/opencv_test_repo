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
  if(iMat->empty()){
      cerr << "image is empty!" << endl;
      return -1;
    }

  namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
  // imshow( "Display window", *iMat );                   // Show our image inside it.

  imshow("Display window", *iMat);
  waitKey(0);

  PixAttrPool *pool = new PixAttrPool();
  cout << pool->addPixAttr().getColor() << endl;

  cout << pool->addPixAttr().getColor() << endl;
  auto newPA = pool->addPixAttr();
  newPA.addPair(1,2);
  cout << pool->hasPixAttr(&newPA)<<endl;
  cout << pool->hasPixAttr(new PixAttr(157))<<endl;
  cout << getIndex(pool, pair<uint16_t, uint16_t>(1,2));

  delete pool;
  delete iMat;
  delete destMat;
  return 0;
}
