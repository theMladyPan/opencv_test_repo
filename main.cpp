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
typedef vector<Point> contour;


RNG rng(12345);

#define TEST_IMAGE "/home/stanke/samples/threshold4.png";

int showMat(string name, Mat &inpArr, int delay_ms=0){
    namedWindow(name, WINDOW_NORMAL);// Create a window for display.
    imshow(name, inpArr);                   // Show our image inside it.
    return waitKey(delay_ms);
}

void findEdges(contour input, Point &topLeft, Point &botRight){
    Point candidateTopLeft = *input.begin();
    Point candidateBotRight = *input.begin();

    for(auto it=input.begin();it!=input.end(); it++){
        if(it->x < candidateTopLeft.x or it->y < candidateTopLeft.y){
            candidateTopLeft = *it;
        }else if (it->x > candidateBotRight.x or it->y > candidateBotRight.y) {
            candidateBotRight = *it;
        }
    }
    topLeft = candidateTopLeft;
    botRight = candidateBotRight;
}


void drawColorContours(Mat &destArray, vector<contour> &contours, vector<Vec4i> &hierarchy){
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        drawContours( destArray, contours, (int)i, color, 1, LINE_8, hierarchy, 0 );
    }
}

int main(int argc, char *argv[])
{
  auto start = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed;


  string image_name;
  if(argc>1){
      image_name = argv[1];
    }
  if(image_name.empty()){
      image_name = TEST_IMAGE;
    }

  start = chrono::high_resolution_clock::now();
  auto iMat = new Mat();
  auto destMat = new Mat();
  *iMat = imread(image_name, CV_8UC1);
  copyMakeBorder(*iMat, *iMat, 1, 1, 1, 1, BORDER_CONSTANT, 255);

  if(iMat->empty()){
      cerr << "image is empty!" << endl;
      return -1;
    }

  // Setup SimpleBlobDetector parameters.
  SimpleBlobDetector::Params params;

  // Change thresholds
  params.minThreshold = 100;
  params.maxThreshold = 150;

  // Filter by Area.
  params.filterByArea = true ;
  params.minArea = 300;

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

  // Detect blobs.
  std::vector<KeyPoint> keypoints;
  detector->detect( *iMat, keypoints);

  // Draw detected blobs as red circles.
  // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob

  Mat im_with_keypoints = Mat::zeros(iMat->rows, iMat->cols, CV_8UC1);
  bitwise_not(im_with_keypoints, im_with_keypoints);
  drawKeypoints( *iMat, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

  // Show blobs
  // showMat("input", im_with_keypoints, 1);

  vector<contour> contours;
  vector<Vec4i> hierarchy;
  findContours(*iMat, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
  Mat drawing = Mat::zeros( iMat->size(), CV_8UC3 );
  drawColorContours(drawing, contours, hierarchy);

  //showMat( "Contours", drawing, 10 );
  vector<vector<Point>> largeContours;

  for(auto vec=contours.begin(); vec!=contours.end(); vec++){
      if(vec->size() > 50){
            largeContours.push_back(*vec);
        }
  }

  Mat arrLargeContours = Mat::zeros(drawing.rows, drawing.cols, CV_8UC3);
  drawColorContours(arrLargeContours, largeContours, hierarchy);
  //showMat("Large contours", arrLargeContours, 10);

  Mat withLine;
  Point tl, br;
  for(auto it=largeContours.begin(); it!=largeContours.end();it++){
      findEdges(*it, tl, br);
      arrowedLine(arrLargeContours,tl, br, Scalar(rng.uniform(0,256),rng.uniform(0,256),rng.uniform(0,256)), 1, LINE_8);
  }

  end = chrono::high_resolution_clock::now();
  elapsed = end-start;
  putText(arrLargeContours, "Test", Point(100,100), FONT_HERSHEY_COMPLEX, 1, Scalar(0,0,255), 1, LINE_8);
  Mat imatcolor = Mat(iMat->rows, iMat->cols, CV_8UC3);
  cvtColor(*iMat, imatcolor,COLOR_GRAY2BGR);
  addWeighted(arrLargeContours, 0.5, imatcolor, 0.5, 0, arrLargeContours);
  cout << "Got and calculated in " << elapsed.count() << " ms\n";
  showMat("Lines", arrLargeContours);

  return 0;
}
