#include <string>
#include <chrono>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <math.h>

using namespace cv;
using namespace std;

RNG rng(12345);

#define TEST_IMAGE "/home/stanke/samples/threshold4.png";

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

  if(iMat->empty()){
      cerr << "image is empty!" << endl;
      return -1;
    }



  // Setup SimpleBlobDetector parameters.
  SimpleBlobDetector::Params params;

  // Change thresholds
  // params.minThreshold = 10;
  //  params.maxThreshold = 200;

  // Filter by Area.
  params.filterByArea = true ;
  params.minArea = 500;

  // Filter by Circularity
  params.filterByCircularity = false;
  params.minCircularity = 0.1f;

  // Filter by Convexity
  params.filterByConvexity = false;
  params.minConvexity = 0.5f;

  // Filter by Inertia
  params.filterByInertia = false;
  params.maxInertiaRatio = .1f;


  Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);


  namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
  // imshow( "Display window", *iMat );                   // Show our image inside it.
  // Set up the detector with default parameters.

  // Detect blobs.
  std::vector<KeyPoint> keypoints;
  detector->detect( *iMat, keypoints);

  // Draw detected blobs as red circles.
  // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
  Mat im_with_keypoints = Mat::zeros(iMat->rows, iMat->cols, CV_8UC1);
  bitwise_not(im_with_keypoints, im_with_keypoints);
  drawKeypoints( *iMat, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

  for(auto kp=keypoints.begin();kp!=keypoints.end();kp++){
      cout<<kp->pt<<endl;
  }
  // Show blobs
  imshow("keypoints", im_with_keypoints );
  waitKey(0);

  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;
  findContours(*iMat, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

  Mat drawing = Mat::zeros( iMat->size(), CV_8UC3 );
  for( size_t i = 0; i< contours.size(); i++ )
  {
      Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
      drawContours( drawing, contours, (int)i, color, 1, LINE_8, hierarchy, 0 );
  }
  imshow( "Contours", drawing );
  waitKey(0);

  for(auto vec=contours.begin(); vec!=contours.end(); vec++){
      if(vec->size() > 100){
          cout<<*vec<<endl;
        }
  }

  return 0;
}
