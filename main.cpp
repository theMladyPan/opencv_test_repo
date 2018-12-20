#include <string>
#include <chrono>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

using namespace cv;
using namespace std;

#define TEST_IMAGE "/home/stanke/test_HQ.jpg";

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
  int *temp = new int();
  auto dst_it = destMat->begin<uint8_t>();
  for(auto it=iMat->begin<uint8_t>();it!=iMat->end<uint8_t>();it++){
      *temp = pow(*it, 1.2);
      *temp = min(*temp, 255);
      *dst_it++= *temp;
    }
  if(destMat->empty()){
      return -2;
    }else{
      //cout << *destMat<<endl;
    }
  imshow("Display window", *destMat);
  waitKey(0);
  return 0;
  return 0;
}
